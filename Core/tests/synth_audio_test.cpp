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
    int sample_rate = 44100;
    int16_t buffer[frames]{0};

    int buffers_per_sec = sample_rate / frames;
    float multiplier = 1.0f;
    int duration = static_cast<int>(buffers_per_sec * multiplier);

    synth.StartTromboneNote(static_cast<Notes::Notes_t>(0), 0);

    for(int octave = 0; octave < 8 && keepRunning; octave++)
    {
        for (int note = 0; note < 12 && keepRunning; note++)
        {
            synth.ChangeTromboneNote(static_cast<Notes::Notes_t>(note), octave);

            for (int i = 0; i < 20 && keepRunning; i++)
            {
                for (int t = 0; t < frames; t++)
                {
                    float sample = synth.ReadTromboneAudio() * 32767.0f;
                    buffer[t] = static_cast<int16_t>(sample);
                    //std::cout << buffer[t];
                }

                output.writeSamples(buffer, frames);
            }
        }
    }

    synth.StopTromboneNote();

    // Let the release/rest stage play out briefly before exiting.
    int release_buffers = buffers_per_sec / 2; // about 0.5 s
    for (int i = 0; i < release_buffers; i++)
    {
        for (int t = 0; t < frames; t++)
        {
            float sample = synth.ReadTromboneAudio() * 32767.0f;
            buffer[t] = static_cast<int16_t>(sample);
            std::cout << buffer[t];
        }

        output.writeSamples(buffer, frames);
    }

    return 0;
}