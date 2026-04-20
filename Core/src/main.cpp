#include "main.hpp"
#include "eventHandler.hpp"
#include <condition_variable>
#include <cstdint>
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <chrono>
#include "MidiCoordinator.hpp"
#include "tromboneSynth.hpp"
#include "USBMidi.hpp"
#include "audioRender.hpp"
#include "PitchMapper.hpp"
#include "AmplitudeMapper.hpp"
#include "CoreWrapper.hpp"

int main() {
    try {
        CoreWrapper coreWrapper(false);
        coreWrapper.start();

        /* Runnning core indefinitely. */
        std::this_thread::sleep_for(std::chrono::hours(24));
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}