/**
 * @file cap_sensor.hpp
 * @author Ben Allen
 * @brief                   Base driver class for the Adafruit CAP1188 sensor breakout board, 
 *                          specific to Raspberry Pi 5 over Linux kernel with I2C communication.
 * @version 0.1
 * @date 2026-04-19
 * 
 * @copyright Copyright (c) 2026
 * 
 */


#pragma once

#include <array>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <linux/i2c-dev.h>
#include <thread>
#include <gpiod.hpp>
#include <functional>
#include <stdexcept>
#include "drivers/i2c_bus.hpp"
#include "drivers/cap1188_registers.hpp"

class CAP1188;
/**
 * @brief                  Helper class unique to one sensor channel
 * @note                   This class is used to call base driver functions specific to
 *                         only one input channel. When the base driver is initialised, a unique channel
 *                         class is created for each of the 8 pins.
 */
class CAP1188Channel{
    public:
        /**
         * @brief                  Function to find whether this channel's pin is touched
         * @return true            The pin is being touched
         * @return false           The pin is not being touched
         * @note                   This function points to the base driver's touched() function, passing only
         *                         this pin to it. The state of being 'touched' is defined as having a delta
         *                         greater than the threshold.
         */
        bool isPinTouched();
        /**
         * @brief                  Function to find the delta value for this channel 
         * @return  int8_t         This channel's delta value
         * @note                   This function points to the base driver's deltaCount() function, passing only
         *                         this pin to it. The delta value is defined as the difference from the current
         *                         capacitance to the baseline capacitance.
         */
        int8_t pinDelta();
        /**
         * @brief                   Getter function for pin's threshold value
         * @return  int             This channel's threshold value
         */
        int getThreshold();
        /**
         * @brief                   Setter function for pin's threshold value
         * @param   threshold       This channel's new threshold value.
         */
        void setThreshold(int threshold);
        /**
         * @brief                   Function to initiate recalibration routine for this channel
         * @note                    This function points to the base driver's recalibratePins function, passing
         *                          only this pin to it.
         */
        void pinRecalibrate();
    private:
        friend class CAP1188;
        CAP1188Channel();
        ~CAP1188Channel();
        void initialise(CAP1188* cap1188, int pin);
        CAP1188* cap1188_;
        int pin_;
};
/**
 * @brief                   Base driver class for the CAP1188
 * @note                    This driver has been created with reference to the CAP1188 datasheet and the CircuitPython
 *                          driver for the Adafruit CAP1188 breakout board. This driver is specific to the Raspberry Pi,
 *                          I2C protocol, and for the express purpose of using a single channel's delta measurement to
 *                          determine a data ready condition, in which case the measurement is passed.
 */
class CAP1188
{
    public:
        using CAP1188CallbackInterface = std::function<void(uint8_t)>;

        /**
         * @brief                   Constructor for the CAP1188 base driver class
         * @param   bus             I2Cbus object for the CAP1188 using i2c_bus
         * @param   address         I2C address for the CAP1188 sensor. With the Adafruit breakout board this defaults to
         *                          0x29. Wiring the '3Vo' pin to the 'AD' pin alters the address to 0x28. This is important
         *                          as both the Time-of-Flight sensor and CAP1188 have the same default address. Default: 0x28.
         * @param   gpioChipPath    Specifies the device file used by the linux kernel. So long as all I2C devices use the same
         *                          chip path this can be any gpiochip. Default: "/dev/gpiochip0"
         * @param   gpioLine        Specifies the GPIO pin on the Raspberry Pi used as the "data ready" pin.
         * @param   enabledChannels Specifies the enabled input channels. Only these channels will be sampled for capacitance values.
         *                          Disabled pins will not be included in the sample cycle. This is formatted as a bit-mask of enabled
         *                          pins from msb:C8 to lsb:C1. The default value is 0x01 = 0b0000'0001 i.e. Only C1.
         * @param   interruptEnabledChannels    Specifies the interrupt enabled channels. Data ready will only be called when one of
         *                                      the specified pins have a touched status. This is formatted as a bit-mask of interrupt
         *                                      enabled pins from msb:C8 to lsb:C1. The default value is 0x01 = 0b0000'0001 i.e. Only C1.
         */
        CAP1188(I2CBus& bus,
                uint8_t address                 = 0x28,
                const std::string& gpioChipPath = "/dev/gpiochip0",
                unsigned int gpioLine = 27,
                uint8_t enabledChannels = 0x01,
                uint8_t interruptEnabledChannels = 0x01,
                bool isTest = false);

