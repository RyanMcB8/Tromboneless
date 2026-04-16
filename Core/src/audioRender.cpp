#include "audioRender.hpp"
#include <iostream>

AudioRender::AudioRender() 
    : synth(), output()
{
}

AudioRender::~AudioRender() {
    stop();
}

void AudioRender::start() {
    if (running.load()) {
        return; // Already running
    }

    stopRequested.store(false);
    running.store(true);
    
    renderThread = std::thread(&AudioRender::renderLoop, this);
}

void AudioRender::stop() {
    if (!running.load()) {
        return; // Not running
    }

    stopRequested.store(true);

    if (renderThread.joinable()) {
        renderThread.join();
    }

    running.store(false);
}

void AudioRender::renderLoop() {
    while (!stopRequested.load()) {
        for (int t = 0; t < frames; t++) {
            float sample = synth.ReadTromboneAudio() * 32767.0f;
            // std::cout << "Sample: " << static_cast<int16_t>(sample) << std::endl;
            buffer[t] = static_cast<int16_t>(sample);
        }
        output.writeSamples(buffer, frames);
    }

    // Render release tail after stop request
    renderReleaseTail();
}

void AudioRender::renderReleaseTail() {
    synth.StopTromboneNote();

    constexpr int buffersPerSec = sample_rate / frames;
    constexpr int releaseBuffers = buffersPerSec / 2; // 0.5 second tail

    for (int i = 0; i < releaseBuffers; i++) {
        for (int t = 0; t < frames; t++) {
            float sample = synth.ReadTromboneAudio() * 32767.0f;
            buffer[t] = static_cast<int16_t>(sample);
        }
        output.writeSamples(buffer, frames);
    }
}