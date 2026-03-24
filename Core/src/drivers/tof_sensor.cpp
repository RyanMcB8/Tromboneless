#include "drivers/tof_sensor.hpp"

#include <cstring>

// -----------------------------------------------------------------------------
// Simple bridge from the C-style ST platform callbacks to your C++ I2CBus object
// -----------------------------------------------------------------------------

namespace
{
    // Pointer to the currently active bus object
    // This keeps the callback bridge simple for now
    I2CBus* g_activeBus = nullptr;

    // Dummy multiplexer function
    // Not used unless you actually have an I2C multiplexer in the system
    int linux_i2c_multi(uint8_t address, uint16_t reg)
    {
        (void)address;
        (void)reg;
        return 0;
    }

    // Callback used by the ST platform layer for reads
    int linux_i2c_read(uint8_t address, uint16_t reg, uint8_t* data, uint8_t length)
    {
        if (g_activeBus == nullptr)
        {
            return -1;
        }

        return g_activeBus->readBlock(address, reg, data, length);
    }

    // Callback used by the ST platform layer for writes
    int linux_i2c_write(uint8_t address, uint16_t reg, uint8_t* data, uint8_t length)
    {
        if (g_activeBus == nullptr)
        {
            return -1;
        }

        return g_activeBus->writeBlock(address, reg, data, length);
    }
}

// -----------------------------------------------------------------------------
// ToFSensor implementation
// -----------------------------------------------------------------------------

ToFSensor::ToFSensor()
    : bus_("/dev/i2c-1")
{
    // Clear the device struct so all fields start in a known state
    std::memset(&dev_, 0, sizeof(dev_));

    // Set fixed sensor address
    dev_.I2cDevAddr = SENSOR_ADDRESS;

    // Disable mux behaviour in the ST platform layer
    // That layer only tries to use a mux if TCA9548A_Device < 8
    dev_.TCA9548A_Device = 0xFF;
    dev_.TCA9548A_Address = 0x00;
}

bool ToFSensor::initialise()
{
    // Open Linux I2C device
    if (!bus_.openBus())
    {
        return false;
    }

    // Make this bus visible to the platform callback bridge
    g_activeBus = &bus_;

    // Register Linux I2C callback functions with the ST platform layer
    VL53L1_set_i2c(
        reinterpret_cast<void*>(linux_i2c_multi),
        reinterpret_cast<void*>(linux_i2c_read),
        reinterpret_cast<void*>(linux_i2c_write)
    );

    return true;
}

bool ToFSensor::readRegisterByte(uint16_t reg, uint8_t& value)
{
    value = 0;

    VL53L1_Error status = VL53L1_RdByte(&dev_, reg, &value);

    return (status == VL53L1_ERROR_NONE);
}