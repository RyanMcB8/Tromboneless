#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include "synth.hpp"
#include "tromboneSynth.hpp"
#include "drivers/audio_output.hpp"
#include <string>

int main(){
    Octaves synth = Octaves();
    TromboneSynth synth2;
    
    AudioOutput output;
    float amplitude;

    constexpr int frames = 512;
    int sample_rate = 44100;
    int16_t buffer[frames];

    int second = int(sample_rate/frames);
    int multiplier = 1;
    int duration = second*multiplier;

    //synth2.NewTromboneNote(Notes::Notes_t::note_B,  4);

    for(int i=0; i<duration; i++)
    {
        for(int t = 0; t<frames; t++){

            //amplitude = synth.PlayingNote(4, Notes::Notes_t(s), (float)(t))*32767.0f;
            amplitude = synth2.ReadTromboneAudio()*32767.0f;
            buffer[t] = static_cast<int16_t>(amplitude);

        }
        output.writeSamples(buffer, frames);
    }

    for(int i = 0; i<frames; i++)
    {       
        std::cout << buffer[i] << " ";
    }

    synth2.EndTromboneAudio();
    return 0;
}