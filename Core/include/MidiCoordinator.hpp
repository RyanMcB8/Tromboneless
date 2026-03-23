#pragma once

/**
 * @brief Class to coordinate MIDI message construction based on output from sensor mappers.
 * 
 * This class is stateful and responsible for translating high-level sensor data into MIDI messages using MIDIMessageBuilder.
 */


class MidiCoordinator{
    private:
        bool noteActive;
        int currentNote;
        int latestNote;
        int latestExpr;
        int lastSentBend;
        int lastSentExpr;
    public:
        
};
