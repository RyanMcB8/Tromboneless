/**
 * @file i2c_bus.cpp
 * @brief Implementation of the I2CBus class for Linux I2C communication.
 * 
 * @details 
 * Implements low-level Linux I2C device access using file descriptors,
 * ioctl device selection, and register-based read/write operations.
 */


#include "drivers/i2c_bus.hpp"

#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <cstring>
#include <cstdio>

/*
 * Constructor
 * Initialise file descriptor to invalid state
 */
I2CBus::I2CBus(const char* devicePath, bool isTest)
    : devicePath_(devicePath), fd_(-1), isTestMode(isTest)
{
}

// Destructor ensures the I2C device file is closed when the object is destroyed.
I2CBus::~I2CBus()
{
    closeBus();
}

// Open the I2C device file
bool I2CBus::openBus()
{
    if (isTestMode){
        return true;
    }

    if (fd_ >= 0)
    {
        // Already open
        return true;
    }

    // Open the I2C device file for read/write access
    fd_ = open(devicePath_, O_RDWR);
    return (fd_ >= 0);
}

// 
void I2CBus::closeBus()
{
    if (isTestMode){
        return;
    }
    
    if (fd_ >= 0)
    {
        // close file descriptor
        close(fd_);
        // mark as closed
        fd_ = -1;
    }
}

/*
 * Write data to a 16-bit register
 *
 * Transaction format:
 * [reg_high][reg_low][data...]
 */
int I2CBus::writeBlock16(uint8_t address, uint16_t reg, const uint8_t* data, uint8_t length)
{
    if (fd_ < 0)
    {
        //bus is not open, cannot communicate
        return -1;
    }

    // Select target device on the I2C bus
    if (ioctl(fd_, I2C_SLAVE, address) < 0)
    {
        return -1;
    }

    // Create buffer: register (2 bytes) + payload
    std::vector<uint8_t> buffer(2 + length);

    // split 16-bit register into two bytes
    buffer[0] = static_cast<uint8_t>(reg >> 8);   // register MSB
    buffer[1] = static_cast<uint8_t>(reg & 0xFF); // register LSB

    // Copy payload data into buffer if present
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
int I2CBus::readBlock16(uint8_t address, uint16_t reg, uint8_t* data, uint8_t length)
{
    if (fd_ < 0)
    {
        printf("File descriptor < 0");
        return -1;
    }

    // Select target device
    if (ioctl(fd_, I2C_SLAVE, address) < 0)
    {
        printf("Select target device failed");
        return -1;
    }

    // Send register address (2 bytes)
    uint8_t regBytes[2];
    regBytes[0] = static_cast<uint8_t>(reg >> 8);
    regBytes[1] = static_cast<uint8_t>(reg & 0xFF);

    ssize_t regWriteResult = write(fd_, regBytes, 2);
    if (regWriteResult != 2)
    {
        printf("regWriteResult != 2");
        return -1;
    }

    // Read requested number of bytes
    ssize_t bytesRead = read(fd_, data, length);

    return (bytesRead == length) ? 0 : -1;
}

/*
 * Write data to a 8-bit register
 *
 * Transaction format:
 * [reg_high][reg_low][data...]
 */
int I2CBus::writeBlock8(uint8_t address, uint8_t reg,
                        const uint8_t* data, uint8_t length)
{
    if (fd_ < 0)
        return -1;

    if (ioctl(fd_, I2C_SLAVE, address) < 0)
        return -1;

    std::vector<uint8_t> buffer(1 + length);

    buffer[0] = reg;

    if (length > 0)
        std::memcpy(&buffer[1], data, length);

    ssize_t result = write(fd_, buffer.data(), buffer.size());

    return (result == static_cast<ssize_t>(buffer.size())) ? 0 : -1;
}

/*
 * Read data from a 16-bit register
 *
 * Transaction:
 * 1. Write register address (2 bytes)
 * 2. Read data back
 */
 
int I2CBus::readBlock8(uint8_t address, uint8_t reg,
                       uint8_t* data, uint8_t length)
{
    if (fd_ < 0)
        return -1;

    if (ioctl(fd_, I2C_SLAVE, address) < 0)
        return -1;

    // Send register (1 byte)
    if (write(fd_, &reg, 1) != 1)
        return -1;

    // Read data
    ssize_t bytesRead = read(fd_, data, length);

    return (bytesRead == length) ? 0 : -1;
}

