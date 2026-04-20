/**
 * @file USBMidi.hpp
 * @author Aidan McIntosh
 * @brief Declares the RtMidiSink class, responsible for sending MIDI messages
 *        to an external device via RtMidi.
 * @date 2026-04-20
 */

#pragma once

#include <RtMidi.h>
#include "MidiMessage.hpp"
#include <iostream>
#include <stdexcept>

/**
 * @brief Handles MIDI output to external devices using RtMidi.
 *
 * This class wraps the RtMidiOut interface to provide a simplified mechanism
 * for sending MIDI messages to a connected USB MIDI device.
 *
 * It is responsible for:
 * - Detecting available MIDI output ports
 * - Selecting and opening an external MIDI device
 * - Sending formatted MIDI messages
 *
 * The class maintains state indicating whether a valid external device is
 * available, allowing higher-level components to fall back to internal
 * synthesis if required.
 */
class RtMidiSink {
public:
    /**
     * @brief Constructs the MIDI sink.
     * @param isTest Enables test mode (may disable actual device interaction).
     */
    RtMidiSink(bool isTest);

    /**
     * @brief Sends a MIDI message to the selected output device.
     * @param message MIDI message to transmit.
     *
     * The message is forwarded to the currently opened RtMidi output port.
     * Behaviour is undefined if no valid device is available.
     */
    void send(const MidiMessage& message);

    /**
     * @brief Detects and selects a suitable USB MIDI output device.
     *
     * Scans available MIDI output ports and attempts to identify an external
     * device. If successful, stores the port index and marks the device as present.
     */
    void DetectUSBMidiOutput();

    /**
     * @brief Returns the status of the external MIDI device.
     * @return True if a valid external device is available, false otherwise.
     */
    bool GetDeviceStatus();

private:
    /** @brief RtMidi output interface instance. */
    RtMidiOut midiOut;

    /** @brief Flag indicating whether an external MIDI device is present. */
    bool EXTERNAL_DEVICE_PRESENT = false;

    /** @brief Index of the selected external MIDI output port. */
    int externalPortIndex = -1;
};