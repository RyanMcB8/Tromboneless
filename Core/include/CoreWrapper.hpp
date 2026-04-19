/** @file           CoreWrapper.hpp
 *  @author         Ryan McBride    Aidan McIntosh
 *  @brief          A file to allow for the app to view methods attached
 *                  to the main functioanlity of the core.
 */


#pragma once

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
#include "AmplitudeMapper.hpp"

class CoreWrapper{
    public:
        /*  ==========================================  */
        /*  Adding the constructor and destructor.      */
        /*  ==========================================  */

        CoreWrapper();
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
        /*  Setting up queue for the threading. */
        std::queue<RawInputEvent> eventQueue;
        std::mutex eventQueueMutex;
        std::condition_variable eventQueueCv;

        /*  Creating the instances. */
        EventHandler eventHandler = EventHandler(eventQueue, eventQueueMutex, eventQueueCv);
        RtMidiSink midiSink;
        AudioRender render;
        AmplitudeMapper amplitudemapper;
        PitchMapper pitchmapper;
        MidiCoordinator coordinator = MidiCoordinator(render);


        /*  Core loop. */
        std::thread eventThread;
        bool running = false;
        void eventLoop();
        bool externalDevicePresent;

};