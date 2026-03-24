#pragma once
#include "MidiMessage.hpp"
#include "functional"

/**
 * @brief Class to coordinate MIDI message construction based on output from sensor mappers.
 * 
 * This class is stateful and responsible for translating high-level sensor data into MIDI messages using MIDIMessageBuilder.
 */


class MidiCoordinator{
    private:      
        using CallbackInterface = std::function<void(const MidiMessage)>;
        CallbackInterface callback;

        bool noteActive;
        int currentNote;
        int latestNote;
        int latestExpr;
        int lastSentBend;
        int lastSentExpr;
    public:
        // Methods should be input events
        MidiCoordinator();
        void RegisterCallback(CallbackInterface cb);
        void setGate(bool on);
        void setNote(int note);
        void setBend(int bend);
        void setExpr(int expr);
};
