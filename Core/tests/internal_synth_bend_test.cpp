#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <csignal>

#include "tromboneSynth.hpp"
#include "drivers/audio_output.hpp"

volatile std::sig_atomic_t keepRunning = 1;

void handleSigInt(int)
{
    keepRunning = 0;
}

int main() {
    std::signal(SIGINT, handleSigInt);

    TromboneSynth synth = TromboneSynth();
    AudioOutput output;

    constexpr int frames = 512;
    const int sample_rate = 44100;
    int16_t buffer[frames]{0};

    const int buffers_per_sec = sample_rate / frames;

    // Hold one note and sweep pitch bend across its range.
    synth.NewTromboneNoteMIDI(40); 
    synth.StartTromboneNote(Notes::note_A, 3);

    // Sit at centre bend briefly first.
    synth.setPitchBend(8192);
    for (int i = 0; i < buffers_per_sec && keepRunning; i++)
    {
        for (int t = 0; t < frames; t++)
        {
            float sample = synth.ReadTromboneAudio() * 32767.0f;
            buffer[t] = static_cast<int16_t>(sample);
        }
        output.writeSamples(buffer, frames);
    }

    // Sweep down from centre to minimum.
    for (int bend = 8192; bend >= 0 && keepRunning; bend -= 64)
    {
        synth.setPitchBend(bend);

        for (int i = 0; i < 2 && keepRunning; i++)
        {
            for (int t = 0; t < frames; t++)
            {
                float sample = synth.ReadTromboneAudio() * 32767.0f;
                buffer[t] = static_cast<int16_t>(sample);
            }
            output.writeSamples(buffer, frames);
        }
    }

    // Sweep up from minimum to centre.
    for (int bend = 0; bend <= 8192 && keepRunning; bend += 64)
    {
        synth.setPitchBend(bend);

        for (int i = 0; i < 2 && keepRunning; i++)
        {
            for (int t = 0; t < frames; t++)
            {
                float sample = synth.ReadTromboneAudio() * 32767.0f;
                buffer[t] = static_cast<int16_t>(sample);
            }
            output.writeSamples(buffer, frames);
        }
    }

    synth.StopTromboneNote();

    const int release_buffers = buffers_per_sec / 2;
    for (int i = 0; i < release_buffers; i++)
    {
        for (int t = 0; t < frames; t++)
        {
            float sample = synth.ReadTromboneAudio() * 32767.0f;
            buffer[t] = static_cast<int16_t>(sample);
        }
        output.writeSamples(buffer, frames);
    }

    return 0;
}