#include "main.hpp"
#include "eventHandler.hpp"
#include <condition_variable>
#include <cstdint>
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <chrono>
#include "MidiCoordinator.hpp"
#include "tromboneSynth.hpp"
#include "USBMidi.hpp"
#include "audioRender.hpp"
#include "PitchMapper.hpp"

int main() {
    try {
        std::queue<RawInputEvent> eventQueue;
        std::mutex eventQueueMutex;
        std::condition_variable eventQueueCv;

        EventHandler eventHandler(eventQueue, eventQueueMutex, eventQueueCv);
        RtMidiSink midiSink;
        AudioRender render;
        PitchMapper pitchmapper;
        MidiCoordinator coordinator(render);
        
        const bool externalDevicePresent = midiSink.GetDeviceStatus();
        coordinator.setDevice(externalDevicePresent);

        if (!eventHandler.initialise()) {
            std::cerr << "Initialisation failed\n";
            return 1;
        }

        coordinator.RegisterCallback(
            [&](const MidiMessage& msg) {
                midiSink.send(msg);
            });

        eventHandler.start();

        if (!externalDevicePresent) {
            render.start();

            render.setDebugTone(true);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            render.setDebugTone(false);

            std::cout << "Internal synth mode\n";
        } else {
            std::cout << "External MIDI mode\n";
        }

        bool pressure_gate = false;
        int current_note = 0;
        int new_note = 0;

        while (true) {
            RawInputEvent event;
            {
                std::unique_lock<std::mutex> lock(eventQueueMutex);
                eventQueueCv.wait(lock, [&] { return !eventQueue.empty(); });
                event = eventQueue.front();
                eventQueue.pop();
            }

            switch (event.type) {
                case RawInputEvent::Type::ToFDistance:
                    coordinator.setBend(pitchmapper.tof_to_MIDI_bend(event.tofDistance));
                    break;

                case RawInputEvent::Type::PressureReading:
                    if (event.pressureReading < 0.22f && !pressure_gate) {
                        coordinator.PressureEdge(true);
                        pressure_gate = true;
                    } else if (event.pressureReading > 0.22f && pressure_gate) {
                        coordinator.PressureEdge(false);
                        pressure_gate = false;
                    }
                    break;

                case RawInputEvent::Type::MouthpieceReading:
                    new_note = pitchmapper.mouthpiece_to_MIDI_note(event.mouthpieceReading);
                    if (current_note != new_note && new_note != 0) {
                        current_note = new_note;
                        coordinator.ChangeNote(current_note);
                    }
                    break;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}