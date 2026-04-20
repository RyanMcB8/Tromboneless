/**
 * @file audioRender.hpp
 * @author Aidan McIntosh
 * @brief Declares the AudioRender class, responsible for managing the real-time
 *        audio rendering pipeline and interfacing with the synthesis engine.
 * @date 2026-04-20
 */

#pragma once

#include <atomic>
#include <thread>
#include <cstdint>
#include <mutex>

#include "tromboneSynth.hpp"
#include "drivers/audio_output.hpp"

/**
 * @brief Handles real-time audio generation and output.
 *
 * This class manages a dedicated audio rendering thread which continuously
 * generates audio samples using the inherited TromboneSynth engine and sends
 * them to the AudioOutput device.
 *
 * It provides thread-safe control interfaces for note triggering and pitch
 * manipulation, allowing interaction from external threads (e.g. MIDI input).
 *
 * The class also supports a debug tone mode for validating the audio pipeline
 * independently of the synthesis engine.
 *
 * @note Inherits from TromboneSynth to directly access synthesis functionality.
 * @note Thread-safe interaction is enforced via atomic flags and a mutex.
 */
class AudioRender : public TromboneSynth {
public:
    /** @brief Number of audio frames processed per buffer. */
    static constexpr int frames = 128;

    /** @brief Audio sample rate in Hz. */
    static constexpr int sample_rate = 44100;

    /**
     * @brief Constructs the AudioRender object.
     * @param isTest Enables test mode behaviour (e.g. simplified or debug operation).
     */
    AudioRender(bool isTest);

    /** @brief Destructor. Ensures rendering thread is stopped safely. */
    ~AudioRender();

    AudioRender(const AudioRender&) = delete;
    AudioRender& operator=(const AudioRender&) = delete;

    /**
     * @brief Starts the audio rendering thread.
     */
    void start();

    /**
     * @brief Requests the rendering thread to stop and joins it.
     */
    void stop();

    /**
     * @brief Checks whether the rendering thread is active.
     * @return True if rendering is ongoing, false otherwise.
     */
    bool isRunning() const { return running.load(); }

    /**
     * @brief Triggers a note-on event from a MIDI source.
     * @param midiNote MIDI note number.
     *
     * Thread-safe wrapper around synth note-on behaviour.
     */
    void noteOnFromMidi(int midiNote);

    /**
     * @brief Triggers a note-off event from a MIDI source.
     *
     * Thread-safe wrapper around synth note-off behaviour.
     */
    void noteOffFromMidi();

    /**
     * @brief Changes the currently playing note without retriggering.
     * @param midiNote New MIDI note number.
     *
     * Intended for legato-style transitions.
     */
    void noteChangeFromMidi(int midiNote);

    /**
     * @brief Applies pitch bend from a MIDI source.
     * @param bend Pitch bend value (typically 14-bit MIDI range).
     *
     * Thread-safe wrapper for continuous pitch control.
     */
    void pitchBendFromMidi(int bend);

    /**
     * @brief Enables or disables debug tone output.
     * @param enabled True to enable debug tone, false to disable.
     *
     * When enabled, a simple tone is generated instead of the synth output,
     * useful for validating the audio pipeline.
     */
    void setDebugTone(bool enabled);

private:
    /**
     * @brief Main rendering loop executed in a separate thread.
     *
     * Continuously fills audio buffers and sends them to the output device
     * until stop is requested.
     */
    void renderLoop();

    /**
     * @brief Handles release tail rendering after note-off.
     *
     * Ensures smooth decay of audio to avoid abrupt cut-offs.
     */
    void renderReleaseTail();

    /** @brief Audio output interface. */
    AudioOutput output;

    /** @brief Thread responsible for audio rendering. */
    std::thread renderThread;

    /** @brief Indicates whether rendering is currently active. */
    std::atomic<bool> running{false};

    /** @brief Signals the render thread to stop. */
    std::atomic<bool> stopRequested{false};

    /** @brief Enables debug tone generation. */
    std::atomic<bool> debugToneEnabled{false};

    /** @brief Mutex protecting synth state during cross-thread access. */
    std::mutex synthMutex;

    /** @brief Audio buffer for a single render block. */
    int16_t buffer[frames]{0};

    /** @brief Phase accumulator for debug tone generation. */
    float debugPhase{0.0f};

    /** @brief Flag indicating test mode operation. */
    bool isTestMode = false;
};