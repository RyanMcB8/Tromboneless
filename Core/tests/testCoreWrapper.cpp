/** @file           testCoreWrapper.cpp
 *  @author         Ryan McBride
 *  @brief          A file which defines functions to test the functionality
 *                  of the methods within the CoreWrapper class.
 */

 /* Adding the necessary include files. */
 #include "unitTestMacros.hpp"
 #include "CoreWrapper.hpp"

 bool testCoreWrapper(void){
    bool passFail = true;
    CoreWrapper testClass(true);
    if (nullptr == testClass.getEventHandler()){
        std::cerr << "  [FAIL]     getEventHandler() in CoreWrapper() failed\n";                                      
        passFail &= false; 
    }
    if (nullptr == testClass.getRtMidiSink()){
        std::cerr << "  [FAIL]     getRtMidiSink() in CoreWrapper() failed\n";                                      
        passFail &= false; 
    }
    if (nullptr == testClass.getAudioRender()){
        std::cerr << "  [FAIL]     getEventHandler() in getAudioRender() failed\n";                                      
        passFail &= false; 
    }
    if (nullptr == testClass.getAmplitudeMapper()){
        std::cerr << "  [FAIL]     getAmplitudeMapper() in CoreWrapper() failed\n";                                      
        passFail &= false; 
    }
    if (nullptr == testClass.getPitchMapper()){
        std::cerr << "  [FAIL]     getPitchMapper() in CoreWrapper() failed\n";                                      
        passFail &= false; 
    }
    if (nullptr == testClass.getMidiCoordinator()){
        std::cerr << "  [FAIL]     getMidiCoordinator() in CoreWrapper() failed\n";                                      
        passFail &= false; 
    }


    return passFail;
 }




 int main(){
    bool success = true;

    success &= testCoreWrapper();

    return !success;
 }