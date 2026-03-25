#include "drivers/tof_sensor.hpp"

#include <chrono>

using namespace std::chrono_literals;

//full 91-byte config here
const uint8_t ToFSensor::DEFAULT_CONFIG[91] = {
	0x00, // 0x2d : set bit 2 and 5 to 1 for fast plus mode (1MHz I2C), else don't touch
	0x00, // 0x2e : bit 0 if I2C pulled up at 1.8V, else set bit 0 to 1 (pull up at AVDD)
	0x00, // 0x2f : bit 0 if GPIO pulled up at 1.8V, else set bit 0 to 1 (pull up at AVDD)
	0x01, // 0x30 : set bit 4 to 0 for active high interrupt and 1 for active low (bits 3:0 must be 0x1), use SetInterruptPolarity()
	0x02, // 0x31 : bit 1 = interrupt depending on the polarity, use CheckForDataReady()
	0x00, // 0x32 : not user-modifiable
	0x02, // 0x33 : not user-modifiable
	0x08, // 0x34 : not user-modifiable
	0x00, // 0x35 : not user-modifiable
	0x08, // 0x36 : not user-modifiable
	0x10, // 0x37 : not user-modifiable
	0x01, // 0x38 : not user-modifiable
	0x01, // 0x39 : not user-modifiable
	0x00, // 0x3a : not user-modifiable
	0x00, // 0x3b : not user-modifiable
	0x00, // 0x3c : not user-modifiable
	0x00, // 0x3d : not user-modifiable
	0xff, // 0x3e : not user-modifiable
	0x00, // 0x3f : not user-modifiable
	0x0F, // 0x40 : not user-modifiable
	0x00, // 0x41 : not user-modifiable
	0x00, // 0x42 : not user-modifiable
	0x00, // 0x43 : not user-modifiable
	0x00, // 0x44 : not user-modifiable
	0x00, // 0x45 : not user-modifiable
	0x20, // 0x46 : interrupt configuration: 0->level low detection, 1-> level high, 2-> Out of window, 3->In window, 0x20-> New sample ready, TBC
	0x0b, // 0x47 : not user-modifiable
	0x00, // 0x48 : not user-modifiable
	0x00, // 0x49 : not user-modifiable
	0x02, // 0x4a : not user-modifiable
	0x0a, // 0x4b : not user-modifiable
	0x21, // 0x4c : not user-modifiable
	0x00, // 0x4d : not user-modifiable
	0x00, // 0x4e : not user-modifiable
	0x05, // 0x4f : not user-modifiable
	0x00, // 0x50 : not user-modifiable
	0x00, // 0x51 : not user-modifiable
	0x00, // 0x52 : not user-modifiable
	0x00, // 0x53 : not user-modifiable
	0xc8, // 0x54 : not user-modifiable
	0x00, // 0x55 : not user-modifiable
	0x00, // 0x56 : not user-modifiable
	0x38, // 0x57 : not user-modifiable
	0xff, // 0x58 : not user-modifiable
	0x01, // 0x59 : not user-modifiable
	0x00, // 0x5a : not user-modifiable
	0x08, // 0x5b : not user-modifiable
	0x00, // 0x5c : not user-modifiable
	0x00, // 0x5d : not user-modifiable
	0x01, // 0x5e : not user-modifiable
	0xdb, // 0x5f : not user-modifiable
	0x0f, // 0x60 : not user-modifiable
	0x01, // 0x61 : not user-modifiable
	0xf1, // 0x62 : not user-modifiable
	0x0d, // 0x63 : not user-modifiable
	0x01, // 0x64 : Sigma threshold MSB (mm in 14.2 format for MSB+LSB), use SetSigmaThreshold(), default value 90 mm
	0x68, // 0x65 : Sigma threshold LSB
	0x00, // 0x66 : Min count Rate MSB (MCPS in 9.7 format for MSB+LSB), use SetSignalThreshold()
	0x80, // 0x67 : Min count Rate LSB
	0x08, // 0x68 : not user-modifiable
	0xb8, // 0x69 : not user-modifiable
	0x00, // 0x6a : not user-modifiable
	0x00, // 0x6b : not user-modifiable
	0x00, // 0x6c : interMeasurement period MSB, 32 bits register, use SetIntermeasurementInMs()
	0x00, // 0x6d : interMeasurement period
	0x0f, // 0x6e : interMeasurement period
	0x89, // 0x6f : interMeasurement period LSB
	0x00, // 0x70 : not user-modifiable
	0x00, // 0x71 : not user-modifiable
	0x00, // 0x72 : distance threshold high MSB (in mm, MSB+LSB), use SetD:tanceThreshold()
	0x00, // 0x73 : distance threshold high LSB
	0x00, // 0x74 : distance threshold low MSB ( in mm, MSB+LSB), use SetD:tanceThreshold()
	0x00, // 0x75 : distance threshold low LSB
	0x00, // 0x76 : not user-modifiable
	0x01, // 0x77 : not user-modifiable
	0x0f, // 0x78 : not user-modifiable
	0x0d, // 0x79 : not user-modifiable
	0x0e, // 0x7a : not user-modifiable
	0x0e, // 0x7b : not user-modifiable
	0x00, // 0x7c : not user-modifiable
	0x00, // 0x7d : not user-modifiable
	0x02, // 0x7e : not user-modifiable
	0xc7, // 0x7f : ROI center, use SetROI()
	0xff, // 0x80 : XY ROI (X=Width, Y=Height), use SetROI()
	0x9B, // 0x81 : not user-modifiable
	0x00, // 0x82 : not user-modifiable
	0x00, // 0x83 : not user-modifiable
	0x00, // 0x84 : not user-modifiable
	0x01, // 0x85 : not user-modifiable
	0x00, // 0x86 : clear interrupt, use ClearInterrupt()
	0x00, // 0x87 : start ranging, use StartRanging() or StopRanging(), If you want an automatic start after VL53L1X_init() call, put 0x40 in location 0x87
};

