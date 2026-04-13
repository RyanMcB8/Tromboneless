#pragma once

#include <cstdint>
#include <condition_variable>
#include <mutex>
#include <queue>

#include "drivers/i2c_bus.hpp"
#include "drivers/ads1115rpi.h"
#include "drivers/tof_sensor.hpp"
#include "drivers/cap_sensor.hpp"

struct RawInputEvent {
    enum class Type {
        ToFDistance,
        PressureReading,
        MouthpieceReading,
        Keycontrol
    };

    Type type;
    uint16_t tofDistance;
    float pressureReading;
    int8_t mouthpieceReading;
    char keycontrol;
};

class EventHandler {
public:
    using EventQueue = std::queue<RawInputEvent>;

    /** @brief Constructor sets up event queuing, conditional blocking,
     * initialises drivers for inputs.
    */
    EventHandler(EventQueue& queue,
                 std::mutex& queueMutex,
                 std::condition_variable& queueCv);

    bool initialise();
    void start();

private:
    void handleToFDistance(uint16_t distance);

    void handleKeyControl(char key);

    void handleEmbouchure(int8_t embouchure);
    
    /* Takes pressure value as a float */
    void handlePressureReading(float pressure);

    EventQueue& eventQueue;
    std::mutex& eventQueueMutex;
    std::condition_variable& eventQueueCv;

    I2CBus bus;
    ToFSensor tofSensor;
    ADS1115rpi ads1115rpi;
    CAP1188 cap1188;

};