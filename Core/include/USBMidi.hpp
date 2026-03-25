#include "lib/rtmidi/RtMidi.h"
#include "MidiMessage.hpp"

class RtMidiSink {
    public:
        RtMidiSink();
        void send(const MidiMessage& message);

    private:
        RtMidiOut midiOut;
};