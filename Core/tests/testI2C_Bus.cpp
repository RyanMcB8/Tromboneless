/**
 * @file test_i2c_bus.cpp
 * @brief Simple C++ tests for the I2CBus class.
 *
 * @details
 * This is a plain test executable, not using GoogleTest.
 * CTest will run this executable and treat exit code 0 as pass,
 * and non-zero as fail.
 *
 * These tests are intentionally simple because I2CBus is only a
 * light wrapper around Linux I2C functionality.
 *
 * What is tested here:
 * - openBus() returns true in test mode
 * - openBus() returns false for an invalid device path
 * - read/write functions fail safely when the bus is not truly open
 * - closeBus() is safe to call
 */

#include "drivers/i2c_bus.hpp"

#include <cstdint>
#include <exception>
#include <iostream>

/**
 * @brief Global counters for test results.
 */
static int testsRun = 0;
static int testsFailed = 0;

/**
 * @brief Simple helper to record and print a test result.
 *
 * @param condition True if the test passed.
 * @param testName Name/description of the test.
 */
void check(bool condition, const char* testName)
{
    ++testsRun;

    if (condition)
    {
        std::cout << "[PASS] " << testName << '\n';
    }
    else
    {
        std::cout << "[FAIL] " << testName << '\n';
        ++testsFailed;
    }
}

/**
 * @brief Main test runner.
 *
 * @return 0 if all tests pass, 1 otherwise.
 */
int main()
{
    std::cout << "Running I2CBus tests...\n\n";

    /**
     * Test 1:
     * In test mode, openBus() should return true because the function
     * is designed to bypass the real Linux open() call.
     */
    {
        I2CBus testBus("/dev/i2c-1", true);
        check(testBus.openBus() == true,
              "openBus() returns true in test mode");
    }

    /**
     * Test 2:
     * With an obviously invalid device path and normal mode,
     * openBus() should fail and return false.
     *
     * This does not require real I2C hardware.
     */
    {
        I2CBus invalidBus("/definitely/not/a/real/device", false);
        check(invalidBus.openBus() == false,
              "openBus() returns false for invalid device path");
    }

    /**
     * Test 3:
     * In test mode, the code skips the real open(),
     * but fd_ is still not a valid open file descriptor.
     *
     * Therefore writeBlock16() should fail safely and return -1.
     */
    {
        I2CBus testBus("/dev/i2c-1", true);
        uint8_t data[2] = {0xAA, 0x55};

        check(testBus.writeBlock16(0x29, 0x010F, data, 2) == -1,
              "writeBlock16() returns -1 when no real fd exists");
    }

    /**
     * Test 4:
     * Same idea as above, but for readBlock16().
     */
    {
        I2CBus testBus("/dev/i2c-1", true);
        uint8_t data[2] = {0x00, 0x00};

        check(testBus.readBlock16(0x29, 0x010F, data, 2) == -1,
              "readBlock16() returns -1 when no real fd exists");
    }

    /**
     * Test 5:
     * 8-bit register write should also fail safely if the bus
     * has not actually been opened.
     */
    {
        I2CBus testBus("/dev/i2c-1", true);
        uint8_t data[2] = {0x12, 0x34};

        check(testBus.writeBlock8(0x29, 0x0F, data, 2) == -1,
              "writeBlock8() returns -1 when no real fd exists");
    }

    /**
     * Test 6:
     * 8-bit register read should also fail safely if the bus
     * has not actually been opened.
     */
    {
        I2CBus testBus("/dev/i2c-1", true);
        uint8_t data[2] = {0x00, 0x00};

        check(testBus.readBlock8(0x29, 0x0F, data, 2) == -1,
              "readBlock8() returns -1 when no real fd exists");
    }

    /**
     * Test 7:
     * If openBus() failed because the path is invalid,
     * subsequent writeBlock16() should return -1 safely.
     */
    {
        I2CBus invalidBus("/definitely/not/a/real/device", false);
        uint8_t data[2] = {0x01, 0x02};

        check(invalidBus.writeBlock16(0x29, 0x010F, data, 2) == -1,
              "writeBlock16() returns -1 when bus is not open");
    }

    /**
     * Test 8:
     * Same idea for readBlock16().
     */
    {
        I2CBus invalidBus("/definitely/not/a/real/device", false);
        uint8_t data[2] = {0x00, 0x00};

        check(invalidBus.readBlock16(0x29, 0x010F, data, 2) == -1,
              "readBlock16() returns -1 when bus is not open");
    }

    /**
     * Test 9:
     * Same idea for writeBlock8().
     */
    {
        I2CBus invalidBus("/definitely/not/a/real/device", false);
        uint8_t data[2] = {0x01, 0x02};

        check(invalidBus.writeBlock8(0x29, 0x0F, data, 2) == -1,
              "writeBlock8() returns -1 when bus is not open");
    }

    /**
     * Test 10:
     * Same idea for readBlock8().
     */
    {
        I2CBus invalidBus("/definitely/not/a/real/device", false);
        uint8_t data[2] = {0x00, 0x00};

        check(invalidBus.readBlock8(0x29, 0x0F, data, 2) == -1,
              "readBlock8() returns -1 when bus is not open");
    }

    /**
     * Test 11:
     * closeBus() should be safe to call even if nothing was opened.
     * Since closeBus() returns void, we just check that it does not throw.
     */
    {
        try
        {
            I2CBus invalidBus("/definitely/not/a/real/device", false);
            invalidBus.closeBus();
            check(true, "closeBus() is safe on unopened bus");
        }
        catch (...)
        {
            check(false, "closeBus() is safe on unopened bus");
        }
    }

    std::cout << "\n-----------------------------\n";
    std::cout << "Tests run:    " << testsRun << '\n';
    std::cout << "Tests failed: " << testsFailed << '\n';
    std::cout << "-----------------------------\n";

    if (testsFailed == 0)
    {
        std::cout << "All I2CBus tests passed.\n";
        return 0;
    }

    std::cout << "Some I2CBus tests failed.\n";
    return 1;
}