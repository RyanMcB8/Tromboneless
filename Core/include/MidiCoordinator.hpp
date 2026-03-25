#pragma once
#include "MidiMessage.hpp"
#include "MIDIMessageBuilder.hpp"
#include "functional"

/**
 * @brief Class to coordinate MIDI message construction based on output from sensor mappers.
 * 
 * This class is stateful and responsible for translating high-level sensor data into MIDI messages using MIDIMessageBuilder.
 */


class MidiCoordinator{
    private:

        MidiMessage message;

        using CallbackInterface = std::function<void(const MidiMessage&)>;
        CallbackInterface callback;

        bool noteActive = false; // Update with pressure gate.

        int latestNote = -1; // Updated by mouthpiece. [0:127]
        int latestExpr = -1; // Updated by pressure. [0:127]
        int latestBend = -1; // Updated by slide. [-8192:8191]

        int currentNote = -1; // Playing note 
        int lastSentExpr = -1; // Saves expression. 
        int lastSentBend = 10000; // Saves bend

        int velocity = 127; // Clamped to max

    public:
        // Methods should be input events
        /**
         * Default constructor.
         */
        MidiCoordinator();
        void RegisterCallback(CallbackInterface cb);
        void setGate(bool on);
        void setNote(int note);
        void setBend(int bend);
        void setExpr(int expr);
};
