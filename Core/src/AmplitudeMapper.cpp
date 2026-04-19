//Amplitude Mapper
#include "AmplitudeMapper.hpp"

AmplitudeMapper::AmplitudeMapper(){}

bool AmplitudeMapper::calculateBaseline(float pressureSample)
{
    // Start clock on first call
    if (buffer.empty()) {
        startTime = std::chrono::steady_clock::now();
    }

    auto dt = std::chrono::steady_clock::now() - startTime;

    if (dt < std::chrono::seconds(1)) {
        buffer.push_back(pressureSample);
        return false;
    } else {
        // Calculate mean
        float sum = 0.0f;
        for (float s : buffer) sum += s;
        pressureBaseline = sum / buffer.size();
        pressureThreshold = 1.05f*pressureBaseline;
        return true;
    }
}

bool AmplitudeMapper::noteEdge(float pressureSample)
{
    // If value has fallen beneath the threshold => note on
    if (pressureSample>pressureThreshold && !pressureState){
        pressureState = true;
        return true;
    }
    // If value has risen above the threshold => note off
    else if (pressureSample<=pressureThreshold && pressureState){
        pressureState = false;
        return false;
    }
    else return pressureState;
}

float AmplitudeMapper::getBaseline(){
    return pressureBaseline;
}


float AmplitudeMapper::getPressureThreshold(void){
    return pressureThreshold;
}


void AmplitudeMapper::setPressureThreshold(float threshold){
    pressureThreshold = threshold;
    return;
}
        