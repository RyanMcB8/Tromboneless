#include "drivers/i2c_bus.hpp"

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <cstring>

/*
 * Constructor
 * Initialise file descriptor to invalid state
 */
I2CBus::I2CBus(const char* devicePath)
    : devicePath_(devicePath), fd_(-1)
{
}

/*
 * Destructor
 * Ensures file descriptor is closed on object destruction
 */
I2CBus::~I2CBus()
{
    closeBus();
}

/*
 * Open the I2C device file
 *
 * Returns true if successful
 */
bool I2CBus::openBus()
{
    if (fd_ >= 0)
    {
        // Already open
        return true;
    }

    fd_ = open(devicePath_, O_RDWR);
    return (fd_ >= 0);
}

/*
 * Close the I2C device file
 */
void I2CBus::closeBus()
{
    if (fd_ >= 0)
    {
        close(fd_);
        fd_ = -1;
    }
}

/*
 * Write data to a 16-bit register
 *
 * Transaction format:
 * [reg_high][reg_low][data...]
 */
int I2CBus::writeBlock(uint8_t address, uint16_t reg, const uint8_t* data, uint8_t length)
{
    if (fd_ < 0)
    {
        return -1;
    }

    // Select target device on the I2C bus
    if (ioctl(fd_, I2C_SLAVE, address) < 0)
    {
        return -1;
    }

    // Create buffer: register (2 bytes) + payload
    std::vector<uint8_t> buffer(2 + length);

    buffer[0] = static_cast<uint8_t>(reg >> 8);   // register MSB
    buffer[1] = static_cast<uint8_t>(reg & 0xFF); // register LSB

    // Copy payload data if present
    if (length > 0)
    {
        std::memcpy(&buffer[2], data, length);
    }

    // Write entire transaction to device
    ssize_t bytesWritten = write(fd_, buffer.data(), buffer.size());

    return (bytesWritten == static_cast<ssize_t>(buffer.size())) ? 0 : -1;
}

/*
 * Read data from a 16-bit register
 *
 * Transaction:
 * 1. Write register address (2 bytes)
 * 2. Read data back
 */
int I2CBus::readBlock(uint8_t address, uint16_t reg, uint8_t* data, uint8_t length)
{
    if (fd_ < 0)
    {
        return -1;
    }

    // Select target device
    if (ioctl(fd_, I2C_SLAVE, address) < 0)
    {
        return -1;
    }

    // Send register address (2 bytes)
    uint8_t regBytes[2];
    regBytes[0] = static_cast<uint8_t>(reg >> 8);
    regBytes[1] = static_cast<uint8_t>(reg & 0xFF);

    ssize_t regWriteResult = write(fd_, regBytes, 2);
    if (regWriteResult != 2)
    {
        return -1;
    }

    // Read requested number of bytes
    ssize_t bytesRead = read(fd_, data, length);

    return (bytesRead == length) ? 0 : -1;
}