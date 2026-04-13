#include "eventHandler.hpp"

EventHandler::EventHandler(EventQueue& queue,
            std::mutex& queueMutex,
            std::condition_variable& queueCv)
      : eventQueue(queue),
        eventQueueMutex(queueMutex),
        eventQueueCv(queueCv),
        bus("/dev/i2c-1"),
        tofSensor(bus, 0x29, "/dev/gpiochip0", 4)
{
}

bool EventHandler::initialise(){
        if(!tofSensor.initialise()) {
            return false;
        }

        tofSensor.registerCallback(
            [&](uint16_t distance)
            {
                handleToFDistance(distance);
            });

        return true;
}

void EventHandler::start(){
    tofSensor.start();
}

void EventHandler::handleToFDistance(uint16_t distance) {
    RawInputEvent event{};
    event.type = RawInputEvent::Type::ToFDistance;
    event.tofDistance = distance;

    {
        std::lock_guard<std::mutex> lock(eventQueueMutex);
        eventQueue.push(event);
    }

    eventQueueCv.notify_one();
}

void EventHandler::handleKeyControl(char key){
    RawInputEvent event{};
    event.type = RawInputEvent::Type::Keycontrol;
    event.keycontrol = key;
}

void EventHandler::handlePressureReading(float pressure){
    RawInputEvent event{};
    event.type = RawInputEvent::Type::PressureReading;
    event.pressureReading = pressure;
    
    {
    std::lock_guard<std::mutex> lock(eventQueueMutex);
    eventQueue.push(event);
    }

    eventQueueCv.notify_one();
};