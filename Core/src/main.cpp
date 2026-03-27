#include "main.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include "drivers/i2c_bus.hpp"
#include "drivers/tof_sensor.hpp"

class getDistance{
    public:
        int hasTOFsample(uint16_t distance){
            // map to 0-812
            if(distance<20) distance = 0;
            if(distance>500) distance = 500;
            int scaled_bend = distance*(8192/500);
            return scaled_bend;
        }
};

int main()
{
    try
    {   
        // opening I2c bus
        I2CBus bus("/dev/i2c-1");

        // GPIO line must match wiring of VL53L1X interrupt pin
        ToFSensor sensor(bus, 0x29, "/dev/gpiochip0", 4);

        // Instantiating midi things
        MidiCoordinator coordinator;
        RtMidiSink midiSink;
        getDistance distancegetter;
        
        // check lidar sensor has initialised properly
        if (!sensor.initialise())
    {
        std::cerr << "Initialisation failed\n";
        return 1;
    }
        
        // midi callback
        coordinator.RegisterCallback(
            [&](const MidiMessage& msg)
            {
                std::cout << "Sending MIDI message of size " << msg.size() << "\n";
                midiSink.send(msg);
            });

        int got_bend = 0;
        
        // lidar callback
        sensor.registerCallback([&](uint16_t distance)
            {
                got_bend = distancegetter.hasTOFsample(distance);
            });

        // Set up some initial values
        coordinator.setExpr(100);   // CC11 expression
        coordinator.setBend(0);  // centre pitch bend, if your builder expects 14-bit style
        coordinator.ChangeNote(60); // middle C

        // Start note
        coordinator.PressureEdge(true);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // Change note while playing
        coordinator.ChangeNote(64);
        
        while(true){
        coordinator.setBend(got_bend);
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

    return 0;
}


/*     // Start blocking GPIO + sensor
    sensor.start();

    // Main thread idle (system is event-driven now)
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
 */
