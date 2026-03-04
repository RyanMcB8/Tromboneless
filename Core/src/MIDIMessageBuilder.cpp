// src/MIDIMessageBuilder.cpp
#include "MIDIMessageBuilder.hpp"

MidiMessage MIDIMessageBuilder::noteOn(int channel, Byte note, Byte velocity) const
{
    Byte ch = clampChannel(channel);          // 0..15
    Byte status = static_cast<Byte>(0x90 | ch); // Note On, channel
    return MidiMessage{status, note, velocity};
}

MidiMessage MIDIMessageBuilder::noteOff(int channel, Byte note, Byte velocity) const
{
    Byte ch = clampChannel(channel);           // 0..15
    Byte status = static_cast<Byte>(0x80 | ch);  // Note Off, channel
    return MidiMessage{status, note, velocity};
}

MIDIMessageBuilder::Byte MIDIMessageBuilder::clampChannel(int channel)
{
    // MIDI channels in the wire protocol are 0..15, but humans usually say 1..16.
    if (channel < 1) channel = 1;
    if (channel > 16) channel = 16;
    return static_cast<Byte>(channel - 1);
}