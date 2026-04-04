#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include "synth.hpp"
#include "drivers/audio_output.hpp"
#include <string>

int main(){
    Octaves synth = Octaves();
    AudioOutput output;
    float amplitude;

    const int frames = 512;
    int sample_rate = 44100;
    int16_t buffer[frames];

    int second = int(sample_rate/frames);
    int multiplier = 1;
    int duration = second*multiplier;

    for(int s = 0; s<8; s++)
    {    
        for(int i=0; i<duration; i++)
        {
            for(int t = 0; t<frames; t++){

                amplitude = synth.PlayingNote(4, Notes::Notes_t(s), (float)(t))*32767.0f;
                buffer[t] = static_cast<int16_t>(amplitude);
                std::cout << buffer[t];

            }
            output.writeSamples(buffer, frames);
        }}
    return 0;
}