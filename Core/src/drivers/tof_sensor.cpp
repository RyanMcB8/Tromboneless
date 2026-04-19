#include "drivers/tof_sensor.hpp"

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
      stDevAddress_(static_cast<uint16_t>(i2cAddress7Bit) << 1),
      gpioChipPath_(gpioChipPath),
      gpioLine_(gpioLine)
{
}

ToFSensor::~ToFSensor()
{
    stop();
}

void ToFSensor::registerCallback(DistanceCallback cb)
{
    // This is where the callback is stored.
    distanceCallback_ = cb;
}

bool ToFSensor::initialise()
{
    // Open Linux I2C bus
    if (!bus_.openBus())
    {
        return false;
    }

    // Give the bus to the ST platform layer
    VL53L1_PlatformInit(bus_);

    // Reset sensor into known state
    softReset();

    // Wait for boot
    // Timing issue can appear here if sensor is slow to wake up.
    uint8_t state = 0;
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

    // Check sensor identity
    uint16_t sensorId = 0;
    if (VL53L1X_GetSensorId(stDevAddress_, &sensorId) != 0)
    {
        return false;
    }

    if (sensorId != 0xEACC)
    {
        return false;
    }

    // Run ST init
    if (VL53L1X_SensorInit(stDevAddress_) != 0)
    {
        return false;
    }

    // Make interrupt active high because we are using rising-edge GPIO detection
    if (VL53L1X_SetInterruptPolarity(stDevAddress_, 1) != 0)
    {
        return false;
    }

    // Hardcoded mode:
    // short mode, 15 ms timing budget, 15 ms inter-measurement
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
    // Direct reset register write
    uint8_t resetLow = 0x00;
    bus_.writeBlock16(i2cAddress7Bit_, SOFT_RESET, &resetLow, 1);
    std::this_thread::sleep_for(2ms);

    uint8_t resetHigh = 0x01;
    bus_.writeBlock16(i2cAddress7Bit_, SOFT_RESET, &resetHigh, 1);
    std::this_thread::sleep_for(2ms);
}

void ToFSensor::start()
{
    // This is where GPIO data-ready handling is set up.
    gpiod::line_config lineCfg;
    lineCfg.add_line_settings(
        gpioLine_,
        gpiod::line_settings()
            .set_direction(gpiod::line::direction::INPUT)
            .set_edge_detection(gpiod::line::edge::RISING)
    );

    chip_ = std::make_shared<gpiod::chip>(gpioChipPath_);

    auto builder = chip_->prepare_request();
    builder.set_consumer("tof_sensor");
    builder.set_line_config(lineCfg);

    request_ = std::make_shared<gpiod::line_request>(builder.do_request());

    // Start sensor ranging
    startRanging();

    // This is where the worker thread is created.
    running_ = true;
    thread_ = std::thread(&ToFSensor::worker, this);
}

void ToFSensor::stop()
{
    if (running_)
    {
        running_ = false;

        // Timing issue can show here if the thread is still blocked
        // waiting for GPIO events.
        if (thread_.joinable())
        {
            thread_.join();
        }
    }

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
    while (running_)
    {
        // This is where the code blocks waiting for data-ready GPIO.
        bool ready = request_->wait_edge_events(1000ms);

        if (!running_)
        {
            break;
        }

        if (ready)
        {
            // Consume GPIO event
            gpiod::edge_event_buffer buffer;
            request_->read_edge_events(buffer, 1);

            // Handle new measurement
            handleDataReady();
        }
    }
}

void ToFSensor::handleDataReady()
{
    uint16_t distance = readDistanceMm();

    // This is where the callback is actually used.
    if (distanceCallback_)
    {
        distanceCallback_(distance);
    }
}

void ToFSensor::startRanging()
{
    (void)VL53L1X_StartRanging(stDevAddress_);
}

void ToFSensor::stopRanging()
{
    (void)VL53L1X_StopRanging(stDevAddress_);
}

uint16_t ToFSensor::readDistanceMm()
{
    VL53L1X_Result_t result{};

    if (VL53L1X_GetResult(stDevAddress_, &result) != 0)
    {
        return 16384;
    }

    // This clear is essential.
    // If this is missed, the next data-ready interrupt may never arrive.
    if (VL53L1X_ClearInterrupt(stDevAddress_) != 0)
    {
        return 16384;
    }

    // Keep behaviour simple: only accept clean readings.
    if (result.Status != 0)
    {
        return 16384;
    }

    if (result.Distance > 4000)
    {
        return 16384;
    }

    return result.Distance;
}