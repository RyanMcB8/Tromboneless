#include "audio_output.hpp"
#include <iostream>

AudioOutput::AudioOutput() {
    int err = snd_pcm_open(&handle, "plughw:CARD=Device,DEV=0", SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0) {
        std::cerr << "Error opening audio device: " << snd_strerror(err) << "\n";
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
        5000      // latency in us
    );

    if (err < 0) {
        std::cerr << "Error setting parameters: " << snd_strerror(err) << "\n";
        snd_pcm_close(handle);
        handle = nullptr;
        return;
    }

    err = snd_pcm_prepare(handle);
    if (err < 0) {
        std::cerr << "Error preparing audio device: " << snd_strerror(err) << "\n";
        snd_pcm_close(handle);
        handle = nullptr;
        return;
    }

    std::cout << "Audio device opened successfully\n";
}

AudioOutput::~AudioOutput() {
    if (handle != nullptr) {
        snd_pcm_drain(handle);
        snd_pcm_close(handle);
    }
}

void AudioOutput::writeSamples(const int16_t* buffer, int frames) {
    if (handle == nullptr || buffer == nullptr || frames <= 0) {
        return;
    }

    int framesRemaining = frames;
    const int16_t* dataPtr = buffer;

    while (framesRemaining > 0) {
        snd_pcm_sframes_t written = snd_pcm_writei(handle, dataPtr, framesRemaining);

        if (written == -EPIPE) {
            std::cerr << "Underrun occurred\n";
            int err = snd_pcm_recover(handle, static_cast<int>(written), 0);
            if (err < 0) {
                std::cerr << "Failed to recover from underrun: " << snd_strerror(err) << "\n";
                return;
            }
            continue;
        }

        if (written < 0) {
            int err = snd_pcm_recover(handle, static_cast<int>(written), 0);
            if (err < 0) {
                std::cerr << "Write error: " << snd_strerror(static_cast<int>(written)) << "\n";
                return;
            }
            continue;
        }

        if (written == 0) {
            std::cerr << "No frames written\n";
            return;
        }

        dataPtr += written;         // mono, so 1 sample per frame
        framesRemaining -= static_cast<int>(written);
    }
}