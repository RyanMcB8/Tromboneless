#include "USBMidi.hpp"

RtMidiSink::RtMidiSink(bool isTest)
{
    std::cout << "IsTestMode Sink " << isTest << "\n";
    if (isTest){
        return;
    }
    DetectUSBMidiOutput();

    if (EXTERNAL_DEVICE_PRESENT)
    {
        midiOut.openPort(externalPortIndex);
        std::cout << "USB MIDI port " << midiOut.getPortName(externalPortIndex) << " opened.\n";
    }
    else
    {
        std::cout << "No external USB MIDI device detected. Using internal synth.\n";
    }
}

void RtMidiSink::send(const MidiMessage& message)
{
    if (!EXTERNAL_DEVICE_PRESENT)
        return;

    const auto& bytes = message.bytes();
    midiOut.sendMessage(&bytes);
}

void RtMidiSink::DetectUSBMidiOutput()
{
    EXTERNAL_DEVICE_PRESENT = false;
    externalPortIndex = -1;

    unsigned int portCount = midiOut.getPortCount();

    for (unsigned int i = 0; i < portCount; i++)
    {
        std::string portName = midiOut.getPortName(i);
        std::cout << i << ": " << portName << "\n";

        if (portName.find("Boutiq") != std::string::npos ||
            portName.find("Roland") != std::string::npos)
        {
            EXTERNAL_DEVICE_PRESENT = true;
            externalPortIndex = static_cast<int>(i);
            break;
        }
    }
}

bool RtMidiSink::GetDeviceStatus(){
    return EXTERNAL_DEVICE_PRESENT;
}