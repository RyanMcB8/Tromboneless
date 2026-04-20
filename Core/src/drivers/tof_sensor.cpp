#include "drivers/tof_sensor.hpp"

/**
 * @file tof_sensor.cpp
 * @brief Implementation of the ToFSensor class.
 *
 * @details
 * Implements VL53L1X initialisation, GPIO interrupt handling via libgpiod,
 * and callback-based publication of distance measurements.
 */

#include "vl53l1_platform.h"

extern "C" {
#include "VL53L1X_api.h"
}

#include <chrono>

using namespace std::chrono_literals;

ToFSensor::ToFSensor(I2CBus& bus,
                     uint8_t i2cAddress7Bit,
                     const std::string& gpioChipPath,
                     unsigned int gpioLine,
                     bool isTest)
    : bus_(bus),
      i2cAddress7Bit_(i2cAddress7Bit),
      // ST Ultra-Lite driver uses 8-bit I2C addresses, so shift left by 1.
      stDevAddress_(static_cast<uint16_t>(i2cAddress7Bit) << 1),
      gpioChipPath_(gpioChipPath),
      gpioLine_(gpioLine),
      isTestMode(isTest)
{
}

ToFSensor::~ToFSensor()
{
    stop();
}

void ToFSensor::registerCallback(DistanceCallback cb)
{
    // Store Callback for publishing meaurements.
    distanceCallback_ = cb;
}

bool ToFSensor::initialise()
{
    
    /*  Stoppinng the rest of the function from being run to initialise hardware. */
    if (isTestMode){
        return true;
    }
    // Open Linux I2C bus before interacting with the sensor
    if (!bus_.openBus())
    {
        return false;
    }

    // Link ST driver layer to our I2C bus implementation
    VL53L1_PlatformInit(bus_);

    // Reset sensor into known state
    softReset();

    // Wait for sensor boot completion
    uint8_t state = 0;

    // ~200 ms total wait (100 x 2 ms)
    // prevents infinite loop if sensor is not responding
    int attempts = 100;

    while (state == 0 && attempts-- > 0)
    {
        if (VL53L1X_BootState(stDevAddress_, &state) != 0)
        {
            return false;
        }

        std::this_thread::sleep_for(2ms);
    }

    if (state == 0)
    {
        return false;
    }

    // Verify sensor identity
    uint16_t sensorId = 0;
    if (VL53L1X_GetSensorId(stDevAddress_, &sensorId) != 0)
    {
        return false;
    }

    if (sensorId != 0xEACC)
    {
        return false;
    }

    // Run ST-provided default configuration sequence.
    if (VL53L1X_SensorInit(stDevAddress_) != 0)
    {
        return false;
    }

    // Configure interrupt as active-high
    if (VL53L1X_SetInterruptPolarity(stDevAddress_, 1) != 0)
    {
        return false;
    }

    // Fix the ideal config for this project
    // short mode, 15 ms timing budget, 15 ms inter-measurement period
    if (VL53L1X_SetDistanceMode(stDevAddress_, 1) != 0)
    {
        return false;
    }

    if (VL53L1X_SetTimingBudgetInMs(stDevAddress_, 15) != 0)
    {
        return false;
    }

    if (VL53L1X_SetInterMeasurementInMs(stDevAddress_, 15) != 0)
    {
        return false;
    }

    return true;
}

void ToFSensor::softReset()
{

    /*  Stoppinng the rest of the function from being run to initialise hardware. */
    if (isTestMode){
        return;
    }
    // perform software reset by toggling reset register
    uint8_t resetLow = 0x00;
    bus_.writeBlock16(i2cAddress7Bit_, SOFT_RESET, &resetLow, 1);
    std::this_thread::sleep_for(2ms);

    uint8_t resetHigh = 0x01;
    bus_.writeBlock16(i2cAddress7Bit_, SOFT_RESET, &resetHigh, 1);
    std::this_thread::sleep_for(2ms);
}

