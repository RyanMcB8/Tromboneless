/** @file           testMidiCoordinator.cpp
 *  @author         Aidan McIntosh
 *  @brief          A file which defines functions to test the functionality
 *                  of the methods within the MidiCoordinator class.
 */

/* Adding the necessary include files. */
#include "unitTestMacros.hpp"
#include "MidiCoordinator.hpp"
#include "audioRender.hpp"

#include <iostream>

bool testMidiCoordinator(void){
    bool passFail = true;

    AudioRender testSynth(true);
    MidiCoordinator testClass(testSynth, true);

    int callbackCount = 0;

    testClass.RegisterCallback(
        [&](const MidiMessage&){
            callbackCount++;
        }
    );

    /* Force external mode so behaviour is observable through callback count. */
    testClass.setDevice(true);

    /* ============================== Test 1: PressureEdge(true) with no note should do nothing ============================== */
    callbackCount = 0;
    testClass.PressureEdge(true);

    if (callbackCount != 0){
        std::cerr << "  [FAIL]     PressureEdge(true) with no valid note sent messages\n";
        passFail &= false;
    }

    /* ============================== Test 2: Starting a note from IDLE should send bend, expr, and noteOn ============================== */
    callbackCount = 0;
    testClass.ChangeNote(60);
    testClass.PressureEdge(true);

    if (callbackCount != 3){
        std::cerr << "  [FAIL]     PressureEdge(true) from IDLE did not send 3 messages\n";
        passFail &= false;
    }

    /* ============================== Test 3: Repeating same bend while playing should not resend ============================== */
    callbackCount = 0;
    testClass.setBend(4000);

    if (callbackCount != 1){
        std::cerr << "  [FAIL]     setBend() did not send exactly one message for new bend\n";
        passFail &= false;
    }

    callbackCount = 0;
    testClass.setBend(4000);

    if (callbackCount != 0){
        std::cerr << "  [FAIL]     setBend() resent duplicate bend value\n";
        passFail &= false;
    }

    /* ============================== Test 4: Changing note while playing should send noteOff and noteOn ============================== */
    callbackCount = 0;
    testClass.ChangeNote(62);

    if (callbackCount != 2){
        std::cerr << "  [FAIL]     ChangeNote() while PLAYING did not send noteOff and noteOn\n";
        passFail &= false;
    }

    /* ============================== Test 5: Repeating same note while playing should do nothing ============================== */
    callbackCount = 0;
    testClass.ChangeNote(62);

    if (callbackCount != 0){
        std::cerr << "  [FAIL]     ChangeNote() resent messages for unchanged note\n";
        passFail &= false;
    }

    /* ============================== Test 6: PressureEdge(false) while playing should send noteOff ============================== */
    callbackCount = 0;
    testClass.PressureEdge(false);

    if (callbackCount != 1){
        std::cerr << "  [FAIL]     PressureEdge(false) while PLAYING did not send noteOff\n";
        passFail &= false;
    }

    /* ============================== Test 7: ChangeNote() while IDLE should not send anything ============================== */
    callbackCount = 0;
    testClass.ChangeNote(65);

    if (callbackCount != 0){
        std::cerr << "  [FAIL]     ChangeNote() while IDLE sent messages\n";
        passFail &= false;
    }

    /* ============================== Test 8: Bend should clamp into valid range ============================== */
    testClass.PressureEdge(true); /* start playing again with latest note */
    callbackCount = 0;
    testClass.setBend(-50);

    if (callbackCount != 1){
        std::cerr << "  [FAIL]     setBend() with low out-of-range value did not clamp/send correctly\n";
        passFail &= false;
    }

    callbackCount = 0;
    testClass.setBend(9000);

    if (callbackCount != 1){
        std::cerr << "  [FAIL]     setBend() with high out-of-range value did not clamp/send correctly\n";
        passFail &= false;
    }

    return passFail;
}

int main(){
    bool success = true;

    success &= testMidiCoordinator();

    return !success;
}