#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <thread>

#include <gpiod.hpp>

#include "drivers/i2c_bus.hpp"

class ToFSensor
{
public:
    using DistanceCallback = std::function<void(uint16_t)>;

    ToFSensor(I2CBus& bus,
              uint8_t i2cAddress7Bit = 0x29,
              const std::string& gpioChipPath = "/dev/gpiochip0",
              unsigned int gpioLine = 4,
              bool isTest=false);

    ~ToFSensor();

    bool initialise();
    void registerCallback(DistanceCallback cb);
    void start();
    void stop();

private:
    void softReset();

    void startRanging();
    void stopRanging();
    uint16_t readDistanceMm();

    void worker();
    void handleDataReady();

    I2CBus& bus_;

    // Linux I2C address form, e.g. 0x29
    uint8_t i2cAddress7Bit_;

    // ST ULD address form, e.g. 0x52
    uint16_t stDevAddress_;

    std::string gpioChipPath_;
    unsigned int gpioLine_;

    DistanceCallback distanceCallback_;

    bool running_ = false;
    std::thread thread_;

    std::shared_ptr<gpiod::chip> chip_;
    std::shared_ptr<gpiod::line_request> request_;

    /*  A flag stored to inform the methods if it should ignore
        all hardware dependent functions. */
    bool isTestMode = false;
};