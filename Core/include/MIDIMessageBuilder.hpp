#pragma once

#include "MidiMessage.hpp"

/**
 * @brief Factory class for constructing common MIDI channel messages.
 *
 * This class encapsulates the encoding logic required to construct
 * correctly formatted MIDI channel voice messages.
 *
 * It separates message construction from message transmission,
 * supporting a clean single-responsibility design.
 *
 * Currently supported:
 * - Note On (0x9n)
 * - Note Off (0x8n)
 *
 * Channels are specified using human-friendly numbering (1–16)
 * and internally converted to zero-based encoding (0–15).
 */
class MIDIMessageBuilder
{
public:
    using Byte = MidiMessage::Byte;

    /**
     * @brief Create a MIDI Note On message.
     *
     * @param channel MIDI channel number (1–16).
     * @param note MIDI note number (0–127).
     * @param velocity Note velocity (0–127).
     * @return Constructed MidiMessage.
     */
    MidiMessage noteOn(int channel,
                       Byte note,
                       Byte velocity) const;

    /**
     * @brief Create a MIDI Note Off message.
     *
     * This implementation uses the dedicated Note Off status
     * byte (0x8n) rather than Note On with velocity 0.
     *
     * @param channel MIDI channel number (1–16).
     * @param note MIDI note number (0–127).
     * @param velocity Release velocity (default 0).
     * @return Constructed MidiMessage.
     */
    MidiMessage noteOff(int channel,
                        Byte note,
                        Byte velocity = 0) const;

    /**
     * @brief Pitch bend message constructor.
     * @param channel MIDI channel number (1–16).
     * @param value Pitch bend value (0–8192).
     * @return Constructed MidiMessage.
     */
    MidiMessage pitchBend(int channel, 
                          int value) const;

private:
    /**
     * @brief Clamp a human-friendly channel number to valid MIDI range.
     *
     * Converts 1–16 into 0–15 and ensures values outside the range
     * are safely clamped.
     *
     * @param channel MIDI channel (1–16 expected).
     * @return Zero-based channel value suitable for status byte encoding.
     */
    static Byte clampChannel(int channel);
};