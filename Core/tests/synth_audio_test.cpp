#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include "synth.hpp"
#include "tromboneSynth.hpp"
#include "drivers/audio_output.hpp"
#include <string>

int main(){
    TromboneSynth synth = TromboneSynth();

    AudioOutput output;

    float amplitude;

    constexpr int frames = 512; // Samples in a buffer
    int sample_rate = 44100;
    int16_t buffer[frames]; // Raw C-style array

    // How long notes will play
    int buffers_per_sec = int(sample_rate / frames); 
    float multiplier = 3.0;
    int duration = int(buffers_per_sec * multiplier);

    int sample_index = 0;
    float attack_time = 0.1f;

    synth.ChangeTromboneNote(Notes::Notes::note_A, 3);
    synth.StartTromboneNote(Notes::Notes::note_A, 3);

    // for(int n; n<7; n++)
    // {
        for(int i = 0; i < duration; i++)
        {
            for(int t = 0; t < frames; t++)
            {
                //float time = static_cast<float>(sample_index) / static_cast<float>(sample_rate);
                //float attack_progress = time / attack_time;

                //synth.StartNoteWithHarmonics(4, 2, Notes::Notes_t(n), time, attack_progress) * 32767.0f;
                //amplitude = 1;
                buffer[t] = static_cast<int16_t>(synth.ReadTromboneAudio()*32767.0f);
                std::cout << buffer[i] << " ";      
                sample_index++;
            }

            output.writeSamples(buffer, frames);
        }
    // }

    for(int i = 0; i < frames; i++)
    {
        std::cout << buffer[i] << " ";
    }

    synth.StopTromboneNote();

    return 0;
}