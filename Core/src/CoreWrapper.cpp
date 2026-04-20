/** @file           CoreWrapper.cpp
 *  @author         Ryan McBride    Aidan McIntosh
 *  @brief          A file which encapsulations the main funcationality to
 *                  allow for the app to act independently of it.
 */

#include "CoreWrapper.hpp"
#include <stdexcept>

CoreWrapper::CoreWrapper(bool isTest) : eventHandler(isTest),
      midiSink(isTest),
      render(isTest), 
      coordinator(render, isTest), 
      isTestMode(isTest)
{
    std::cout << "CoreWrapper IsTestMode: " << isTestMode << "\n";
    if (isTest)
    {
        externalDevicePresent = false;
        return;
    }
}

CoreWrapper::~CoreWrapper(){
    stop();
}

void CoreWrapper::start()
{
    /*  Stoppinng the rest of the function from being run to initialise hardware. */
    if (isTestMode){
        return;
    }
    externalDevicePresent = midiSink.GetDeviceStatus();
    coordinator.setDevice(externalDevicePresent);

    if (!eventHandler.initialise())
    {
        throw std::runtime_error("EventHandler initialisation failed");
    }

    coordinator.RegisterCallback(
        [&](const MidiMessage& msg)
        {
            midiSink.send(msg);
        });

    eventHandler.start();

    if (!externalDevicePresent)
    {
        render.start();

        render.setDebugTone(true);
        std::this_thread::sleep_for(std::chrono::seconds(2));
        render.setDebugTone(false);
    }

    running = true;
    eventThread = std::thread(&CoreWrapper::eventLoop, this);
}

void CoreWrapper::stop(){
    running = false;

    if (eventThread.joinable()){
        eventThread.join();
    }
}

void CoreWrapper::eventLoop(){
    while (running) {
        RawInputEvent event = eventHandler.waitForEvent();

        switch (event.type) {
            case RawInputEvent::Type::ToFDistance:
                coordinator.setBend(pitchmapper.tof_to_MIDI_bend(event.tofDistance));
                break;

            case RawInputEvent::Type::PressureReading:
                coordinator.PressureEdge(amplitudemapper.noteEdge(event.pressureReading));
                break;

            case RawInputEvent::Type::MouthpieceReading: {
                int new_note = pitchmapper.mouthpiece_to_MIDI_note(event.mouthpieceReading);
                if (new_note != 0) {
                    coordinator.ChangeNote(new_note);
                }
                break;
            }

            case RawInputEvent::Type::Keycontrol:
                break;
        }
    }
}

EventHandler* CoreWrapper::getEventHandler(void){
    return &eventHandler;
}

RtMidiSink* CoreWrapper::getRtMidiSink(void){
    return &midiSink;
}

AudioRender* CoreWrapper::getAudioRender(void){
    return &render;
}

AmplitudeMapper* CoreWrapper::getAmplitudeMapper(void){
    return &amplitudemapper;
}

PitchMapper* CoreWrapper::getPitchMapper(void){
    return &pitchmapper;
}

MidiCoordinator* CoreWrapper::getMidiCoordinator(void){
    return &coordinator;
}