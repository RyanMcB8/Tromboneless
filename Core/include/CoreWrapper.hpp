/** @file           CoreWrapper.hpp
 *  @author         Ryan McBride    Aidan McIntosh
 *  @brief          A file to allow for the app to view methods attached
 *                  to the main functioanlity of the core.
 */

#pragma once

#include "eventHandler.hpp"
#include <cstdint>
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>

#include "MidiCoordinator.hpp"
#include "tromboneSynth.hpp"
#include "USBMidi.hpp"
#include "audioRender.hpp"
#include "PitchMapper.hpp"
#include "AmplitudeMapper.hpp"

class CoreWrapper{
    public:
        /*  ==========================================  */
        /*  Adding the constructor and destructor.      */
        /*  ==========================================  */

        CoreWrapper(bool isTest);
        ~CoreWrapper();

        /*  ==========================================  */
        /*  Adding functions to start the threads.      */
        /*  ==========================================  */
        void start();
        void stop();
        

        EventHandler* getEventHandler(void);
        RtMidiSink* getRtMidiSink(void);
        AudioRender* getAudioRender(void);
        AmplitudeMapper* getAmplitudeMapper(void);
        PitchMapper* getPitchMapper(void);
        MidiCoordinator* getMidiCoordinator(void);

        /*  ==========================================  */
        /* Adding the getters for the classes' instances */
        /*  ==========================================  */

    private:
        /*  Creating the instances. */
        
        EventHandler eventHandler;
        RtMidiSink midiSink;
        AudioRender render;
        AmplitudeMapper amplitudemapper;
        PitchMapper pitchmapper;
        MidiCoordinator coordinator;
        /*  A flag stored to inform the methods if it should ignore
            all hardware dependent functions. */
        bool isTestMode = false;

        /*  Core loop. */
        std::thread eventThread;
        std::atomic<bool> running{false};
        void eventLoop();
        bool externalDevicePresent = false;
};