#pragma once

#include <cstdint>

/*
 * Minimal Linux I2C bus wrapper
 *
 * Responsibility:
 * - Open/close I2C device (e.g. /dev/i2c-1)
 * - Perform register-based read/write transactions
 *
 * Does NOT:
 * - Know anything about specific sensors
 * - Perform any higher-level logic
 */

class I2CBus
{
public:
    // Construct with device path (default = Raspberry Pi I2C bus 1)
    explicit I2CBus(const char* devicePath = "/dev/i2c-1", bool isTest = false);

    // Destructor ensures bus is closed
    ~I2CBus();

    // Open the I2C device file
    bool openBus();

    // Close the I2C device file
    void closeBus();

    /*
     * Write a block of data to a device register
     *
     * address : 7-bit I2C device address
     * reg     : 16-bit register address
     * data    : pointer to data buffer
     * length  : number of bytes to write
     *
     * Returns 0 on success, -1 on failure
     */
    int writeBlock16(uint8_t address, uint16_t reg, const uint8_t* data, uint8_t length);

    /*
     * Read a block of data from a device register
     *
     * address : 7-bit I2C device address
     * reg     : 16-bit register address
     * data    : buffer to store result
     * length  : number of bytes to read
     *
     * Returns 0 on success, -1 on failure
     */
    int readBlock16(uint8_t address, uint16_t reg, uint8_t* data, uint8_t length);

        /*
     * Write a block of data to a device register
     *
     * address : 7-bit I2C device address
     * reg     : 8-bit register address
     * data    : pointer to data buffer
     * length  : number of bytes to write
     *
     * Returns 0 on success, -1 on failure
     */
    int writeBlock8(uint8_t address, uint8_t reg, const uint8_t* data, uint8_t length);

        /*
     * Read a block of data to a device register
     *
     * address : 7-bit I2C device address
     * reg     : 8-bit register address
     * data    : pointer to data buffer
     * length  : number of bytes to write
     *
     * Returns 0 on success, -1 on failure
     */
    int readBlock8(uint8_t address, uint8_t reg, uint8_t* data, uint8_t length);

private:
    const char* devicePath_;   // e.g. "/dev/i2c-1"
    int fd_;                   // file descriptor for the I2C device
    bool isTestMode = false;
};