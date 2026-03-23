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

MidiMessage MIDIMessageBuilder::pitchBend(int channel, int value) const
{
    Byte ch = clampChannel(channel);           // 0..15
    Byte status = static_cast<Byte>(0xE0 | ch);  // Pitch Bend, channel

    // MIDI pitch bend is a 14-bit value (0..16383) with center at 8192.
    // This implementation clamps to between 0 and 8192 to provide only downward bends.
    if (value < 0) value = 0;
    if (value > 8192) value = 8192;

    Byte lsb = static_cast<Byte>(value & 0x7F);        // Lower 7 bits
    Byte msb = static_cast<Byte>((value >> 7) & 0x7F); // Upper 7 bits

    return MidiMessage{status, lsb, msb};
}

MIDIMessageBuilder::Byte MIDIMessageBuilder::clampChannel(int channel)
{
    // MIDI channels in the wire protocol are 0..15, but humans usually say 1..16.
    if (channel < 1) channel = 1;
    if (channel > 16) channel = 16;
    return static_cast<Byte>(channel - 1);
}