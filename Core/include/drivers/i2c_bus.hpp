#pragma once

/**
 * @file i2c_bus.hpp
 * @brief Minimal Linux I2C bus wrapper.
 *
 * @details
 * This class provides basic functionality to open/close an I2C device and
 * perform register-based read/write transactions. It does not contain any
 * sensor-specific logic, making it reusable for various I2C devices.
 */

#include <cstdint>

/**
 * @brief Minimal Linux I2C bus wrapper for register-based communication.
 * 
 * This class:
 * - Opens and closes the I2C device file (e.g. "/dev/i2c-1").
 * - Provides methods to read/write blocks of data to/from 8-bit or 16-bit
 *  registers on I2C devices.
 * 
 * This class does not: 
 * - Handle any sensor-specific logic or protocols.
 */

class I2CBus
{
public:
    /**
     * @brief Construct an I2CBus object.
     * 
     * @param devicePath Path to the Linux I2C device file (e.g. "/dev/i2c-1").
     */
    explicit I2CBus(const char* devicePath = "/dev/i2c-1", bool isTest = false);

    /**
     * @brief Destructor ensures the I2C device file is closed.
     */
    ~I2CBus();

    /**
     * @brief Open the I2C device file.
     * 
     * If the bus is already open, this function returns success.
     * 
     * @return true if successful, false otherwise.
     */
    bool openBus();

    /**
     * @brief Close the I2C device file.
     */
    void closeBus();

    /**
     * @brief Write a block of data to a 16-bit register address.
     * 
     * @param address 7-bit I2C device address.
     * @param reg 16-bit register address.
     * @param data Pointer to the data buffer.
     * @param length Number of bytes to write.
     * 
     * @return 0 on success, -1 on failure.
     */
    int writeBlock16(uint8_t address, uint16_t reg, const uint8_t* data, uint8_t length);

    /**
     * @brief Read a block of data from a 16-bit register address.
     * 
     * @param address 7-bit I2C device address.
     * @param reg 16-bit register address.
     * @param data Pointer to the buffer where read data will be stored.
     * @param length Number of bytes to read.
     * 
     * @return 0 on success, -1 on failure.
     */
    int readBlock16(uint8_t address, uint16_t reg, uint8_t* data, uint8_t length);


    /**
     * @brief Write a block of data to an 8-bit register address.
     * 
     * @param address 7-bit I2C device address.
     * @param reg 8-bit register address.
     * @param data Pointer to the data buffer.
     * @param length Number of bytes to write.
     * 
     * @return 0 on success, -1 on failure.
     */
    int writeBlock8(uint8_t address, uint8_t reg, const uint8_t* data, uint8_t length);


    /**
     * @brief Read a block of data from an 8-bit register address.
     * 
     * @param address 7-bit I2C device address.
     * @param reg 8-bit register address.
     * @param data Pointer to the buffer where read data will be stored.
     * @param length Number of bytes to read.
     * 
     * @return 0 on success, -1 on failure.
     */
    int readBlock8(uint8_t address, uint8_t reg, uint8_t* data, uint8_t length);

private:

    /// Path to the Linux I2C device file (e.g. "/dev/i2c-1")
    const char* devicePath_; 

    /// File descriptor for the opened I2C device.
    int fd_;        
             
    /*  A flag stored to inform the methods if it should ignore
        all hardware dependent functions. */
    bool isTestMode = false;
};