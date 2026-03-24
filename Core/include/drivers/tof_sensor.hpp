#pragma once

#include <cstdint>

#include "drivers/i2c_bus.hpp"

// ST headers are C headers, so keep them in extern "C"
extern "C"
{
#include "vl53l1_api.h"
#include "vl53l1_platform.h"
}

/*
 * Minimal VL53L1X sensor wrapper
 *
 * Responsibility:
 * - Own the I2C bus object
 * - Own the VL53L1 device struct
 * - Connect the ST platform layer to Linux I2C
 * - Provide a very small public interface for initial testing
 */

class ToFSensor
{
public:
    ToFSensor();
    ~ToFSensor() = default;

    // Open bus and prepare device for communication
    bool initialise();

    // Read a single byte from a sensor register
    bool readRegisterByte(uint16_t reg, uint8_t& value);

private:
    static constexpr uint8_t SENSOR_ADDRESS = 0x29;

    I2CBus bus_;
    VL53L1_Dev_t dev_;
};