        /**
         * @brief                  Destructor for the CAP1188 base driver class. Calls "stop()" function.
         */
        ~CAP1188();
        /**
         * @brief                   Runs initialisation routine for driver
         * @return  true            Initialisation successful
         * @return  false           Initialisation failed
         * @note                    This routine will open the I2C bus for the driver and check that a CAP1188 is found at the
         *                          specified I2C address. Set enabled channels, set interrupt-enabled channels, enable LED-linking
         *                          on enabled channels, set sampling cycle settings, set threshold for data-ready condition, enable
         *                          interrupt on touch release, set the sensitivity, disable standby mode and clear the interrupt.
         *                          After successful initialisation the sensor should be sampling correctly.
         */
        bool initialise();
        /**
         * @brief                   Configures the GPIO line for input and falling edge detection. Creates a thread with worker(). Can now
         *                          have callback registered.
         */
        void start();
        /**
         * @brief                  After thread has finished, releases it and all GPIO resources. This ensures data is released in a safe way.
         */
        void stop();
        /**
         * @brief                   Checks for data ready condition. When data is ready, call the handleDataReady() function.
         */
        void worker();
        /**
         * @brief                   If worker() determines data is ready this function is called. Gets the actual delta value on C1 channel
         *                          and then clears the interrupt so that subsequent interrupts cause an edge event as opposed to
         *                          maintaining a level. Publish delta value to the callback interface.
         */
        void handleDataReady();
        /**
         * @brief                   Register the callback of pin 1 delta values to be published when pin 1 is touched
         * @param   ci              Delta values as 8-bit unsigned integer word. This MUST be converted to an 8-bit
         *                          signed integer (int8_t) later, as this is the true form of delta values.
         */
        void registerCallback(CAP1188CallbackInterface ci);
        /**
         * @brief                   Get the sensitivity value from the sensitivity control register
         * @return  uint8_t         From the CAP1188 datasheet: "Bits 6-4 DELTA_SENSE[2:0] - Controls the sensitivity of a touch detection.
         *                          The sensitivity settings act to scale the relative delta count value higher or lower based on the system
         *                          parameters. A setting of 000b is the most sensitive while a setting of 111b is the least sensitive. At
         *                          the more sensitive settings,
         *                          touches are detected for a smaller delta capacitance corresponding to a “lighter” touch. These settings
         *                          are more sensitive to noise, however, and a noisy environment may flag more false touches with higher
         *                          sensitivity levels"
         */
        
