#include <iostream>
#include <thread>
#include <chrono>
#include "MidiCoordinator.hpp"
#include "USBMidi.hpp"


int main() {
    try {
        MidiCoordinator coordinator;
        RtMidiSink midiSink;

        coordinator.RegisterCallback(
            [&](const MidiMessage& msg) {
                midiSink.send(msg);
            });

        coordinator.setExpr(100);
        coordinator.setBend(0);
        coordinator.ChangeNote(60);      // Middle C
        coordinator.PressureEdge(true);  // Note on

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        // Sweep down from centre to minimum
        for (int bend = 8192; bend >= 0; bend -= 128) {
            coordinator.setBend(bend);
            std::cout << "Bend: " << bend << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        // Sweep up from minimum to maximum
        for (int bend = 0; bend <= 16383; bend += 128) {
            coordinator.setBend(bend);
            std::cout << "Bend: " << bend << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        // Sweep back to centre
        for (int bend = 16383; bend >= 8192; bend -= 128) {
            coordinator.setBend(bend);
            std::cout << "Bend: " << bend << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(500));

        coordinator.PressureEdge(false); // Note off
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}