#include "main.hpp"
#include <chrono>
#include <thread>
#include <iostream>

int main()
{
    try
    {
        MidiCoordinator coordinator;
        RtMidiSink midiSink;

        coordinator.RegisterCallback(
            [&](const MidiMessage& msg)
            {
                std::cout << "Sending MIDI message of size " << msg.size() << "\n";
                midiSink.send(msg);
            }
        );

        // Set up some initial values
        coordinator.setExpr(100);   // CC11 expression
        coordinator.setBend(0);  // centre pitch bend, if your builder expects 14-bit style
        coordinator.ChangeNote(60); // middle C

        // Start note
        coordinator.PressureEdge(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // Change note while playing
        coordinator.ChangeNote(64);
        
        // Loop to test pitch bend
        for (int i = 8000; i > 0; i-=1) 
        {
            coordinator.setBend(i);
            coordinator.setExpr(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        // Stop note
        coordinator.PressureEdge(false);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

#include <iostream>
#include <thread>
#include <chrono>

#include "drivers/i2c_bus.hpp"
#include "drivers/tof_sensor.hpp"

/*
 * Subscriber class
 * Receives distance from sensor
 */
class ToFPrinter
{
public:
    void hasToFSample(uint16_t distance)
    {
        std::cout << "Distance: " << distance << " mm\n";
    }
};

int main()
{
    I2CBus bus("/dev/i2c-1");

    // GPIO line must match wiring of VL53L1X interrupt pin
    ToFSensor sensor(bus, 0x29, "/dev/gpiochip0", 4);

    ToFPrinter printer;

    if (!sensor.initialise())
    {
        std::cerr << "Initialisation failed\n";
        return 1;
    }

    // Connect publisher → subscriber via lambda
    sensor.registerCallback([&](uint16_t distance)
    {
        printer.hasToFSample(distance);
    });

    // Start blocking GPIO + sensor
    sensor.start();

    // Main thread idle (system is event-driven now)
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}