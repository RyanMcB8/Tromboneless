/** @file           CoreWrapper.cpp
 *  @author         Ryan McBride    Aidan McIntosh
 *  @brief          A file which encapsulations the main funcationality to
 *                  allow for the app to act independently of it.
 */

#include "CoreWrapper.hpp"

CoreWrapper::CoreWrapper() : eventHandler(eventQueue, eventQueueMutex, eventQueueCv), coordinator(render){
        externalDevicePresent = midiSink.GetDeviceStatus();
        coordinator.setDevice(externalDevicePresent);

        /* Ensuring that the event handler was initialised properly. */
        if (!eventHandler.initialise()) {
            std::cerr << "Initialisation failed\n";
            throw;
        }

        /*  Registering a callback for midi messages. */
        coordinator.RegisterCallback(
            [&](const MidiMessage& msg) {
                midiSink.send(msg);
            });


}


CoreWrapper::~CoreWrapper(){

}

void CoreWrapper::start(){
    /*  Starting the callback*/
    eventHandler.start();

    /*  Checking whether the external synth or internal synth is being used. */
    if (!externalDevicePresent) {
        render.start();

        /*  Running a test tone to test if the external speakers are connected and audible. */
        render.setDebugTone(true);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        render.setDebugTone(false);

        std::cout << "Internal synth mode\n";
    } 
    else {
        std::cout << "External MIDI mode\n";
    }

    std::cout << "Beginning Pressure Baseline Calculation\n";
    // Baseline collection loop
    while (true) {
        RawInputEvent event;
        {
            std::unique_lock<std::mutex> lock(eventQueueMutex);
            eventQueueCv.wait(lock, [&] { return !eventQueue.empty(); });
            event = eventQueue.front();
            eventQueue.pop();
        }

        // Only feed pressure readings to the baseline
        if (event.type == RawInputEvent::Type::PressureReading) {
            if (amplitudemapper.calculateBaseline(event.pressureReading)) {
                break;
            }
        }
    }
    std::cout << "Pressure Baseline found to be: " << amplitudemapper.getBaseline() << "\n";
}

void CoreWrapper::stop(){
    running = false;
    eventQueueCv.notify_all();
    if (eventThread.joinable()){
        eventThread.join();
    }
}



void CoreWrapper::eventLoop(){
    int current_note = 0;

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
                coordinator.PressureEdge(amplitudemapper.noteEdge(event.pressureReading));
                break;

            case RawInputEvent::Type::MouthpieceReading:
                int new_note = pitchmapper.mouthpiece_to_MIDI_note(event.mouthpieceReading);
                if (current_note != new_note && new_note != 0) {
                    current_note = new_note;
                    coordinator.ChangeNote(current_note);
                }
                break;
        }
    }
}

