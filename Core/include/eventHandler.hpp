#pragma once

#include <cstdint>
#include <condition_variable>
#include <mutex>
#include <queue>

#include "drivers/i2c_bus.hpp"
#include "drivers/tof_sensor.hpp"

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
    float mouthpieceReading;
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

    void handlePressureReading(float pressure);

    EventQueue& eventQueue;
    std::mutex& eventQueueMutex;
    std::condition_variable& eventQueueCv;

    I2CBus bus;
    ToFSensor tofSensor;
};