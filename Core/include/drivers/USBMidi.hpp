#pragma once

#include <RtMidi.h>
#include "MidiMessage.hpp"
#include <iostream>
#include <stdexcept>

class RtMidiSink {
    public:
        RtMidiSink(bool isTest);
        void send(const MidiMessage& message);

        void DetectUSBMidiOutput();

        bool GetDeviceStatus();

    private:
        RtMidiOut midiOut;

        bool EXTERNAL_DEVICE_PRESENT = false;
        int externalPortIndex = -1;
};