ToFSensor::ToFSensor(I2CBus& bus,
                     uint8_t address,
                     const std::string& gpioChipPath,
                     unsigned int gpioLine)
    : bus_(bus),
      address_(address),
      gpioChipPath_(gpioChipPath),
      gpioLine_(gpioLine)
{
}

ToFSensor::~ToFSensor()
{
    stop(); // ensure clean shutdown
}

void ToFSensor::registerCallback(DistanceCallbackInterface cb)
{
    // Store subscriber callback
    distanceCallbackInterface_ = cb;
}

bool ToFSensor::initialise()
{
    // Open I2C device
    if (!bus_.openBus())
        return false;


    // force sensor into known state
    softReset();
    
    // Write default configuration to sensor registers
    for (uint16_t reg = 0x2D; reg <= 0x87; ++reg)
    {
        uint8_t value = DEFAULT_CONFIG[reg - 0x2D];
        bus_.writeBlock(address_, reg, &value, 1);
    }

    // Start once to initialise internal state
    startRanging();

    // Wait until first measurement ready
    while (!isDataReady())
        std::this_thread::sleep_for(10ms);

    clearInterrupt();
    stopRanging();

    // Required additional config (ST recommended)
    uint8_t vhvBound = 0x09;
    bus_.writeBlock(address_, VHV_CONFIG_TIMEOUT_MACROP_LOOP_BOUND, &vhvBound, 1);

    uint8_t vhvInit = 0x00;
    bus_.writeBlock(address_, VHV_CONFIG_INIT, &vhvInit, 1);

    // Read interrupt polarity (used for data-ready detection)
    uint8_t tmp = 0;
    bus_.readBlock(address_, GPIO_HV_MUX_CTRL, &tmp, 1);
    interruptPolarity_ = !((tmp & 0x10) >> 4);

    return true;
}

void ToFSensor::softReset()
{
    // Put sensor into reset
    uint8_t resetLow = 0x00;
    bus_.writeBlock(address_, 0x0000, &resetLow, 1);

    std::this_thread::sleep_for(std::chrono::milliseconds(2));

    // Bring sensor out of reset
    uint8_t resetHigh = 0x01;
    bus_.writeBlock(address_, 0x0000, &resetHigh, 1);

    std::this_thread::sleep_for(std::chrono::milliseconds(2));
}

void ToFSensor::start()
{
    // Configure GPIO line for input + rising edge detection
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

    // Launch worker thread (blocking)
    running_ = true;
    thread_ = std::thread(&ToFSensor::worker, this);
}

void ToFSensor::stop()
{
    if (!running_)
        return;

    running_ = false;

    // Wait for thread to finish
    if (thread_.joinable())
        thread_.join();

    // Stop sensor measurements
    stopRanging();

    // Release GPIO resources
    if (request_)
        request_->release();

    if (chip_)
        chip_->close();
}

void ToFSensor::worker()
{
    while (running_)
    {
        // BLOCKING call — sleeps until GPIO edge event occurs
        bool ready = request_->wait_edge_events(std::chrono::milliseconds(1000));

        if (!running_)
            break;

        if (ready)
        {
            // Consume event (required by libgpiod)
            gpiod::edge_event_buffer buffer;
            request_->read_edge_events(buffer, 1);

            // Handle new measurement
            handleDataReady();
        }
    }
}

void ToFSensor::handleDataReady()
{
    // Read distance from sensor
    uint16_t distance = readDistanceMm();

    // Publish to subscriber if registered
    if (distanceCallbackInterface_)
        distanceCallbackInterface_(distance);
}

void ToFSensor::startRanging()
{
    // 0x40 → continuous ranging mode
    uint8_t value = 0x40;
    bus_.writeBlock(address_, SYSTEM_MODE_START, &value, 1);
}

void ToFSensor::stopRanging()
{
    // 0x00 → stop measurements
    uint8_t value = 0x00;
    bus_.writeBlock(address_, SYSTEM_MODE_START, &value, 1);
}

bool ToFSensor::isDataReady()
{
    uint8_t status = 0;
    bus_.readBlock(address_, GPIO_TIO_HV_STATUS, &status, 1);

    // Compare against configured interrupt polarity
    return (status & 0x01) == interruptPolarity_;
}

uint16_t ToFSensor::readDistanceMm()
{
    // Read 16-bit distance result (MSB first)
    uint8_t data[2] = {0, 0};
    bus_.readBlock(address_, RESULT_RANGE_MM, data, 2);

    uint16_t distance = (static_cast<uint16_t>(data[0]) << 8) | data[1];

    // Clear interrupt so next measurement can trigger
    clearInterrupt();

    // Clamp out-of-range values
    if (distance > 4000)
        return 16384;

    return distance;
}

void ToFSensor::clearInterrupt()
{
    // Must clear interrupt after each read
    uint8_t clear = 0x01;
    bus_.writeBlock(address_, SYSTEM_INTERRUPT_CLEAR, &clear, 1);
}