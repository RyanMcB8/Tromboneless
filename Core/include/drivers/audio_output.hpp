#pragma once

#include <alsa/asoundlib.h>
#include <cstdint>

class AudioOutput {
public:
    AudioOutput(bool isTest);
    ~AudioOutput();

    void writeSamples(const int16_t* buffer, int frames);

private:
    snd_pcm_t* handle = nullptr;
    bool isTestMode = false;
};