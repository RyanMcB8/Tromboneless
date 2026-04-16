#pragma once

#include <atomic>
#include <thread>
#include <cstdint>

#include "tromboneSynth.hpp"
#include "drivers/audio_output.hpp"

class AudioRender {
public:
    static constexpr int frames = 512;
    static constexpr int sample_rate = 44100;

    AudioRender();
    ~AudioRender();

    // Prevent copying
    AudioRender(const AudioRender&) = delete;
    AudioRender& operator=(const AudioRender&) = delete;

    /** @brief Start the audio render thread */
    void start();

    /** @brief Stop the audio render thread (with release tail) */
    void stop();

    /** @brief Check if currently rendering */
    bool isRunning() const { return running.load(); }

    /** @brief Get reference to synth for external control */
    TromboneSynth& getSynth() { return synth; }

private:
    void renderLoop();
    void renderReleaseTail();

    TromboneSynth synth = TromboneSynth();
    AudioOutput output;

    std::thread renderThread;
    std::atomic<bool> running{false};
    std::atomic<bool> stopRequested{false};

    int16_t buffer[frames]{0};
};