        uint8_t getSensitivity();
        /**
         * @brief                   Set the sensitivity value by writing to the sensitivity control register
         * @param  sensitivityVal   From the CAP1188 datasheet: "Bits 6-4 DELTA_SENSE[2:0] - Controls the sensitivity of a touch detection.
         *                          The sensitivity settings act to scale the relative delta count value higher or lower based on the system
         *                          parameters. A setting of 000b is the most sensitive while a setting of 111b is the least sensitive. At 
         *                          the more sensitive settings,
         *                          touches are detected for a smaller delta capacitance corresponding to a “lighter” touch. These settings
         *                          are more sensitive to noise, however, and a noisy environment may flag more false touches with higher
         *                          sensitivity levels"
         */
        void setSensitivity(uint8_t sensitivityVal);
        /**
         * @brief                   Get the number of samples per average by reading from the Averaging and Sampling Config register
         * @return  uint8_t         From the CAP1188 datasheet: "Bits 6 - 4 - AVG[2:0] - Determines the number of samples that are taken 
         *                          for all active channels during the sensor cycle... All samples are taken 
         *                          consecutively on the same channel before the next channel is sampled and the result is averaged 
         *                          over the number of samples measured before updating the measured results."
         */
        uint8_t getAveraging();
        /**
         * @brief                  Set the sample number per channel average by writing to the Averaging and Sampling Config register
         * @param   averagingVal   The number of samples taken from each enabled channel during the sensor input cycle
         *                         From the CAP1188 datasheet: "Bits 6 - 4 - AVG[2:0] - Determines the number of samples that are taken
         *                         for all active channels during the sensor cycle... All samples are taken
         *                         consecutively on the same channel before the next channel is sampled and the result is averaged
         *                         over the number of samples measured before updating the measured results." Must be 
         */
        void setAveraging(uint8_t averagingVal);
        /**
         * @brief                  Get the sampling time by reading from the Averaging and Sampling Config register
         * @return  std::string    The sampling time determined by the SAMP_TIME[1:0] bits. The sampling time is the period of time
         *                         between each sample.
         */
        std::string getSampleTime();
        /**
         * @brief                   Set the sampling time by writing to the Averaging and Sampling Config register
         * @param   newSampleTime   The sampling time determined by the SAMP_TIME[B3:B2] bits. The sampling time is the period of time
         *                          between each sample. Options: {"320us", "640us", "1.28ms", "2.56ms"}
         */
        void setSampleTime(std::string newSampleTime);
        /**
         * @brief                   Get the cycle time by reading from the Averaging and Sampling Config register
         * @return  std::string     The cycle time determined by the CYCLE_TIME[B1:B0] buts. If the sensor input cycle ends befor the
         *                          cycle time, then the system will wait for the full cycle time before initiating the next sensor
         *                          input cycle. However, the cycle time is ignored if the sensor input cycle exceeds it.
         */
        std::string getCycleTime();
        /**
         * @brief                   Set the cycle time by writing to the Averaging and Sampling Config register
         * @param   newCycleTime    The cycle time determined by the CYCLE_TIME[B1:B0] buts. If the sensor input cycle ends befor the
         *                          cycle time, then the system will wait for the full cycle time before initiating the next sensor
         *                          input cycle. However, the cycle time is ignored if the sensor input cycle exceeds it.
         *                          Options: {"35ms", "70ms", "105ms", "140ms"}
         */
        void setCycleTime(std::string newCycleTime);
        /**
         * @brief                   Read the Sensor Input Status Register to find which pins are being touched
         * @return  int             An 8-bit pinmask of touched pins, from pin CS8:CS1. For example: 0110'0001
         *                          means pins 7, 6, and 1 are touched. The touched status is determined internally by the chip by
         *                          comparing the current delta values with a pre-determined threshold. Only enabled channels can be registered
         *                          as touched.
         */
        int touched();
        /**
         * @brief                   Set threshold for touched condition on one pin.
         * 
         * @param   pin             Pin number as integer from 1 to 8. This is used to source the relevant pin threshold register.
         * @param   threshold       New threshold value for the threshold register to update to. 
         */
        int8_t deltaCount(int pin);
        /**
         * @brief                   Force-initiate the baseline re-calibration routine for given channels
         * @param   pins            8-bit bitmask of channels to be re-calibrated from C8(msb) to C1(lsb).
         */
        void recalibratePins(uint8_t pins);
        /**
         * @brief                           Get the touch thresholds for every pin
         * @return  std::array<uint8_t, 8>  An array of size 8 (from C1 to C8) of every delta value (8-bit signed integer)
         *                                  at which the pin is considered touched
         */
        std::array<uint8_t, 8> getThresholds();
        /**
         * @brief                   Set new touch thresholds for every pin
         * @param   newThresholds   Size 8 array of signed 8-bit integer. From C1 to C8, every 8-bit word contains the delta
         *                          value after which the delta value is considered touching.
         */
        void setThresholds(std::array<uint8_t, 8> newThresholds);
        /**
         * @brief                   Enable onboard LED functionality for given pin
         * @param   pinmask         8-bit bitmask of channels to be linked to LED from C8(msb) to C1(lsb).
         * @note                    LED will switch on if pin is touched
         */
        void linkLEDs(uint8_t pinmask);
        /**
         * @brief                   Sets the interrupt repeat according to a 4-bit word.
         * @param   rate            New interrupt repeat rate. Starts at 35ms = 0b0000 and increments by 35ms for
         *                          every added +1.
         */
        void setInterruptRepeatRate(uint8_t rate);
        /**
         * @brief                   Enable channels according to the enabledChannels setting bitmask
         */
        void enableChannels();
        /**
         * @brief                   Enable interrupt channels according to the interruptEnabledChannels
         *                          settings bitmask.
         */
        void enableInterruptChannels();
        /**
         * @brief                   Disable standby mode
         */
        void disableStandby();
        /**
         * @brief                   Clear the interrupt bit. This is essential to do after data is read, to get
         *                          edge events when the pin is touched, and so that the interrupt bit doesn't
         *                          remain stuck after interrupt.
         */
        void clearInterrupt();

    private:

        I2CBus& bus_;
        uint8_t address_;
        std::string gpioChipPath_;
        unsigned int gpioLine_;
        unsigned int gpioReset_;
        uint8_t enabledChannels_;
        uint8_t interruptEnabledChannels_;
        CAP1188Channel channels[8];

        uint8_t interruptPolarity_ = 0;
        bool running_ = false;

        // Stored callback
        CAP1188CallbackInterface cap1188CallbackInterface_;

        std::shared_ptr<gpiod::chip> chip_;
        std::shared_ptr<gpiod::line_request> request_;
        std::thread thread_;

};












