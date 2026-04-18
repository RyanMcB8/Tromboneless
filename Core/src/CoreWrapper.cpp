/** @file           CoreWrapper.cpp
 *  @author         Ryan McBride    Aidan McIntosh
 *  @brief          A file which encapsulations the main funcationality to
 *                  allow for the app to act independently of it.
 */

#include "CoreWrapper.hpp"

CoreWrapper::CoreWrapper() : eventHandler(eventQueue, eventQueueMutex, eventQueueCv), coordinator(render){
        const bool externalDevicePresent = midiSink.GetDeviceStatus();
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

}

void CoreWrapper::stop(){

}



void CoreWrapper::eventLoop(){

}

