/** @file   hpp.MidiCoordinator
 *  @author Aidan McIntosh
 *  @brief  File to define high-level musical coordination behaviour.
 *          Takes premapped sensor inputs and either internal 
 *          or external synth.
 */

#pragma once
#include "MidiMessage.hpp"
#include "MidiMessageBuilder.hpp"
#include "functional"
#include "tromboneSynth.hpp"

/** @brief 
 *          Class to coordinate Midi message construction based on output from sensor mappers.
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
        TromboneSynth& synthRef;

        using CallbackInterface = std::function<void(const MidiMessage&)>;
        CallbackInterface callback;

        int latestNote = -1; // Updated by mouthpiece. [0:127]
        int latestExpr = -1; // Updated by pressure. [0:127]
        int latestBend = -1; // Updated by slide. [-8192:8191]

        int currentNote = -1; // Playing note 
        int lastSentExpr = -1; // Saves last expression sent to device
        int lastSentBend = 10000; // Saves last bend sent to device

        const int velocity = 127; // Clamped to max
        /**
         * States dependent on whether external or internal synth is being used.
         */
        enum OutputType{
            EXTERNAL,
            INTERNAL
        };

        enum OutputType current_output = INTERNAL;

        /**
         * High-level states that device can be in.
         */

        enum State{
            IDLE,
            PLAYING,
            ERROR
        };

        enum State current_state = IDLE;

    public:
        // Methods should be input events
        /**
         * Default constructor.
         */
        MidiCoordinator(TromboneSynth& synth);
        void RegisterCallback(CallbackInterface cb);

        /**
         * Method to update gate on/off.
         * @param on Boolean: Decides whether 
         */
        void PressureEdge(bool on);
        /**
         * @brief Method to update note from mouthpiece.
         * @param note MIDI standard 0-127 notes.
         */
        void ChangeNote(int note);
        /**
         * Method to update pitch bend from slide.
         * @param bend Values from -8192 to 0 (so only downward bends possible)
         */
        void setBend(int bend);
        /**
         * Method to update MIDI expression.
         * @param expr: Expression value, 0-127.
         */
        void setExpr(int expr);

        /**
         * Method to set state of instrument.
         */
        void setState(State newstate);

        TromboneSynth& getSynth(void);
};
