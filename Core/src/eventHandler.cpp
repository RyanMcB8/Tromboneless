#include "eventHandler.hpp"

EventHandler::EventHandler(EventQueue& queue,
            std::mutex& queueMutex,
            std::condition_variable& queueCv)
      : eventQueue(queue),
        eventQueueMutex(queueMutex),
        eventQueueCv(queueCv),
        bus("/dev/i2c-1"),
        tofSensor(bus, 0x29, "/dev/gpiochip0", 4),
        cap1188(bus, 0x28, "/dev/gpiochip0", 27, 22, 0x01, 0x01)
{
}

bool EventHandler::initialise(){
        if(!tofSensor.initialise()) {
            return false;
        }

        if(!cap1188.initialise()) {
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

void EventHandler::start(){
    cap1188.recalibratePins(0x01);      
    ADS1115settings s;
    s.samplingRate = ADS1115settings::FS128HZ;
    pressureSensor.start(s);
    tofSensor.start();
    cap1188.start();
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

void EventHandler::handleEmbouchure(int8_t embouchure){
    //std::cout << embouchure << "\n";
   
    RawInputEvent event{};
    event.type = RawInputEvent::Type::MouthpieceReading;
    event.mouthpieceReading = embouchure;
    
    {
    std::lock_guard<std::mutex> lock(eventQueueMutex);
    eventQueue.push(event);
    }

    eventQueueCv.notify_one();
};