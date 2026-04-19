#include "eventHandler.hpp"
#include <iostream>

EventHandler::EventHandler(bool isTest)
    : bus(isTest ? I2CBus() : I2CBus("/dev/i2c-1")),
      tofSensor(bus, 0x29, "/dev/gpiochip0", 4, isTest),
      cap1188(bus, 0x28, "/dev/gpiochip0", 27, 22, 0x01, 0x01, isTest)
{
    std::cout << "IsTestMode Event " << isTest << "\n";
}

bool EventHandler::initialise() {
    if (!tofSensor.initialise()) {
        std::cout << "TOF Initialise failed.\n";
        return false;
    }

    if (!cap1188.initialise()) {
        std::cout << "cap1188 Initialise failed.\n";
        return false;
    }

    tofSensor.registerCallback(
        [&](uint16_t distance)
        {
            handleToFDistance(distance);
        });

    pressureSensor.registerCallback(
        [&](float pressure)
        {
            handlePressureReading(pressure);
        });

    cap1188.registerCallback(
        [&](int8_t embouchure)
        {
            handleEmbouchure(embouchure);
        });

    return true;
}

void EventHandler::start() {
    cap1188.recalibratePins(0x01);

    ADS1115settings s;
    s.samplingRate = ADS1115settings::FS128HZ;

    pressureSensor.start(s);
    tofSensor.start();
    cap1188.start();
}

RawInputEvent EventHandler::waitForEvent() {
    std::unique_lock<std::mutex> lock(eventQueueMutex);

    eventQueueCv.wait(lock, [&] {
        return !eventQueue.empty();
    });

    RawInputEvent event = eventQueue.front();
    eventQueue.pop();

    return event;
}

void EventHandler::pushEvent(const RawInputEvent& event) {
    {
        std::lock_guard<std::mutex> lock(eventQueueMutex);
        eventQueue.push(event);
    }
    eventQueueCv.notify_one();
}

void EventHandler::handleToFDistance(uint16_t distance) {
    RawInputEvent event{};
    event.type = RawInputEvent::Type::ToFDistance;
    event.tofDistance = distance;

    pushEvent(event);
}

void EventHandler::handleKeyControl(char key) {
    RawInputEvent event{};
    event.type = RawInputEvent::Type::Keycontrol;
    event.keycontrol = key;

    pushEvent(event);
}

void EventHandler::handlePressureReading(float pressure) {
    RawInputEvent event{};
    event.type = RawInputEvent::Type::PressureReading;
    event.pressureReading = pressure;

    pushEvent(event);
}

void EventHandler::handleEmbouchure(int8_t embouchure) {
    RawInputEvent event{};
    event.type = RawInputEvent::Type::MouthpieceReading;
    event.mouthpieceReading = embouchure;

    pushEvent(event);
}