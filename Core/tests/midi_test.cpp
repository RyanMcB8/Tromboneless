
#include "main.hpp"
#include <condition_variable>
#include <cstdint>
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
//#include <rtmidi>
#include <eventHandler.hpp>
#include "MidiCoordinator.hpp"
#include "tromboneSynth.hpp"

int main(){
try
    {
        std::queue<RawInputEvent> eventQueue;
        std::mutex eventQueueMutex;
        std::condition_variable eventQueueCv;

        EventHandler eventHandler(eventQueue, eventQueueMutex, eventQueueCv);

        MidiCoordinator coordinator;
        RtMidiSink midiSink;
        GetDistance distanceGetter;

        if (!eventHandler.initialise()) {
            std::cerr << "Initialisation failed\n";
            return 1;
        }

        coordinator.RegisterCallback(
            [&](const MidiMessage& msg)
            {
                std::cout << "Sending MIDI message of size " << msg.size() << "\n";
                midiSink.send(msg);
            });

        coordinator.setExpr(100);
        coordinator.setBend(0);
        coordinator.ChangeNote(60);
        coordinator.PressureEdge(true);

        eventHandler.start();

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
                break;

            case RawInputEvent::Type::MouthpieceReading:
                break;
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}