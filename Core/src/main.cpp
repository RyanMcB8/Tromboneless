#include "main.hpp"
#include <atomic>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>

#include "drivers/i2c_bus.hpp"
#include "drivers/tof_sensor.hpp"

class GetDistance {
public:
    int hasTOFsample(uint16_t distance) {
        if (distance > 500) distance = 500;
        int scaled_bend = 8192 - distance * (8192 / 500);
        std::cout << scaled_bend << "/n";
        return scaled_bend;
    }
};

int main()
{
    try
    {
        I2CBus bus("/dev/i2c-1");
        ToFSensor sensor(bus, 0x29, "/dev/gpiochip0", 4);

        MidiCoordinator coordinator;
        RtMidiSink midiSink;
        GetDistance distanceGetter;

        if (!sensor.initialise()) {
            std::cerr << "Initialisation failed\n";
            return 1;
        }

        coordinator.RegisterCallback(
            [&](const MidiMessage& msg)
            {
                std::cout << "Sending MIDI message of size " << msg.size() << "\n";
                midiSink.send(msg);
            });

        std::atomic<int> got_bend{0};

        sensor.registerCallback([&](uint16_t distance)
        {
            got_bend.store(distanceGetter.hasTOFsample(distance), std::memory_order_relaxed);
        });

        coordinator.setExpr(100);
        coordinator.setBend(0);
        coordinator.ChangeNote(60);
        coordinator.PressureEdge(true);

        sensor.start();

        while (true) {
            coordinator.setBend(got_bend.load(std::memory_order_relaxed));
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}