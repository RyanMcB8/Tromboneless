
#include "main.hpp"
#include <eventHandler.hpp>

#include <condition_variable>
#include <cstdint>
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>

#include "MidiCoordinator.hpp"
#include "tromboneSynth.hpp"
#include "USBMidi.hpp"

class GetDistance {
public:
    int hasTOFsample(uint16_t distance) {
        int mm_limit = 500;
        if (distance > mm_limit) distance = mm_limit;
        int scaled_bend = 8192 - (distance * 8192) / mm_limit;
        
        //std::cout << scaled_bend << "\n";
        return scaled_bend;
    }
};

class MapEmbouchure {
    public:
    int noteMapping(int8_t delta) {

        int strength;
        if (delta >= 80)
            strength = 62; //D4
        else if (delta >= 70)
            strength = 58; //Bb3
        else if (delta >= 60)
            strength = 53; //F3 = 53
        else if (delta >= 50)
            strength = 46; // Bb2 = 46
        else
            strength = 34; // Bb1 = 34

    std::cout << strength << std::endl;
    
    return strength;
    }
};

char key;

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
        MapEmbouchure mapembouchure;

        if (!eventHandler.initialise()) {
            std::cerr << "Initialisation failed\n";
            return 1;
        }

        coordinator.RegisterCallback(
            [&](const MidiMessage& msg)
            {
                //std::cout << "Sending MIDI message of size " << msg.size() << "\n";
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
                //std::cout << event.pressureReading << std::endl;
                if(event.pressureReading > 0.0029f)
                {
                    coordinator.PressureEdge(true);
                }
                else{
                    coordinator.PressureEdge(false);
                }
                break;

            case RawInputEvent::Type::MouthpieceReading:
                coordinator.ChangeNote(mapembouchure.noteMapping(event.mouthpieceReading));
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