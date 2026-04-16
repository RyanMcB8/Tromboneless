#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <string>
#include <thread>

#include <gpiod.hpp>

#include "drivers/i2c_bus.hpp"

/**
 * @brief Time-of-Flight sensor interface using I2C and GPIO interrupt handling.
 *
 * This class manages a VL53L1X sensor.
 * It communicates with the sensor over I2C, waits for a GPIO interrupt
 * to indicate that a new measurement is ready, and publishes the measured
 * distance via a callback.
 */

class ToFSensor
{
public:

    /**
     * @brief Callback type used to publish distance measurements.
     *
     * The callback receives one argument:
     * - measured distance in millimetres
     */

    using DistanceCallback = std::function<void(uint16_t)>;


    /** 
     * @brief Construct a ToFSensor object.
     * 
     * @param bus Reference to an I2CBus object for communication.
     * @param i2cAddress7Bit 7-bit I2C address of the sensor in 7-bit Linux form (0x29 by default).
     * @param gpioChipPath Path to the GPIO chip (e.g. "/dev/gpiochip0").
     * @param gpioLine GPIO line connected to the sensor interrupt output (e.g. 4).
     */
    ToFSensor(I2CBus& bus,
              uint8_t i2cAddress7Bit = 0x29,
              const std::string& gpioChipPath = "/dev/gpiochip0",
              unsigned int gpioLine = 4);

    /**
     * @brief Destructor ensures sensor is stopped and resources are released.
     */
    ~ToFSensor();

    /**
     * @brief Initialise the sensor and prepare it for operation.
     * 
     * Opens the I2C bus, initialises the ST platform layer, resets the sensor,
     * waits for boot, verifies sensor ID, and applies the configured ranging
     * and interrupt settings.
     * 
     * @return true if initialisation succeeded.
     * @return false if initialisation failed (e.g. I2C communication error).
     */
    bool initialise();

    /**
     * @brief Register a callback for new distance measurements.
     * 
     * @param cb The callback function to call when a new measurement is available.
     */
    void registerCallback(DistanceCallback cb);

    /**
     * @brief Start sensor ranging and launch the worker thread.
     */
    void start();

    /**
     * @brief Stop the worker thread, stop ranging, and release GPIO resources.
     */
    void stop();

private:
    
    /**
     * @brief Perform a software reset of the sensor.
     */
    void softReset();

    /**
     * @brief Put the sensor into ranging mode.
     */
    void startRanging();

    /**
     * @brief Stop the sensor ranging.
     */
    void stopRanging();

    /**
     * @brief Read the latest sensor result and return the distance.
     * 
     * Also clears the sensor interrupt and returns a default value 
     * (16384)if the read fails or the result is invalid.
     * 
     * @return Distance in millimetres, or an error value (16384) for invalid data.
     */
    uint16_t readDistanceMm();

    /**
     * @brief Worker thread function that waits for GPIO events and handles new measurements.
     */
    void worker();

    /**
     * @brief Handle a data-ready GPIO event from the sensor.
     * 
     * Reads the new measurement and publishes it through the callback.
     */
    void handleDataReady();

    /// Reference to the I2C bus for communication with the sensor
    I2CBus& bus_;

    /// Linux I2C address form, e.g. 0x29
    uint8_t i2cAddress7Bit_;

    /// ST ULD address form, e.g. 0x52
    uint16_t stDevAddress_;

    /// GPIO chip path for the data-ready interrupt, e.g. "/dev/gpiochip0"
    std::string gpioChipPath_;

    /// GPIO line number for the data-ready interrupt, e.g. 4
    unsigned int gpioLine_;

    /// Callback function to publish distance measurements
    DistanceCallback distanceCallback_;

    /// True while worker thread should keep running
    bool running_ = false;

    /// Background thread that waits for GPIO events
    std::thread thread_;

    /// Handle to the GPIO chip
    std::shared_ptr<gpiod::chip> chip_;

    /// Requested GPIO line for data-ready events
    std::shared_ptr<gpiod::line_request> request_;
};