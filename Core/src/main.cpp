#include "main.hpp"

int main() {
    try {
        EventHandler eventHandler;
        RtMidiSink midiSink;
        AudioRender render;
        AmplitudeMapper amplitudemapper;
        PitchMapper pitchmapper;
        MidiCoordinator coordinator(render);

        const bool externalDevicePresent = midiSink.GetDeviceStatus();
        coordinator.setDevice(externalDevicePresent);

        if (!eventHandler.initialise()) {
            std::cerr << "Initialisation failed\n";
            return 1;
        }

        coordinator.RegisterCallback(
            [&](const MidiMessage& msg) {
                midiSink.send(msg);
            });

        eventHandler.start();

        if (!externalDevicePresent) {
            render.start();

            render.setDebugTone(true);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            render.setDebugTone(false);

            std::cout << "Internal synth mode\n";
        } else {
            std::cout << "External MIDI mode\n";
        }

        std::cout << "Beginning Pressure Baseline Calculation\n";

        while (true) {
            RawInputEvent event = eventHandler.waitForEvent();

            if (event.type == RawInputEvent::Type::PressureReading) {
                if (amplitudemapper.calculateBaseline(event.pressureReading)) {
                    break;
                }
            }
        }

        std::cout << "Pressure Baseline found to be: "
                  << amplitudemapper.getBaseline() << "\n";

        int current_note = 0;
        int new_note = 0;

        while (true) {
            RawInputEvent event = eventHandler.waitForEvent();

            switch (event.type) {
                case RawInputEvent::Type::ToFDistance:
                    coordinator.setBend(
                        pitchmapper.tof_to_MIDI_bend(event.tofDistance));
                    break;

                case RawInputEvent::Type::PressureReading:
                    coordinator.PressureEdge(
                        amplitudemapper.noteEdge(event.pressureReading));
                    break;

                case RawInputEvent::Type::MouthpieceReading:
                    new_note = pitchmapper.mouthpiece_to_MIDI_note(
                        event.mouthpieceReading);
                    if (current_note != new_note && new_note != 0) {
                        current_note = new_note;
                        coordinator.ChangeNote(current_note);
                    }
                    break;

                case RawInputEvent::Type::Keycontrol:
                    break;
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}