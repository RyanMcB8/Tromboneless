#pragma once

#include <cstdint>
#include <functional>
#include <thread>
#include <memory>
#include <gpiod.hpp>

#include "drivers/i2c_bus.hpp"

/*
 * ToFSensor
 *
 * - Handles VL53L1X sensor
 * - Uses GPIO interrupt (blocking) for data-ready
 * - Publishes distance via std::function callback
 */

class ToFSensor
{
public:
    // Callback type: publishes distance in mm
    using DistanceCallbackInterface = std::function<void(uint16_t)>;

    ToFSensor(I2CBus& bus,
              uint8_t address = 0x29,
              const std::string& gpioChipPath = "/dev/gpiochip0",
              unsigned int gpioLine = 4);

    ~ToFSensor();

    // Initialise sensor (writes config + prepares device)
    bool initialise();

    // Register subscriber callback
    void registerCallback(DistanceCallbackInterface cb);

    // Start GPIO interrupt + ranging (spawns worker thread)
    void start();

    // Stop thread + ranging
    void stop();

private:
    // Worker thread: blocks on GPIO edge events
    void worker();

    // Called when GPIO signals "data ready"
    void handleDataReady();

    // Sensor control helpers
    void startRanging();
    void stopRanging();
    bool isDataReady();
    uint16_t readDistanceMm();
    void clearInterrupt();
    void softReset();

    I2CBus& bus_;            // I2C communication
    uint8_t address_;        // sensor address

    std::string gpioChipPath_;
    unsigned int gpioLine_;

    uint8_t interruptPolarity_ = 0;
    bool running_ = false;

    // Stored callback (publisher → subscriber)
    DistanceCallbackInterface distanceCallbackInterface_;

    // GPIO objects (libgpiod)
    std::shared_ptr<gpiod::chip> chip_;
    std::shared_ptr<gpiod::line_request> request_;
    std::thread thread_;

    // Default config block (must be provided)
    static const uint8_t DEFAULT_CONFIG[91];

    // Required register addresses
    static constexpr uint16_t SYSTEM_MODE_START = 0x0087;
    static constexpr uint16_t GPIO_TIO_HV_STATUS = 0x0031;
    static constexpr uint16_t SYSTEM_INTERRUPT_CLEAR = 0x0086;
    static constexpr uint16_t RESULT_RANGE_MM = 0x0096;
    static constexpr uint16_t GPIO_HV_MUX_CTRL = 0x0030;
    static constexpr uint16_t VHV_CONFIG_TIMEOUT_MACROP_LOOP_BOUND = 0x0008;
    static constexpr uint16_t VHV_CONFIG_INIT = 0x000B;
};