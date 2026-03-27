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

#include "synth.hpp"
#include <iostream>
#define OUT std::cout

/* ========================================================================================== */
/*                                                                                            */
/*                                      DeltaTime                                             */
/*                                                                                            */
/* ========================================================================================== */
void TestDeltaTime(){
    DeltaTime deltaTime = DeltaTime();

    if ( 0 > (deltaTime.getStartTime()).tv_usec){
        OUT << "Error reading the start time. \n";
        throw;
    }

    // if (0 > deltaTime.getDifference()){
    //     OUT << "Error reading the difference in time in deltaTime";
    //     throw;
    // }

}

// deltaTime.setStartTime();


int main(){
    TestDeltaTime();
    return 0;
}
