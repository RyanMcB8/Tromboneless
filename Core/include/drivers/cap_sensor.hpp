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

class CAP1188Channel{
    public:
        bool isPinTouched();
        int8_t pinDelta();
        int getThreshold();
        void setThreshold(int threshold);
        void pinRecalibrate();
    private:
        friend class CAP1188;
        CAP1188Channel();
        ~CAP1188Channel();
        void initialise(CAP1188* cap1188, int pin);
        CAP1188* cap1188_;
        int pin_;
};

class CAP1188
{
    public:
        using CAP1188CallbackInterface = std::function<void(uint8_t)>;

        // Class constructor, includes setting
        CAP1188(I2CBus& bus,
                uint8_t address                 = 0x28,
                const std::string& gpioChipPath = "/dev/gpiochip0",
                unsigned int gpioLine = 27,
                unsigned int gpioReset = 22,
                uint8_t enabledChannels = 0x01,
                uint8_t interruptEnabledChannels = 0x01);
        
        // Destructor
        ~CAP1188();

        void start();
        void stop();

        // Initialise sensor
        bool initialise();
        
        // Sensitivity Getter & Setter
        uint8_t sensitivityGetter();
        void sensitivitySetter(uint8_t sensitivityVal);

        uint8_t getAveraging();
        void setAveraging(uint8_t averageingVal);

        std::string getSampleTime();
        void setSampleTime(std::string newSampleTime);

        std::string getCycleTime();
        void setCycleTime(std::string newCycleTime);

        std::array<bool, 8> touched_pins();
        int touched();
        int8_t deltaCount(int pin);
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












