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
    int newValue = 5;
    SET_GET_INT(testClass, setAttack, getAttack, newValue, Envelope, passFail);
    SET_GET_INT(testClass, setDecay, getDecay, newValue, Envelope, passFail);
    
    float newFloatValue = 0.9;
    SET_GET_FLOAT(testClass, setSustain, getSustain, newFloatValue, Envelope, passFail);
    SET_GET_INT(testClass, setRest, getRest, newValue, Envelope, passFail);
    
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
    
    std::cout << "[PASS] Envelope passed all set/get tests. \n";
    return passFail;
}

bool testEnvelope(void){
    bool passFail = true;
    Envelope testClass = Envelope(100, 100, 0.95, 100);
    /* Testing if the values change the expected amount*/
    float amplitude = 0;
    bool decayFlag = false;
    bool sustainFlag = false;

    testClass.startEnvelope();
    float previousAmplitude = testClass.getAmplitude()-0.01f;
    for (int i=1; i < (100*4); i++){
        amplitude = testClass.getAmplitude();

        if(i == 300){
            testClass.endEnvelope();
        }

        /*  Attack. */
        if (testClass.getStage() == Envelope::envelope_stages_t::attack_stage){
            if(amplitude <= previousAmplitude){
                std::cerr << "[FAIL]   Envelope attack stage is not increasing.\n";     
                previousAmplitude = amplitude;                         
                return false;
            }
            previousAmplitude = amplitude;
        }
        /*  Decay.  */
        else if(testClass.getStage() == Envelope::envelope_stages_t::decay_stage){
            if (decayFlag == false){
                decayFlag = true;
            }
            else if(amplitude > previousAmplitude){
                std::cerr << "[FAIL]   Envelope decay stage is not decreasing.\n";    
                previousAmplitude = amplitude;                                  
                return false;
            }
            previousAmplitude = amplitude;
        }

        /*  Saturation. */
        else if (testClass.getStage() == Envelope::envelope_stages_t::sustain_stage){
            if (sustainFlag == false){
                sustainFlag = true;
            }
            else if(amplitude != previousAmplitude){
                std::cerr << "[FAIL]   Envelope sustain stage is not constant.\n";
                previousAmplitude = amplitude;
                return false;
            }
            previousAmplitude = amplitude;
        }
        /*  Rest.   */
        else if(testClass.getStage() == Envelope::envelope_stages_t::rest_stage){
            if(amplitude > previousAmplitude){
                std::cerr << "[FAIL]   Envelope rest stage is not decreasing.\n";
                previousAmplitude = amplitude;
                return false;
            }
            previousAmplitude = amplitude;
        }
        else{
            previousAmplitude = amplitude;
            
        }

    }
    
    std::cout << "[PASS] Envelope passed all ADSR tests. \n";
    return passFail;
}

int main(){
    bool success = true;

    success &= testSettersAndGetters();
    success &= testEnvelope();

    return !success;
}