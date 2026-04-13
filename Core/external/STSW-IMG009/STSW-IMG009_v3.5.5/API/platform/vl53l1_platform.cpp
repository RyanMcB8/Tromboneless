#include "vl53l1_platform.h"
#include "drivers/i2c_bus.hpp"

#include <chrono>
#include <cstdint>
#include <functional>
#include <optional>
#include <thread>

/*
 * This file connects the ST VL53L1X driver (ULD)
 * to your actual hardware (Linux I2C via I2CBus).
 *
 * The ST API (VL53L1X_api.c) does NOT talk to hardware directly.
 * Instead, it calls the functions in this file.
 *
 * Your job here is simply:
 * "when ST asks to read/write registers, use my I2CBus to do it"
 */

/*
 * We store a reference to the I2C bus that your application already created.
 *
 * std::optional is used so we can initialise it later (not at startup).
 * std::reference_wrapper lets us store a reference-like object inside optional.
 *
 * This avoids raw null pointers and keeps things safe.
 */
namespace
{
    std::optional<std::reference_wrapper<I2CBus>> g_bus;
}

/*
 * This function is called ONCE from your ToFSensor class.
 *
 * It "injects" your real I2CBus object into this platform layer,
 * so all ST functions can use it.
 */
void VL53L1_PlatformInit(I2CBus& bus)
{
    g_bus = bus;
}

/*
 * Helper function to get the stored bus reference.
 *
 * This assumes PlatformInit has already been called.
 * If not, this would be a logic error in your program flow.
 */
static I2CBus& platformBus()
{
    return g_bus->get();
}

/*
 * ST API uses an 8-bit style address (e.g. 0x52).
 * Linux I2C expects a 7-bit address (e.g. 0x29).
 *
 * So we convert by shifting right by 1.
 */
static uint8_t to7BitAddress(uint16_t dev)
{
    return static_cast<uint8_t>(dev >> 1);
}

/*
 * Write multiple bytes to a register.
 *
 * ST calls this when it wants to write a block of data.
 *
 * We simply forward it to your I2CBus::writeBlock().
 */
int8_t VL53L1_WriteMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count)
{
    // Your I2CBus uses uint8_t length, so we enforce that limit
    if (count > 255U)
        return -1;

    return static_cast<int8_t>(
        platformBus().writeBlock(
            to7BitAddress(dev),
            index,
            pdata,
            static_cast<uint8_t>(count)
        )
    );
}

/*
 * Read multiple bytes from a register.
 */
int8_t VL53L1_ReadMulti(uint16_t dev, uint16_t index, uint8_t *pdata, uint32_t count)
{
    if (count > 255U)
        return -1;

    return static_cast<int8_t>(
        platformBus().readBlock(
            to7BitAddress(dev),
            index,
            pdata,
            static_cast<uint8_t>(count)
        )
    );
}

/*
 * Write a single byte.
 *
 * This is heavily used by the ST API internally.
 */
int8_t VL53L1_WrByte(uint16_t dev, uint16_t index, uint8_t data)
{
    return static_cast<int8_t>(
        platformBus().writeBlock(
            to7BitAddress(dev),
            index,
            &data,
            1
        )
    );
}

/*
 * Write a 16-bit value (2 bytes).
 *
 * IMPORTANT:
 * The sensor expects big-endian format:
 * MSB first, then LSB.
 */
int8_t VL53L1_WrWord(uint16_t dev, uint16_t index, uint16_t data)
{
    uint8_t buffer[2] = {
        static_cast<uint8_t>(data >> 8),       // MSB
        static_cast<uint8_t>(data & 0xFF)      // LSB
    };

    return static_cast<int8_t>(
        platformBus().writeBlock(
            to7BitAddress(dev),
            index,
            buffer,
            2
        )
    );
}

/*
 * Write a 32-bit value (4 bytes).
 */
int8_t VL53L1_WrDWord(uint16_t dev, uint16_t index, uint32_t data)
{
    uint8_t buffer[4] = {
        static_cast<uint8_t>(data >> 24),
        static_cast<uint8_t>((data >> 16) & 0xFF),
        static_cast<uint8_t>((data >> 8) & 0xFF),
        static_cast<uint8_t>(data & 0xFF)
    };

    return static_cast<int8_t>(
        platformBus().writeBlock(
            to7BitAddress(dev),
            index,
            buffer,
            4
        )
    );
}

/*
 * Read a single byte.
 */
int8_t VL53L1_RdByte(uint16_t dev, uint16_t index, uint8_t *data)
{
    return static_cast<int8_t>(
        platformBus().readBlock(
            to7BitAddress(dev),
            index,
            data,
            1
        )
    );
}

/*
 * Read a 16-bit value (2 bytes).
 *
 * We read raw bytes, then reconstruct the integer.
 */
int8_t VL53L1_RdWord(uint16_t dev, uint16_t index, uint16_t *data)
{
    uint8_t buffer[2] = {0, 0};

    int rc = platformBus().readBlock(
        to7BitAddress(dev),
        index,
        buffer,
        2
    );

    if (rc != 0)
        return static_cast<int8_t>(rc);

    // Reassemble MSB + LSB into uint16_t
    *data = (static_cast<uint16_t>(buffer[0]) << 8) |
            static_cast<uint16_t>(buffer[1]);

    return 0;
}

/*
 * Read a 32-bit value (4 bytes).
 */
int8_t VL53L1_RdDWord(uint16_t dev, uint16_t index, uint32_t *data)
{
    uint8_t buffer[4] = {0, 0, 0, 0};

    int rc = platformBus().readBlock(
        to7BitAddress(dev),
        index,
        buffer,
        4
    );

    if (rc != 0)
        return static_cast<int8_t>(rc);

    *data = (static_cast<uint32_t>(buffer[0]) << 24) |
            (static_cast<uint32_t>(buffer[1]) << 16) |
            (static_cast<uint32_t>(buffer[2]) << 8)  |
            static_cast<uint32_t>(buffer[3]);

    return 0;
}

/*
 * Delay function used by ST driver.
 *
 * The sensor requires short waits during init and operation.
 * We simply map this to std::this_thread::sleep_for().
 */
int8_t VL53L1_WaitMs(uint16_t dev, int32_t wait_ms)
{
    (void)dev; // not used

    std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));
    return 0;
}