void ToFSensor::start()
{

    /*  Stoppinng the rest of the function from being run to initialise hardware. */
    if (isTestMode){
        return;
    }
    // configure GPIO before starting ranging to avoid missing interrupts while setting up GPIO

    // Create a configuration object to describe how the GPIO line should behave
    gpiod::line_config lineCfg;

    // Apply sttings to the chosen GPIO line
    lineCfg.add_line_settings(
        gpioLine_, // GPIO line number (e.g. 4)

        // Create a temporary settings object for this line
        gpiod::line_settings()
            // Set the line as input since it's an interrupt from the sensor to the Pi
            .set_direction(gpiod::line::direction::INPUT)
            // Enable edge detection for rising edges (active-high interrupt)
            .set_edge_detection(gpiod::line::edge::RISING)
    );

    // Open the chip device (e.g. "/dev/gpiochip0")
    chip_ = std::make_shared<gpiod::chip>(gpioChipPath_);

    // Prepare a request builder for aquiring GPIO lines from the kernel
    auto builder = chip_->prepare_request();
    // Assign a name to this GPIO consumer
    builder.set_consumer("tof_sensor");
    // Attatch the previously defined line configuration to the request builder
    builder.set_line_config(lineCfg);

    // Request control of the GPIO line from the kernel.
    request_ = std::make_shared<gpiod::line_request>(builder.do_request());
    // GPIO line now generates events on sensor data-ready signal


    // Start sensor ranging
    startRanging();

    // Launch worker thread to handle events
    running_ = true;
    thread_ = std::thread(&ToFSensor::worker, this);
}

void ToFSensor::stop()
{

    /*  Stoppinng the rest of the function from being run to initialise hardware. */
    if (isTestMode){
        return;
    }
    if (running_) // only stop if the worker thread is currently active
    {
        running_ = false; // signal worker thread to stop

        // worker thread may be blocked inside
        // wait_edge_events (1000 ms) , so may not exit immediately.

        if (thread_.joinable()) // check the thread exists and has not been joined yet
        {
            thread_.join();
            // block here until the worker thread fully exits to ensure clean shutdown and resource release before proceeding.
        }
    }

    // Stop measurements before releasing resources
    stopRanging();

    if (request_)
    {
        request_->release();
    }

    if (chip_)
    {
        chip_->close();
    }
}

void ToFSensor::worker()
{

    /*  Stoppinng the rest of the function from being run to initialise hardware. */
    if (isTestMode){
        return;
    }
    while (running_)
    {
        // Block until a GPIO event is detected or timeout occurs (e.g. to check running_ flag periodically)
        // Timeout allows periodic checks of the running_ flag to allow clean shutdown
        bool ready = request_->wait_edge_events(1000ms);

        if (!running_)
        {
            break;
        }

        if (ready)
        {
            // Consume GPIO event from buffer before processing
            gpiod::edge_event_buffer buffer;
            request_->read_edge_events(buffer, 1);

            // Handle new measurement
            handleDataReady();
        }
    }
}

void ToFSensor::handleDataReady()
{

    /*  Stoppinng the rest of the function from being run to initialise hardware. */
    if (isTestMode){
        return;
    }
    // read measurement in response to data-ready interrupt and publish through callback
    uint16_t distance = readDistanceMm();

    // This is where the callback is actually used. Publish the new measurement if a callback is registered.
    if (distanceCallback_)
    {
        distanceCallback_(distance);
    }
}

void ToFSensor::startRanging()
{

    /*  Stoppinng the rest of the function from being run to initialise hardware. */
    if (isTestMode){
        return;
    }
    //direct wrapper around ST API call to start ranging mode. See VL53L1X_api.c for details.
    (void)VL53L1X_StartRanging(stDevAddress_);
}

void ToFSensor::stopRanging()
{

    /*  Stoppinng the rest of the function from being run to initialise hardware. */
    if (isTestMode){
        return;
    }
    // Direct wrapper around ST API call to stop ranging mode. See VL53L1X_api.c for details.
    (void)VL53L1X_StopRanging(stDevAddress_);
}

uint16_t ToFSensor::readDistanceMm()
{

    /*  Stoppinng the rest of the function from being run to initialise hardware. */
    if (isTestMode){
        return 0;
    }
    VL53L1X_Result_t result{};

    // Retrieve the latest measurement result from the sensor. If this fails, return an error value.
    if (VL53L1X_GetResult(stDevAddress_, &result) != 0)
    {
        return 16384;
    }

    // Clear interrupt after reading the result
    // If skipped, next interrupt may never occur
    if (VL53L1X_ClearInterrupt(stDevAddress_) != 0)
    {
        return 16384;
    }

    // Filter out invalid readings
    if (result.Status != 0)
    {
        return 16384;
    }

    // Reject out-of-range values ( > 4m)
    if (result.Distance > 4000)
    {
        return 16384;
    }

    // Valid distance measurement is returned in millimetres
    return result.Distance;
}