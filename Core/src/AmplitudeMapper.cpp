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
        pressureThreshold = gateFactor*pressureBaseline;
        return true;
    }
}

bool AmplitudeMapper::noteEdge(float pressureSample)
{
    pressureThreshold = gateFactor*pressureBaseline;
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


float AmplitudeMapper::getGateFactor(void){
    return gateFactor;
}


void AmplitudeMapper::setGateFactor(float factor){
    gateFactor = 1.0f-(0.1f-(factor/10));
    return;
}
        