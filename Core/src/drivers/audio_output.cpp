#include "audio_output.hpp"
#include <iostream>

AudioOutput::AudioOutput() {
    int err = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0) {
        std::cerr << "Error opening device: " << snd_strerror(err) << "\n";
        handle = nullptr;
        return;
    }

    err = snd_pcm_set_params(
        handle,
        SND_PCM_FORMAT_S16_LE,
        SND_PCM_ACCESS_RW_INTERLEAVED,
        1,          // mono
        44100,      // sample rate
        1,          // allow resampling
        500000      // latency in us
    );

    if (err < 0) {
        std::cerr << "Error setting parameters: " << snd_strerror(err) << "\n";
        snd_pcm_close(handle);
        handle = nullptr;
    }
}

AudioOutput::~AudioOutput() {
    if (handle != nullptr) {
        snd_pcm_close(handle);
    }
}

void AudioOutput::writeSamples(const int16_t* buffer, int frames) {
    if (handle == nullptr) {
        return;
    }

    int err = snd_pcm_writei(handle, buffer, frames);
    if (err < 0) {
        std::cerr << "Write error: " << snd_strerror(err) << "\n";
    }
}