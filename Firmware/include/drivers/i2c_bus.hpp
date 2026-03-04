// i2c_bus.hpp
// Minimal, C++17-friendly I2C bus wrapper for Linux (/dev/i2c-*)
// No std::span, no templates. Just the essentials.
//
// Usage idea:
//   I2cBus bus("/dev/i2c-1");
//   uint8_t whoami = bus.readReg8_8(0x68, 0x75);

#pragma once

#include <cstdint>
#include <cstddef>
#include <string>

class I2cBus {
public:
    // Open the Linux I2C device node (e.g. "/dev/i2c-1").
    explicit I2cBus(const std::string& device_path);

    // Close the device node.
    ~I2cBus();

    // Not copyable (owns a file descriptor).
    I2cBus(const I2cBus&) = delete;
    I2cBus& operator=(const I2cBus&) = delete;

    // ---- Raw transactions (blocking) ----

    // Write 'length' bytes to the slave at 'addr'.
    void write(uint8_t addr, const uint8_t* data, std::size_t length);

    // Read 'length' bytes from the slave at 'addr' into 'data'.
    void read(uint8_t addr, uint8_t* data, std::size_t length);

    // Common pattern: write some bytes (often a register address), then read bytes.
    void writeThenRead(uint8_t addr,
                       const uint8_t* tx, std::size_t tx_len,
                       uint8_t* rx, std::size_t rx_len);

    // ---- Convenience register helpers ----

    // 8-bit register address, 8-bit value.
    uint8_t readReg8_8(uint8_t addr, uint8_t reg);
    void    writeReg8_8(uint8_t addr, uint8_t reg, uint8_t value);

    // 16-bit register address (big-endian address on wire), 8-bit value.
    uint8_t readReg8_16(uint8_t addr, uint16_t reg);
    void    writeReg8_16(uint8_t addr, uint16_t reg, uint8_t value);

    // 16-bit register address (big-endian address), 16-bit value read back (big-endian).
    uint16_t readReg16BE_16(uint8_t addr, uint16_t reg);

    // Advanced: expose fd if you later need it (usually you don't).
    int fd() const { return fd_; }

private:
    int fd_{-1};

    // Select the current slave address on this file descriptor.
    void setAddress_(uint8_t addr);

    // Throw runtime_error with errno message.
    [[noreturn]] static void throwSys_(const char* what);
};