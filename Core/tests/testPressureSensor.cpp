/**
 * @file test_pressure_sensor.cpp
 * @brief Simple plain-C++ tests for the PressureSensor class.
 *
 * @details
 * This file is intended to be built as a normal executable and run by CTest.
 * It does not use GoogleTest.
 *
 * The PressureSensor class is a lightweight wrapper around the ADS1115rpi
 * library. Since the class directly owns a real ADS1115rpi object and we are
 * not refactoring the design for mocks, these tests focus on what can be
 * checked safely without real hardware.
 *
 * What these tests check:
 * - construction is safe
 * - the default ADS1115 settings are what we expect
 * - callback registration is safe
 * - getADC() returns a usable reference
 * - stop() is safe
 * - destruction is safe
 *
 * What these tests do NOT prove:
 * - the ADS1115 hardware is actually connected and working
 * - the ADC callback fires with live data
 * - start() communicates correctly with the hardware
 *
 * Those things should be covered by integration testing on the Raspberry Pi.
 */

#include "drivers/pressure_sensor.hpp"

#include <exception>
#include <iostream>

/**
 * @brief Count how many tests have been run.
 */
static int testsRun = 0;

/**
 * @brief Count how many tests have failed.
 */
static int testsFailed = 0;

/**
 * @brief Helper function to print pass/fail results.
 *
 * @param condition True if the test passed.
 * @param testName Natural-language description of the test.
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
 * @brief Main entry point for the test executable.
 *
 * @return 0 if all tests pass, 1 otherwise.
 *
 * CTest treats a return value of 0 as success.
 */
int main()
{
    std::cout << "Running PressureSensor tests...\n\n";

    /**
     * Test 1:
     * Check that a PressureSensor object can be constructed safely.
     *
     * This is a very basic sanity check. The constructor is defaulted, so this
     * should succeed unless there is some deeper issue in the wrapped ADC class.
     */
    try
    {
        PressureSensor sensor;
        check(true, "constructor is safe");
    }
    catch (...)
    {
        check(false, "constructor is safe");
    }

    /**
     * Test 2:
     * Check that getDefaultSettings() returns the expected sampling rate.
     *
     * This verifies one of the design choices encoded in the class:
     * default sampling should be 128 Hz.
     */
    try
    {
        ADS1115settings settings = PressureSensor::getDefaultSettings();

        check(settings.samplingRate == ADS1115settings::FS128HZ,
              "default settings use 128 Hz sampling rate");
    }
    catch (...)
    {
        check(false, "default settings use 128 Hz sampling rate");
    }

    /**
     * Test 3:
     * Check that getDefaultSettings() returns the expected channel.
     *
     * The class is currently configured to read AIN0 by default.
     */
    try
    {
        ADS1115settings settings = PressureSensor::getDefaultSettings();

        check(settings.channel == ADS1115settings::AIN0,
              "default settings use AIN0 channel");
    }
    catch (...)
    {
        check(false, "default settings use AIN0 channel");
    }

    /**
     * Test 4:
     * Check that getDefaultSettings() returns the expected gain.
     *
     * The class currently uses the +/-0.512 V full-scale range by default.
     */
    try
    {
        ADS1115settings settings = PressureSensor::getDefaultSettings();

        check(settings.pgaGain == ADS1115settings::FSR0_512,
              "default settings use FSR0_512 gain");
    }
    catch (...)
    {
        check(false, "default settings use FSR0_512 gain");
    }

    /**
     * Test 5:
     * Check that registerCallback() is safe.
     *
     * This does not prove that the callback will be called by live ADC data.
     * It only checks that storing and forwarding the callback does not throw
     * or otherwise fail immediately.
     */
    try
    {
        PressureSensor sensor;

        sensor.registerCallback(
            [](float pressureValue)
            {
                /**
                 * Dummy callback body.
                 * We do nothing here because this test only checks that the
                 * callback can be registered safely.
                 */
                (void)pressureValue;
            });

        check(true, "registerCallback() is safe");
    }
    catch (...)
    {
        check(false, "registerCallback() is safe");
    }

    /**
     * Test 6:
     * Check that getADC() returns a usable reference.
     *
     * We cannot deeply inspect the ADC object without depending on the external
     * ADS1115rpi API, but we can at least verify that taking a reference is safe.
     */
    try
    {
        PressureSensor sensor;
        ADS1115rpi& adcRef = sensor.getADC();

        /**
         * We do not need to do anything with adcRef here.
         * The fact that we obtained a reference safely is enough for this test.
         */
        (void)adcRef;

        check(true, "getADC() returns a valid reference safely");
    }
    catch (...)
    {
        check(false, "getADC() returns a valid reference safely");
    }

    /**
     * Test 7:
     * Check that stop() is safe even if start() was never called.
     *
     * This is a useful lifecycle safety test. Well-behaved resource wrappers
     * should allow stop() to be called harmlessly even if the device was not
     * started first.
     */
    try
    {
        PressureSensor sensor;
        sensor.stop();

        check(true, "stop() is safe when sensor was never started");
    }
    catch (...)
    {
        check(false, "stop() is safe when sensor was never started");
    }

    /**
     * Test 8:
     * Check that the common setup path is safe:
     * - construct object
     * - get default settings
     * - register callback
     *
     * We deliberately do not call start() here because that may attempt to
     * access real hardware depending on the ADS1115rpi implementation.
     */
    try
    {
        PressureSensor sensor;
        ADS1115settings settings = PressureSensor::getDefaultSettings();

        sensor.registerCallback(
            [](float pressureValue)
            {
                (void)pressureValue;
            });

        /**
         * Cast settings to void because this test is only checking that the
         * sequence is safe, not that start() works on real hardware.
         */
        (void)settings;

        check(true, "basic setup sequence is safe");
    }
    catch (...)
    {
        check(false, "basic setup sequence is safe");
    }

    /**
     * Test 9:
     * Check that destruction is safe after normal use.
     *
     * The destructor calls stop(), so this is another useful lifecycle test.
     * We create the object in a nested scope so that destruction happens
     * immediately when the block ends.
     */
    try
    {
        {
            PressureSensor sensor;
            sensor.registerCallback(
                [](float pressureValue)
                {
                    (void)pressureValue;
                });

            ADS1115settings settings = PressureSensor::getDefaultSettings();
            (void)settings;
        }

        check(true, "destructor is safe after normal use");
    }
    catch (...)
    {
        check(false, "destructor is safe after normal use");
    }

    /**
     * Print final summary.
     */
    std::cout << "\n-----------------------------\n";
    std::cout << "Tests run:    " << testsRun << '\n';
    std::cout << "Tests failed: " << testsFailed << '\n';
    std::cout << "-----------------------------\n";

    if (testsFailed == 0)
    {
        std::cout << "All PressureSensor tests passed.\n";
        return 0;
    }

    std::cout << "Some PressureSensor tests failed.\n";
    return 1;
}