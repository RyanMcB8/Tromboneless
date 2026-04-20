/** @file   MidiCoordinator.hpp
 *  @author Aidan McIntosh
 *  @brief  Declaration of class for high-level musical coordination behaviour.
 *          Takes premapped sensor inputs and either internal 
 *          or external synth.
 */

#pragma once
#include "MidiMessage.hpp"
#include "MidiMessageBuilder.hpp"
#include "functional"
#include "tromboneSynth.hpp"
#include "audioRender.hpp"

/** @brief  Class to coordinate Midi message construction based on output from sensor mappers.
 *          This class is stateful and responsible for translating high-level sensor data into Midi messages.
 *          Utilises MidiMessageBuilder for external synths and directly controls internal synth.
 */
class MidiCoordinator{
    private:
/** @brief Initialising downstream classes for constructing, storing and sending
 *         MIDI messages to internal and external synths.
 * 
 */
        MidiMessage message;
        MidiMessageBuilder builder;
        AudioRender& synthRef;

        using CallbackInterface = std::function<void(const MidiMessage&)>;
        CallbackInterface callback;

        int latestNote = -1; // Updated by mouthpiece. [0:127]
        int latestExpr = -1; // Updated by pressure. [0:127]
        int latestBend = -1; // Updated by slide. [0:8192]

        int currentNote = -1; // Playing note 
        int lastSentExpr = -1; // Saves last expression sent to device
        int lastSentBend = -1; // Saves last bend sent to device

        const int velocity = 127; // Clamped to max
        /**
         * @brief States dependent on whether external or internal synth is being used.
         */
        typedef enum OutputType{
            EXTERNAL,
            INTERNAL
        } outputtype;

        outputtype current_output = INTERNAL;

        /**
         * High-level states that device can be in.
         */

        enum State{
            IDLE,
            PLAYING,
            ERROR
        };

        enum State current_state = IDLE;
        bool isTestMode = false;

    public:

        /**
         * @brief Constructor takes reference to internal synth and optionally triggers tests.
         */
        MidiCoordinator(AudioRender& synth, bool isTest);
        void RegisterCallback(CallbackInterface cb);

        /**
         * @brief Method to update gate on/off.
         * @param on true = 'Note on' message, false = 'Note off'
         */
        void PressureEdge(bool on);
        /**
         * @brief Method to update note from mouthpiece.
         * @param note MIDI standard 0-127 notes.
         */
        void ChangeNote(int note);
        /**
         * @brief Method to update pitch bend from slide.
         * @param bend Values from 8192 to 0 (so only downward bends possible)
         */
        void setBend(int bend);
        /**
         * @param Method to update MIDI expression.
         * @param expr: Expression value, 0-127.
         */
        void setExpr(int expr);

        /**
         * @brief Method to set state of instrument.
         * @param newstate Set state to either IDLE, PLAYING or ERROR.
         */
        void setState(State newstate);

        /**
         * @brief Set state of whether to use internal or external synth
         * 
         * @param external_device_present true = Use soundcard/external synth, false = use internal synth
         */
        void setDevice(bool external_device_present);

        AudioRender& getSynth(void);
};
