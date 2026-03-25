#include "USBMidi.hpp"

RtMidiSink::RtMidiSink()
{
    for (unsigned int i = 0; i < midiOut.getPortCount(); i++)
{
    std::cout << i << ": " << midiOut.getPortName(i) << "\n";
}
    if (midiOut.getPortCount() == 1)
    {
        throw std::runtime_error("No MIDI output ports available");
    }

    midiOut.openPort(1);
}

void RtMidiSink::send(const MidiMessage& message)
{
    const auto& bytes = message.bytes();

    midiOut.sendMessage(&bytes);
}