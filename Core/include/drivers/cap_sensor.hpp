/**
 * @file cap_sensor.hpp
 * @author Ben Allen (you@domain.com)
 * @brief                   desc
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

        
        CAP1188(I2CBus& bus,
                uint8_t address                 = 0x28,
                const std::string& gpioChipPath = "/dev/gpiochip0",
                unsigned int gpioLine = 27,
                unsigned int gpioReset = 22,
                uint8_t enabledChannels = 0x01,
                uint8_t interruptEnabledChannels = 0x01);
        
        // Destructor
        ~CAP1188();
        /**
         * @brief                  Start
         * 
         */
        void start();
        /**
         * @brief                   desc
         * 
         */
        void stop();
        /**
         * @brief                   desc
         * 
         * @return  true            Initialisation successful
         * @return  false           Initialisation failed
         */
        bool initialise();
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
         * @brief                   Read the Delta Count Register for a given pin and return the delta value
         *
         * @param   pin             The pin AKA channel number. Must be an integer between 1 & 8.
         * @return  int8_t          The delta count for the given channel as an 8-bit word signed with 2's complement. 
         *                          The count value represents a change in input due to
                                    the capacitance associated with a touch on one of the sensor inputs and is referenced to a calibrated
                                    base “Not Touched” count value.
         */
        int8_t deltaCount(int pin);
        /**
         * @brief                   Force-initiate the baseline re-calibration routine for given channels
         * 
         * @param   pins            desc
         */
        void recalibratePins(uint8_t pins);
        std::array<uint8_t, 8> getThresholds();
        void setThresholds(std::array<uint8_t, 8> newThresholds);

        // REALTIME FUNCTIONALITY
        // Worker thread: blocks on GPIO edge events
        void worker();

        // Called for data = ready
        void handleDataReady();

        void disableStandby();
        void clearInterrupt();


        // Register subscriber callback
        void registerCallback(CAP1188CallbackInterface ci);

        void tester();

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












