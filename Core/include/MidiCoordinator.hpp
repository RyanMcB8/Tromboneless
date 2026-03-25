#pragma once
#include "MidiMessage.hpp"
#include "MidiMessageBuilder.hpp"
#include "functional"

/**
 * @brief Class to coordinate Midi message construction based on output from sensor mappers.
 * 
 * This class is stateful and responsible for translating high-level sensor data into Midi messages using MidiMessageBuilder.
 */


class MidiCoordinator{
    private:

        MidiMessage message;
        MidiMessageBuilder builder;

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
        MidiCoordinator();
        void RegisterCallback(CallbackInterface cb);

        /**
         * Method to update gate on/off.
         */
        void PressureEdge(bool on);
        /**
         * Method to update note from mouthpiece.
         */
        void ChangeNote(int note);
        /**
         * Method to update pitch bend from slide.
         */
        void setBend(int bend);
        /**
         * Method to update MIDI expression.
         */
        void setExpr(int expr);

        /**
         * Method to set state of instrument.
         */
        void setState(State newstate);
};
