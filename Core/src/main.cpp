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
        for (int i = 8000; i > 0; i-=200) 
        {
            coordinator.setBend(i);
            coordinator.setExpr(i);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
