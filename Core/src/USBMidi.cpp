#include "USBMidi.hpp"

RtMidiSink::RtMidiSink()
{
    if (midiOut.getPortCount() == 0)
    {
        throw std::runtime_error("No MIDI output ports available");
    }

    midiOut.openPort(0);
}

void RtMidiSink::send(const MidiMessage& message)
{
    const auto& bytes = message.bytes();

    midiOut.sendMessage(&bytes);
}