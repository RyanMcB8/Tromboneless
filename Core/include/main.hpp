/* Defining main.h to have so there is no recursion */

#ifndef __MAIN_H_
#define __MAIN_H_
#ifdef C_PLUS_PLUS
C{
#endif

/* Including header files */

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
#include <iostream>
#include "tromboneless_data.hpp"

#ifdef C_PLUS_PLUS
}
#endif
#endif