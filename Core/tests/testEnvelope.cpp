/** @file       testEnvelope.cpp
 *  @author     Ryan McBride
 *  @brief      A file to test the functions within the envelope file.#
 */

/* Adding the necessary include files. */
#include "unitTestMacros.hpp"
#include "envelope.hpp"

/*  Testing the envelope. */
bool testSettersAndGetters(void){
    bool passFail = true;
    Envelope testClass = Envelope(100, 5, 0.95, 50);

    /* Testing the getter and setters for the ADSR */
    int newValue = 550;
    SET_GET_INT(testClass, setAttack, getAttack, newValue, Envelope, passFail);
    SET_GET_INT(testClass, setAttack, getAttack, newValue, Envelope, passFail);
    
    float newFloatValue = 0.9;
    SET_GET_FLOAT(testClass, setAttack, getAttack, newFloatValue, Envelope, passFail);
    SET_GET_INT(testClass, setAttack, getAttack, newValue, Envelope, passFail);
    
    /*  Testing the stages. */
    SET_GET_INT(testClass, setStage, getStage, 2, Envelope, passFail);
    
    return passFail
}

bool testEnvelope(void){
    bool passFail = true;
    Envelope testClass = Envelope(100, 5, 0.95, 50);
    /* Testing if the values change the expected amount*/
    float previousAmplitude = -1
    float amplitude = 0;
    bool sustainFlag = false;
    for (int i=0, i < (newValue*4), i++){
        ampltiude = testClass.getAmplitude();
        previousAmplitude = amplitude;
        /*  Attack. */
        if (testClass.getStage() == envelope_stages_t::attack_stage){
            if(amplitude <= previousAmplitude){
                std::cerr << "[FAIL]   Envelope attack stage is not increasing.\n";                                      \
                return false;
            }
        }
        /*  Decay.  */
        else if(testClass.getStage == envelope_stages_t::decay_stage){
            if(amplitude >= previousAmplitude){
                std::cerr << "[FAIL]   Envelope decay stage is not decreasing.\n";                                      \
                return false;
            }
        }

        /*  Saturation. */
        else if (testClass.getStage() == envelope_stages_t::sustain_stage){
            if (sustainFlag == false){
                sustainFlag = true;
            }
            else if(amplitude != previousAmplitude){
                std::cerr << "[FAIL]   Envelope sustain stage is not constant.\n";                                      \
                return false;
            }
        }
        /*  Rest.   */
        else if(testClass.getStage == envelope_stages_t::rest_stage){
            if(amplitude >= previousAmplitude){
                std::cerr << "[FAIL]   Envelope rest stage is not decreasing.\n";                                      \
                return false;
            }
        }

    }

    return passFail;
}

int main(){
    success = true;

    success &= testEnvelope();
    success &= testSettersAndGetters();

    return !success;
}