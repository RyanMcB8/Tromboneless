#include "audioRender.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>

AudioRender::AudioRender(bool isTest)
    : output(isTest), isTestMode(isTest)
{
    std::cout << "IsTestMode AudioRender " << isTest << "\n";
    if (isTestMode){
        return;
    }
}

AudioRender::~AudioRender()
{
    stop();
}

void AudioRender::start()
{
    std::cout << "Audio renderer started." << std::endl;
    if (running.load())
        return;

    stopRequested.store(false);

    try {
        renderThread = std::thread(&AudioRender::renderLoop, this);
        running.store(true);
    } catch (...) {
        running.store(false);
        stopRequested.store(false);
        throw;
    }
}

void AudioRender::stop()
{
    stopRequested.store(true);

    if (renderThread.joinable())
        renderThread.join();

    running.store(false);
}

void AudioRender::setDebugTone(bool enabled)
{
    debugToneEnabled.store(enabled);
}

void AudioRender::noteOnFromMidi(int midiNote)
{
    std::lock_guard<std::mutex> lock(synthMutex);
    NewTromboneNoteMIDI(midiNote);
    HandleMIDINoteOn();
}

void AudioRender::noteOffFromMidi()
{
    std::lock_guard<std::mutex> lock(synthMutex);
    HandleMIDINoteOff();
}

void AudioRender::noteChangeFromMidi(int midiNote)
{
    std::lock_guard<std::mutex> lock(synthMutex);
    HandleMIDINoteChange(midiNote);
}

void AudioRender::pitchBendFromMidi(int bend)
{
    std::lock_guard<std::mutex> lock(synthMutex);
    HandleMIDIPitchBend(bend);
}

void AudioRender::renderLoop()
{
    std::cout << "Renderloop started." << std::endl;
    while (!stopRequested.load()) {
        for (int t = 0; t < frames; ++t) {
            float sample = 0.0f;

            if (debugToneEnabled.load()) {
                constexpr float freq = 440.0f;
                float phaseInc = 2.0f * static_cast<float>(M_PI) * freq / static_cast<float>(sample_rate);
                debugPhase += phaseInc;
                if (debugPhase >= 2.0f * static_cast<float>(M_PI))
                    debugPhase -= 2.0f * static_cast<float>(M_PI);

                sample = 0.15f * std::sin(debugPhase);
            } else {
                std::lock_guard<std::mutex> lock(synthMutex);
                sample = ReadTromboneAudio();
            }

            sample = std::clamp(sample, -1.0f, 1.0f);
            buffer[t] = static_cast<int16_t>(sample * 32767.0f);
            //std::cout << "Buffer: " << buffer[t] << std::endl;
        }

        output.writeSamples(buffer, frames);
    }

    renderReleaseTail();
}

void AudioRender::renderReleaseTail()
{
    {
        std::lock_guard<std::mutex> lock(synthMutex);
        StopTromboneNote();
    }

    constexpr int tailFrames = sample_rate / 2;
    int rendered = 0;

    while (rendered < tailFrames) {
        int block = std::min(frames, tailFrames - rendered);

        for (int t = 0; t < block; ++t) {
            float sample;
            {
                std::lock_guard<std::mutex> lock(synthMutex);
                sample = ReadTromboneAudio();
            }

            sample = std::clamp(sample, -1.0f, 1.0f);
            buffer[t] = static_cast<int16_t>(sample * 32767.0f);
        }

        output.writeSamples(buffer, block);
        rendered += block;
    }
}