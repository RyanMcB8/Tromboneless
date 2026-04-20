#pragma once

#include <atomic>
#include <thread>
#include <cstdint>
#include <mutex>

#include "tromboneSynth.hpp"
#include "drivers/audio_output.hpp"

class AudioRender : public TromboneSynth {
public:
    static constexpr int frames = 128;
    static constexpr int sample_rate = 44100;

    AudioRender(bool isTest);
    ~AudioRender();

    AudioRender(const AudioRender&) = delete;
    AudioRender& operator=(const AudioRender&) = delete;

    void start();
    void stop();

    bool isRunning() const { return running.load(); }

    // Thread-safe wrappers for synth control from other threads
    void noteOnFromMidi(int midiNote);
    void noteOffFromMidi();
    void noteChangeFromMidi(int midiNote);
    void pitchBendFromMidi(int bend);

    // Optional debug mode to prove audio path works
    void setDebugTone(bool enabled);

private:
    void renderLoop();
    void renderReleaseTail();

    AudioOutput output;

    std::thread renderThread;
    std::atomic<bool> running{false};
    std::atomic<bool> stopRequested{false};
    std::atomic<bool> debugToneEnabled{false};

    std::mutex synthMutex;

    int16_t buffer[frames]{0};
    float debugPhase{0.0f};
    bool isTestMode = false;
};