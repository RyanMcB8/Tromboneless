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
    Envelope::envelope_stages_t newStage = Envelope::envelope_stages_t::attack_stage;
    Envelope::envelope_stages_t original = testClass.getStage();
    if ((Envelope::envelope_stages_t)newStage == original) {   
        newStage = (Envelope::envelope_stages_t) (newStage + 1);                               
    }                                                          
    testClass.setStage(newStage);          
    Envelope::envelope_stages_t updated = testClass.getStage();                            
    if (updated != newStage || updated == original)                 
    {                                                               
        std::cerr << "[FAIL]     setStage() / getStage() in Envelope() failed\n";                                      
        passFail &= false;                                            
    }                                                               
    else{                                                           
        passFail &= true;                                             
    }                                                               
    
    return passFail;
}

bool testEnvelope(void){
    bool passFail = true;
    Envelope testClass = Envelope(100, 100, 0.95, 100);
    /* Testing if the values change the expected amount*/
    float previousAmplitude = -1;
    float amplitude = 0;
    bool sustainFlag = false;
    for (int i=0; i < (100*4); i++){
        amplitude = testClass.getAmplitude();
        previousAmplitude = amplitude;

        if(i == 300){
            testClass.endEnvelope();
        }

        /*  Attack. */
        if (testClass.getStage() == Envelope::envelope_stages_t::attack_stage){
            if(amplitude <= previousAmplitude){
                std::cerr << "[FAIL]   Envelope attack stage is not increasing.\n";                                     
                return false;
            }
        }
        /*  Decay.  */
        else if(testClass.getStage() == Envelope::envelope_stages_t::decay_stage){
            if(amplitude >= previousAmplitude){
                std::cerr << "[FAIL]   Envelope decay stage is not decreasing.\n";                                      
                return false;
            }
        }

        /*  Saturation. */
        else if (testClass.getStage() == Envelope::envelope_stages_t::sustain_stage){
            if (sustainFlag == false){
                sustainFlag = true;
            }
            else if(amplitude != previousAmplitude){
                std::cerr << "[FAIL]   Envelope sustain stage is not constant.\n";
                return false;
            }
        }
        /*  Rest.   */
        else if(testClass.getStage() == Envelope::envelope_stages_t::rest_stage){
            if(amplitude >= previousAmplitude){
                std::cerr << "[FAIL]   Envelope rest stage is not decreasing.\n";
                return false;
            }
        }

    }

    return passFail;
}

int main(){
    bool success = true;

    success &= testEnvelope();
    success &= testSettersAndGetters();

    return !success;
}