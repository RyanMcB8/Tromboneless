// i2c_bus.cpp
#include "include/drivers/i2c_bus.hpp"

#include <cerrno>
#include <cstring>
#include <stdexcept>

#include <fcntl.h>        // open()
#include <unistd.h>       // close(), read(), write()
#include <sys/ioctl.h>    // ioctl()
#include <linux/i2c-dev.h> // I2C_SLAVE

[[noreturn]] void I2cBus::throwSys_(const char* what) {
    throw std::runtime_error(std::string(what) + ": " + std::strerror(errno));
}

I2cBus::I2cBus(const std::string& device_path) {
    // Open I2C bus device node.
    fd_ = ::open(device_path.c_str(), O_RDWR | O_CLOEXEC);
    if (fd_ < 0) {
        throwSys_("open(/dev/i2c-*) failed");
    }
}

I2cBus::~I2cBus() {
    // Never throw from a destructor.
    if (fd_ >= 0) {
        ::close(fd_);
        fd_ = -1;
    }
}

void I2cBus::setAddress_(uint8_t addr) {
    // Linux keeps the "current slave address" as state on the fd.
    // So if you talk to multiple devices, you must set this before each transaction.
    if (::ioctl(fd_, I2C_SLAVE, addr) < 0) {
        throwSys_("ioctl(I2C_SLAVE) failed");
    }
}

void I2cBus::write(uint8_t addr, const uint8_t* data, std::size_t length) {
    setAddress_(addr);

    // Blocking write. We expect all bytes to be written.
    const ssize_t n = ::write(fd_, data, length);
    if (n < 0) {
        throwSys_("i2c write() failed");
    }
    if (static_cast<std::size_t>(n) != length) {
        throw std::runtime_error("i2c write(): short write");
    }
}

void I2cBus::read(uint8_t addr, uint8_t* data, std::size_t length) {
    setAddress_(addr);

    // Blocking read. We expect all bytes to be read.
    const ssize_t n = ::read(fd_, data, length);
    if (n < 0) {
        throwSys_("i2c read() failed");
    }
    if (static_cast<std::size_t>(n) != length) {
        throw std::runtime_error("i2c read(): short read");
    }
}

void I2cBus::writeThenRead(uint8_t addr,
                           const uint8_t* tx, std::size_t tx_len,
                           uint8_t* rx, std::size_t rx_len) {
    // Common register read pattern:
    //   1) write register address bytes
    //   2) read back response bytes
    //
    // Note: This is two syscalls, which usually implies a STOP between them.
    // Many sensors accept this.
    // If you later find a device needs a repeated-start, you can upgrade this
    // to use I2C_RDWR with i2c_msg structs. Start simple first.
    write(addr, tx, tx_len);
    read(addr, rx, rx_len);
}

// ---- Convenience helpers ----

uint8_t I2cBus::readReg8_8(uint8_t addr, uint8_t reg) {
    uint8_t tx[1] = { reg };
    uint8_t rx[1] = { 0 };
    writeThenRead(addr, tx, 1, rx, 1);
    return rx[0];
}

void I2cBus::writeReg8_8(uint8_t addr, uint8_t reg, uint8_t value) {
    uint8_t tx[2] = { reg, value };
    write(addr, tx, 2);
}

uint8_t I2cBus::readReg8_16(uint8_t addr, uint16_t reg) {
    // Big-endian register address on the wire: [MSB][LSB]
    uint8_t tx[2] = {
        static_cast<uint8_t>(reg >> 8),
        static_cast<uint8_t>(reg & 0xFF)
    };
    uint8_t rx[1] = { 0 };
    writeThenRead(addr, tx, 2, rx, 1);
    return rx[0];
}

void I2cBus::writeReg8_16(uint8_t addr, uint16_t reg, uint8_t value) {
    uint8_t tx[3] = {
        static_cast<uint8_t>(reg >> 8),
        static_cast<uint8_t>(reg & 0xFF),
        value
    };
    write(addr, tx, 3);
}

uint16_t I2cBus::readReg16BE_16(uint8_t addr, uint16_t reg) {
    uint8_t tx[2] = {
        static_cast<uint8_t>(reg >> 8),
        static_cast<uint8_t>(reg & 0xFF)
    };
    uint8_t rx[2] = { 0, 0 };
    writeThenRead(addr, tx, 2, rx, 2);

    // Returned value interpreted as big-endian.
    return static_cast<uint16_t>(rx[0] << 8) | static_cast<uint16_t>(rx[1]);
}