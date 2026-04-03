#include "main.hpp"


#include <condition_variable>
#include <cstdint>
#include <iostream>
#include <mutex>
#include <queue>
#include "tromboneSynth.hpp"

class GetDistance {
public:
    int hasTOFsample(uint16_t distance) {
        if (distance > 500) distance = 500;
        int scaled_bend = 8192 - (distance * 8192) / 500;
        std::cout << scaled_bend << "\n";
        return scaled_bend;
    }
};
char key;
TromboneSynth synth;
int main()
{
    std::cout << "Input key for note \n";
    std::cin >> key;
    switch(key){
        case 'a':
            synth.NewTromboneNote(Notes::Notes_t::note_A, 4);
        case "b":
            synth.NewTromboneNote(Notes::Notes_t::note_B, 4);
        case "c":
            synth.NewTromboneNote(Notes::Notes_t::note_C, 4);
        case "d":
            synth.NewTromboneNote(Notes::Notes_t::note_D, 4);
        case "e":
            synth.NewTromboneNote(Notes::Notes_t::note_E, 4);
        case "f":
            synth.NewTromboneNote(Notes::Notes_t::note_F, 4);
            
        default:
            synth.NewTromboneNote(Notes::Notes_t::note_G, 4);
    }
    while(true){
        synth.ReadTromboneAudio();
    }
    // try
    // {
    //     std::queue<RawInputEvent> eventQueue;
    //     std::mutex eventQueueMutex;
    //     std::condition_variable eventQueueCv;

    //     EventHandler eventHandler(eventQueue, eventQueueMutex, eventQueueCv);

    //     MidiCoordinator coordinator;
    //     RtMidiSink midiSink;
    //     GetDistance distanceGetter;

    //     if (!eventHandler.initialise()) {
    //         std::cerr << "Initialisation failed\n";
    //         return 1;
    //     }

    //     coordinator.RegisterCallback(
    //         [&](const MidiMessage& msg)
    //         {
    //             std::cout << "Sending MIDI message of size " << msg.size() << "\n";
    //             midiSink.send(msg);
    //         });

    //     coordinator.setExpr(100);
    //     coordinator.setBend(0);
    //     coordinator.ChangeNote(60);
    //     coordinator.PressureEdge(true);

    //     eventHandler.start();

    //     while (true) {
    //         RawInputEvent event;

    //         {
    //             std::unique_lock<std::mutex> lock(eventQueueMutex);
    //             eventQueueCv.wait(lock, [&] { return !eventQueue.empty(); });

    //             event = eventQueue.front();
    //             eventQueue.pop();
    //         }

    //         switch (event.type) {
    //         case RawInputEvent::Type::ToFDistance:
    //             coordinator.setBend(distanceGetter.hasTOFsample(event.tofDistance));
    //             break;

    //         case RawInputEvent::Type::PressureReading:
    //             break;

    //         case RawInputEvent::Type::MouthpieceReading:
    //             break;
    //         }
    //     }
    // }
    // catch (const std::exception& e)
    // {
    //     std::cerr << "Error: " << e.what() << "\n";
    //     return 1;
    // }

    // return 0;
}
