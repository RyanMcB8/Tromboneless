/**
 * @file audio_output.hpp
 * @author Aidan McIntosh
 * @brief Declares the AudioOutput class, responsible for interfacing with the
 *        ALSA audio subsystem for playback.
 * @date 2026-04-20
 */

#pragma once

#include <alsa/asoundlib.h>
#include <cstdint>

/**
 * @brief Provides a low-level interface for audio playback via ALSA.
 *
 * This class encapsulates the configuration and use of an ALSA PCM playback
 * device. It is responsible for sending rendered audio buffers to the system
 * audio output in a format compatible with the configured device.
 *
 * In test mode, hardware interaction is disabled, allowing the rest of the
 * audio pipeline to be exercised without requiring a physical audio device.
 *
 * @note This class assumes audio data is provided as interleaved 16-bit samples.
 * @note Intended for real-time usage; buffer writes should be performed
 *       consistently to avoid underruns.
 */
class AudioOutput {
public:
    /**
     * @brief Constructs the AudioOutput interface.
     * @param isTest Enables test mode (disables ALSA interaction).
     */
    AudioOutput(bool isTest);

    /**
     * @brief Destructor.
     *
     * Ensures that the ALSA device is properly closed and resources are released.
     */
    ~AudioOutput();

    /**
     * @brief Writes a buffer of audio samples to the output device.
     * @param buffer Pointer to the audio sample buffer.
     * @param frames Number of frames to write.
     *
     * The buffer is expected to contain 16-bit signed samples. The interpretation
     * (mono/stereo, interleaving) must match the device configuration.
     *
     * In test mode, this function performs no hardware write.
     */
    void writeSamples(const int16_t* buffer, int frames);

private:
    /** @brief Handle to the ALSA PCM playback device. */
    snd_pcm_t* handle = nullptr;

    /**
     * @brief Flag indicating test mode operation.
     *
     * When enabled, audio writes are suppressed and no device interaction occurs.
     */
    bool isTestMode = false;
};