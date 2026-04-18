#pragma once

#include <cstdint>
#include <condition_variable>
#include <mutex>
#include <queue>

#include "drivers/i2c_bus.hpp"
#include "drivers/pressure_sensor.hpp"
#include "drivers/tof_sensor.hpp"
#include "drivers/cap_sensor.hpp"

struct RawInputEvent {
    enum class Type {
        ToFDistance,
        PressureReading,
        MouthpieceReading,
        Keycontrol
    };

    Type type{};
    uint16_t tofDistance{};
    float pressureReading{};
    int8_t mouthpieceReading{};
    char keycontrol{};
};

class EventHandler {
public:
    using EventQueue = std::queue<RawInputEvent>;

    /** @brief Constructor initialises drivers and internal event transport. */
    EventHandler();

    bool initialise();
    void start();

    /** @brief Block until an event is available, then return it. */
    RawInputEvent waitForEvent();

private:
    void pushEvent(const RawInputEvent& event);

    void handleToFDistance(uint16_t distance);
    void handleKeyControl(char key);
    void handleEmbouchure(int8_t embouchure);
    void handlePressureReading(float pressure);

    EventQueue eventQueue;
    std::mutex eventQueueMutex;
    std::condition_variable eventQueueCv;

    I2CBus bus;
    ToFSensor tofSensor;
    PressureSensor pressureSensor;
    CAP1188 cap1188;
};