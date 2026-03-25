#pragma once

#include <cstdint>
#include "drivers/i2c_bus.hpp"

/*
 * Minimal VL53L1X ToF sensor class
 *
 * Responsibilities:
 * - Initialise sensor
 * - Start/stop ranging
 * - Read distance in mm
 */

class ToFSensor
{
public:
    explicit ToFSensor(I2CBus& bus, uint8_t address = 0x29);

    bool initialise();

    void startRanging();
    void stopRanging();

    bool isDataReady();
    uint16_t getDistanceMm();

private:
    I2CBus& bus_;
    uint8_t address_;

    uint8_t interruptPolarity_ = 0;

    // --- Registers ---
    static constexpr uint16_t SYSTEM_MODE_START = 0x0087;
    static constexpr uint16_t GPIO_TIO_HV_STATUS = 0x0031;
    static constexpr uint16_t SYSTEM_INTERRUPT_CLEAR = 0x0086;
    static constexpr uint16_t RESULT_RANGE_MM = 0x0096;
    static constexpr uint16_t GPIO_HV_MUX_CTRL = 0x0030;

    static constexpr uint16_t VHV_CONFIG_TIMEOUT_MACROP_LOOP_BOUND = 0x0008;
    static constexpr uint16_t VHV_CONFIG_INIT = 0x000B;

    // Default configuration (paste full array in .cpp)
    static const uint8_t DEFAULT_CONFIG[91];
};