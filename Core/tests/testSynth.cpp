/** @file   testSynth.cpp
    @brief  This file contains all of the necessary unit tests
            for the functions declared within the `synth.hpp`
            header file. If there is a failure, the system
            will use the `throw` command after displaying an
            error message.
    @note   This file has a main function at the end which 
            calls upon all the functions which have been
            defined within this file.
*/

#include <iostream>
#include "synth.hpp"
#include <thread>
#include <chrono>

/* ========================================================================================== */
/*                                                                                            */
/*                                      DeltaTime                                             */
/*                                                                                            */
/* ========================================================================================== */
bool TestDeltaTime(){
    DeltaTime deltaTime = DeltaTime();

    if ( 0 > (deltaTime.getStartTime()).tv_usec){
        std::cerr << "[FAIL] Error reading the start time." << "\n";
        return false;
    }
    /* Adding a slight delay to allow the start time to be more than the end time. */
    std::this_thread::sleep_for(std::chrono::microseconds(100));


    /* Checking that the */
    if (0 > deltaTime.getDifference()){
        std::cerr << "[FAIL] Error reading the difference in time in deltaTime" << "\n";
        return false;
    }
    std::cout << "[PASS] TestDeltaTime passed all tests. \n";
    return true;

}

int main() {
    bool success = true;

    /* Calling upon the test funtions to run. */
    success &= TestDeltaTime();



    if (success) {
        return 0; 
    } else {
        return 1; 
    }
}