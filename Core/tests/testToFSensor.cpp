/**
 * @file test_tof_sensor.cpp
 * @brief Simple plain-C++ tests for the ToFSensor class.
 *
 * @details
 * This file builds as a normal executable and is intended to be run by CTest.
 * It does not use GoogleTest or any third-party unit test framework.
 *
 * The ToFSensor class is strongly tied to hardware:
 * - it talks to the VL53L1X sensor over I2C
 * - it uses GPIO interrupts through libgpiod
 * - it starts a background worker thread in normal operation
 *
 * Because of that, and because we do not want to refactor the class just to
 * test it, these tests focus on what is realistically possible with the
 * current design:
 *
 * - construction is safe in test mode
 * - callback registration is safe
 * - initialise() succeeds in test mode
 * - start() is safe in test mode
 * - stop() is safe in test mode
 * - destruction is safe after use
 *
 * These are valid public-API tests for the current design.
 * The deeper behaviour of live GPIO interrupts and real sensor readings should
 * be verified separately through hardware integration testing.
 */

#include "drivers/i2c_bus.hpp"
#include "drivers/tof_sensor.hpp"

#include <cstdint>
#include <exception>
#include <iostream>

/**
 * @brief Count how many tests have been run.
 *
 * This is just used to print a simple summary at the end.
 */
static int testsRun = 0;

/**
 * @brief Count how many tests have failed.
 *
 * If this stays at zero, the executable will return 0 so that CTest marks the
 * whole test as passed.
 */
static int testsFailed = 0;

/**
 * @brief Small helper function to record a pass/fail result.
 *
 * @param condition True if the test has passed.
 * @param testName A short natural-language description of the test.
 *
 * This keeps the main() function readable and avoids repeating the same
 * output code over and over.
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
 * @brief Entry point for the test executable.
 *
 * @return 0 if all tests pass, non-zero otherwise.
 *
 * CTest uses the return code of this executable:
 * - return 0  -> test passed
 * - return 1  -> test failed
 */
int main()
{
    std::cout << "Running ToFSensor tests...\n\n";

    /**
     * First we create an I2CBus object in test mode.
     *
     * Why test mode?
     * Because we do not want this test executable to try to access real
     * hardware. In your current I2CBus design, test mode prevents the bus
     * from attempting to open the real Linux I2C device.
     *
     * This bus object is then passed into each ToFSensor object below.
     */
    I2CBus testBus("/dev/i2c-1", true);

    /**
     * Test 1:
     * Check that constructing a ToFSensor object in test mode is safe.
     *
     * In your implementation, the constructor only stores configuration values.
     * It does not touch the hardware.
     *
     * So this test is essentially confirming that object creation itself works
     * and does not throw any exceptions.
     */
    try
    {
        ToFSensor sensor(testBus, 0x29, "/dev/gpiochip0", 4, true);
        check(true, "constructor is safe in test mode");
    }
    catch (...)
    {
        check(false, "constructor is safe in test mode");
    }

    /**
     * Test 2:
     * Check that registering a callback is safe.
     *
     * In your implementation, registerCallback() simply stores the callback in
     * the member variable distanceCallback_. It does not call the callback
     * immediately and it does not access hardware.
     *
     * So this is a simple public-API safety test.
     */
    try
    {
        ToFSensor sensor(testBus, 0x29, "/dev/gpiochip0", 4, true);

        sensor.registerCallback(
            [](uint16_t distanceMm)
            {
                /**
                 * This is just a dummy callback body.
                 *
                 * We cast the parameter to void so the compiler does not warn
                 * that it is unused.
                 */
                (void)distanceMm;
            });

        check(true, "registerCallback() is safe in test mode");
    }
    catch (...)
    {
        check(false, "registerCallback() is safe in test mode");
    }

    /**
     * Test 3:
     * Check that initialise() returns true in test mode.
     *
     * This matches your actual implementation exactly:
     *
     *     if (isTestMode) {
     *         return true;
     *     }
     *
     * So in test mode, initialise() should always succeed immediately without
     * touching the real sensor, the ST API, or the I2C bus.
     */
    try
    {
        ToFSensor sensor(testBus, 0x29, "/dev/gpiochip0", 4, true);
        bool ok = sensor.initialise();

        check(ok == true, "initialise() returns true in test mode");
    }
    catch (...)
    {
        check(false, "initialise() returns true in test mode");
    }

    /**
     * Test 4:
     * Check that start() is safe in test mode.
     *
     * In your implementation, start() immediately returns if isTestMode is true.
     * That means it should:
     * - not configure GPIO
     * - not start ranging
     * - not launch the worker thread
     *
     * This test simply confirms that calling start() in test mode does not
     * crash or throw.
     */
    try
    {
        ToFSensor sensor(testBus, 0x29, "/dev/gpiochip0", 4, true);
        sensor.start();

        check(true, "start() is safe in test mode");
    }
    catch (...)
    {
        check(false, "start() is safe in test mode");
    }

    /**
     * Test 5:
     * Check that stop() is safe in test mode.
     *
     * In your implementation, stop() also immediately returns if isTestMode is
     * true. So this should always be safe, even if start() was never called.
     */
    try
    {
        ToFSensor sensor(testBus, 0x29, "/dev/gpiochip0", 4, true);
        sensor.stop();

        check(true, "stop() is safe in test mode even when never started");
    }
    catch (...)
    {
        check(false, "stop() is safe in test mode even when never started");
    }

    /**
     * Test 6:
     * Check the common lifecycle sequence:
     * - construct
     * - register callback
     * - initialise
     * - start
     * - stop
     *
     * This is useful because it exercises the public methods in the order your
     * real application would normally use them, while still staying safely in
     * test mode.
     */
    try
    {
        ToFSensor sensor(testBus, 0x29, "/dev/gpiochip0", 4, true);

        sensor.registerCallback(
            [](uint16_t distanceMm)
            {
                (void)distanceMm;
            });

        bool ok = sensor.initialise();
        sensor.start();
        sensor.stop();

        check(ok == true, "full public lifecycle is safe in test mode");
    }
    catch (...)
    {
        check(false, "full public lifecycle is safe in test mode");
    }

    /**
     * Test 7:
     * Check that destruction is safe after normal test-mode use.
     *
     * The destructor calls stop(). In test mode, stop() immediately returns, so
     * destruction should be clean and safe.
     *
     * We create the object inside a nested scope so it is destroyed as soon as
     * that block ends.
     */
    try
    {
        {
            ToFSensor sensor(testBus, 0x29, "/dev/gpiochip0", 4, true);
            sensor.registerCallback(
                [](uint16_t distanceMm)
                {
                    (void)distanceMm;
                });

            sensor.initialise();
            sensor.start();
            sensor.stop();
        }

        check(true, "destructor is safe after test-mode use");
    }
    catch (...)
    {
        check(false, "destructor is safe after test-mode use");
    }

    /**
     * Print a simple summary at the end so the output is easy to read when
     * running the executable directly or through CTest with output enabled.
     */
    std::cout << "\n-----------------------------\n";
    std::cout << "Tests run:    " << testsRun << '\n';
    std::cout << "Tests failed: " << testsFailed << '\n';
    std::cout << "-----------------------------\n";

    /**
     * Return code for CTest.
     *
     * Returning 0 means success.
     * Returning 1 means at least one test failed.
     */
    if (testsFailed == 0)
    {
        std::cout << "All ToFSensor tests passed.\n";
        return 0;
    }

    std::cout << "Some ToFSensor tests failed.\n";
    return 1;
}