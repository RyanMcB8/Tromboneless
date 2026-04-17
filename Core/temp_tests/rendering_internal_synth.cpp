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

class GetDistance {
public:
    int hasTOFsample(uint16_t distance) {
        int mm_limit = 500;
        if (distance > mm_limit) distance = mm_limit;
        int scaled_bend = 8192 - (distance * 8192) / mm_limit;
        return scaled_bend;
    }
};

class MapEmbouchure {
public:
    int noteMapping(int8_t delta) {
        int strength = 34;

        if (delta >= 75)
            strength = 62;
        else if (delta >= 70)
            strength = 58;
        else if (delta >= 60)
            strength = 53;
        else if (delta >= 50)
            strength = 46;
        else if (delta >= 40)
            strength = 34;
        else
            strength = 0;

        return strength;
    }
};

int main() {
    try {
        std::queue<RawInputEvent> eventQueue;
        std::mutex eventQueueMutex;
        std::condition_variable eventQueueCv;

        EventHandler eventHandler(eventQueue, eventQueueMutex, eventQueueCv);
        RtMidiSink midiSink;
        GetDistance distanceGetter;
        MapEmbouchure mapembouchure;
        AudioRender render;

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
                    coordinator.setBend(distanceGetter.hasTOFsample(event.tofDistance));
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
                    new_note = mapembouchure.noteMapping(event.mouthpieceReading);
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