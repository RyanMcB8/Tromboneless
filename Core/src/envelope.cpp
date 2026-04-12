/** @file           envelope.cpp 
 *  @author         @RyanMcB8
 *  @brief          Contains the defintions of the functions and variables
 *                  needed to create an envelope.
 */

 /* Adding the necessary header files. */
 #include "envelope.hpp"
 #include <stdio.h>
 #include <algorithm>

/* ========================================================================================== */
/*                                                                                            */
/*                                      Envelope                                              */
/*                                                                                            */
/* ========================================================================================== */
Envelope::Envelope(int attack_in, int decay_in, float sustain_in, int rest_in){
    attack = attack_in;
    decay = decay_in;
    sustain = sustain_in;
    rest = rest_in;
}

float Envelope::getAmplitude(void){
    float amplitude;
    switch (stage)
    {
        case no_stage:
            amplitude = 0;
            break;
            
        case attack_stage:
            amplitude = attackFunction();
            if (counter >= attack){
                stage = decay_stage;
                counter = 0;
            }
            break;

        case decay_stage:
            amplitude = decayFunction();
            if (counter >= decay) {
                stage = sustain_stage;
                counter = 0;
            }
            break;

        case sustain_stage:
            return sustain;
            break;

        case rest_stage:
            amplitude = restFunction();
            if (sustain == counter - 1){
                stage = no_stage;
                counter = 0;
            }
            break;
        
        default:
            break;
    }
    counter ++;
    return amplitude;
}

void Envelope::startEnvelope(void){
    stage = attack_stage;
    counter = 0;
}

void Envelope::endEnvelope(void){
    stage = rest_stage;
    counter = 0;
    return;
}

void Envelope::setAttack(int attack_in){
    attack = attack_in;
}

int Envelope::getAttack(void){
    return attack;
}

void Envelope::setDecay(int decay_in){
    decay = decay_in;
}

int Envelope::getDecay(void){
    return decay;
}

void Envelope::setSustain(float sustain_in){
    sustain = clamp01(sustain_in);
}

float Envelope::getSustain(void){
    return sustain;
}

void Envelope::setRest(int rest_in){
    rest = rest_in;
}

int Envelope::getRest(void){
    return rest;
}

float Envelope::attackFunction(void){
    /*  currently this is just a linear ramp function. */
    return (float)(static_cast<float>(counter)/static_cast<float>(attack));
}

float Envelope::decayFunction(void){
    /*  currently this is just a linear ramp function. */
    return (1.0f - (float)(static_cast<float>(counter)/static_cast<float>(decay))) * (1.0f - sustain) + sustain;
}

float Envelope::restFunction(void){
    /*  currently this is just a linear ramp function. */
    return (1.0f - (float)(static_cast<float>(counter)/static_cast<float>(rest))) * (sustain);
}

float Envelope::clamp01(float value){
    float clamped = std::clamp(value, 0.0f, 1.0f);
    return clamped;
}

Envelope::envelope_stages_t Envelope::getStage(void){
    return stage;
}

void Envelope::setStage(envelope_stages_t stage_in){
    stage = stage_in;
    counter = 0;
}