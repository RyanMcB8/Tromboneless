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
/*                                          Notes                                             */
/*                                                                                            */
/* ========================================================================================== */
bool TestNotes(){
    /* Creating an instance of the Notes class. */
    Notes testNote = Notes();

    /* Testing the set/get note. */
    float noteRead = testNote.getNote(Notes::Notes_t::note_C);
    if (0 >= noteRead){
        std::cerr << "[FAIL] Error with getNote() function." << "\n";
        return false;
    }
    
    testNote.setNote(Notes::Notes_t::note_C, noteRead + 15.0);
    if (testNote.getNote(Notes::Notes_t::note_C) != (noteRead + 15.0)){
        std::cerr << "[FAIL] Error with setNote() function." << "\n";
        return false;
    }
    std::cout << "[PASS] Notes passed all tests. \n";
    return true;
}

/* ========================================================================================== */
/*                                                                                            */
/*                                        Ocatves                                             */
/*                                                                                            */
/* ========================================================================================== */
bool TestOctaves(){
    /* Creating an instance of the Notes class. */
    Octaves testOctave = Octaves();

    /* Testing that the octave system is doubling each octave of the same note by default. */
    if (testOctave.octaves[0].getC() != testOctave.octaves[1].getC() || 
        testOctave.octaves[1].getC()*2 != testOctave.octaves[2].getC()){
        std::cerr << "[FAIL] Error with intialising the note frequencies." << "\n";
        return false;
    }

    /* Testing that the clamp function works. */
    if(0 > testOctave.Clamp01(-10) || 1 < testOctave.Clamp01(10)){
        std::cerr << "[FAIL] Error with Clamp01() function.\nValues returned not between 0 and 1." << "\n";
        return false;
    }

    /*  Checking that the TimeAscension function has values increasing as t increases. Proportion doesn't matter. */
    if (testOctave.TimeAscension(0.1) > testOctave.TimeAscension(0.8)){
        std::cerr << "[FAIL] Error with TimeAscension() function.\nIncreasing values of t do not return a greater value." << "\n";
        return false;
    }

    /*  Testing that the TimeDecay function has values decreasing as t is increased. Proportion doesn't matter. 
        Saturation values must be kept the same for the comparison. */
    if (testOctave.TimeDecay(0.1, 0.5) > testOctave.TimeDecay(0.8, 0.5)){
        std::cerr << "[FAIL] Error with TimeDecay() function.\nIncreasing values of t do not return a lower value." << "\n";
        return false;
    }

    /*  Testing that the TimeRest function reaches both its bounds and there is no discontinuity from saturation to rest. */
    if ((0.5 != testOctave.TimeRest(0.0, 0.5)) || (0 != testOctave.TimeRest(1.0, 0.5))){
        std::cerr << "[FAIL] Error with TimeRest() function.\nIncreasing values of t do not return a lower value." << "\n";
        return false;
    }
    
    /*  Testing that the TimeRest function has values which decrease as t increases. */
    if (testOctave.TimeRest(0.1, 0.5) > testOctave.TimeRest(0.8, 0.5)){
        std::cerr << "[FAIL] Error with TimeRest() function.\nInitial and/or final values of t do not reach the require amplitude." << "\n";
        return false;
    }

    /* Checking that the playing note function can return a cosine wave. */
    testOctave.octaves[1].setC(1);  /*  Setting this note to have a frequency of 1 for the test. */
    float noteOutput = 0;
    for (int i=0; i < 9; i++){
        noteOutput = testOctave.PlayingNote(1, Notes::Notes_t::note_C, (float)(i/8));

        /* These should be the zero crossings. */
        if (1 == i%4 || 3 == i%4){
            if (0.0 != noteOutput){
                std::cerr << "[FAIL] Error with PlayingNote() function.\nUnexpected value returned - not acting like a cosine wave." << "\n";
                return false;
            }
        }

        /* This should be the peak value. */
        else if(0 == i%8){
            if (1.0 != noteOutput){
                std::cerr << "[FAIL] Error with PlayingNote() function.\nUnexpected value returned - not acting like a cosine wave." << "\n";
                return false;
            }
        }

        /* This should be the trough value. */
        else if(4 == i%8){
            if (-1.0 != noteOutput){
                std::cerr << "[FAIL] Error with PlayingNote() function.\nUnexpected value returned - not acting like a cosine wave." << "\n";
                return false;
            }
        }

        /* This will be the side of the medium values*/
        else if (1 == i%8 || 7 == i%8){
            /* Checkling for sqrt(2)/2 allowing for a small error. */
            if (0.720 < noteOutput || 0.69 > noteOutput){
                std::cerr << "[FAIL] Error with PlayingNote() function.\nUnexpected value returned - not acting like a cosine wave." << "\n";
                return false;
            }
        }

        /* This will be the negative side of the medium values*/
        else if (1 == i%8 || 7 == i%8){
            /* Checkling for sqrt(2)/2 allowing for a small error. */
            if (-0.720 > noteOutput || -0.69 < noteOutput){
                std::cerr << "[FAIL] Error with PlayingNote() function.\nUnexpected value returned - not acting like a cosine wave." << "\n";
                return false;
            }
        }
    }

    /* Checking that the ADSR functions are using the correct function calls within them to determine the amplitude at that point. */
    for (int i=0; i < 12; i++){

    /*  A */
        if (testOctave.StartNote(0, Notes::Notes_t::note_C, i, (float)(i/20)) != 
            testOctave.PlayingNote(0, Notes::Notes_t::note_C, i) * testOctave.TimeAscension((float)(i/20))){
                std::cerr << "[FAIL] Error with StartNote() function.\nDoes not align with the products of Attack and playing note funcitons." << "\n";
                return false;
        }
    /*  D */
        if (testOctave.DecayNote(0, Notes::Notes_t::note_C, i, (float)(i/20), 0.5) != 
            testOctave.PlayingNote(0, Notes::Notes_t::note_C, i) * testOctave.TimeDecay((float)(i/20), 0.5)){
                std::cerr << "[FAIL] Error with DecayNote() function.\nDoes not align with the products of Decay and playing note funcitons." << "\n";
                return false;
        }
    /*  S */
        if(testOctave.SaturatedNote(0, Notes::Notes_t::note_C, i, 0.5) !=
            testOctave.PlayingNote(0, Notes::Notes_t::note_C, i) * 0.5){
                std::cerr << "[FAIL] Error with SaturatedNote() function.\nDoes not align with the products of saturation and playing note funcitons." << "\n";
                return false;
        }
    /*  R */
        if(testOctave.EndNote(0, Notes::Notes_t::note_C, i, (float)(i/20), 0.5) !=
            testOctave.PlayingNote(0, Notes::Notes_t::note_C, i) * testOctave.TimeRest((float)(i/20), 0.5)){
                std::cerr << "[FAIL] Error with EndNote() function.\nDoes not align with the products of Rest and playing note funcitons." << "\n";
                return false;
        }

    }


    std::cout << "[PASS] Ocatves passed all tests. \n";
    return true;
}

/* ========================================================================================== */
/*                                                                                            */
/*                                 OctavesWithHarmonics                                       */
/*                                                                                            */
/* ========================================================================================== */
bool TestOctavesWithHarmonics(){
    OctavesWithHarmonics harmonics = OctavesWithHarmonics();
    /* Testing that the harmonic decay function has a maximum of 1 and minimum approach 0. */
    if ((0.99 > harmonics.HarmonicDecay(0, 0, Notes::Notes_t::note_C)) ||
        (1.01 < harmonics.HarmonicDecay(0, 0, Notes::Notes_t::note_C))){
        std::cerr << "[FAIL] Error with HarmonicDecay() function.\n      Does not have a maxmimum of 1 at n=0." << "\n";
        return false;
    }
    if (0.00 < harmonics.HarmonicDecay(10e3, 0, Notes::Notes_t::note_C) && 0.01 > harmonics.HarmonicDecay(10e3, 0, Notes::Notes_t::note_C)){
        std::cerr << "[FAIL] Error with HarmonicDecay() function.\n      Does not have a minimum approach 0 at n=10e3." << "\n";
        return false;
    }

    /* Testing that the output values are in the correct bounds. */
    float saturation = 0.5;
    int nHarmonics = 5;
    for (int i=0; i < 20; i++){
        /* A */
        if ((1 < harmonics.StartNoteWithHarmonics(nHarmonics, 0, Notes::Notes_t::note_C, i, i/20)) ||
            (-1 > harmonics.StartNoteWithHarmonics(nHarmonics, 0, Notes::Notes_t::note_C, i, i/20))){
                std::cerr << "[FAIL] Error with StartNoteWithHarmonics() function.\n         Exits expected bounds of -1 and 1." << "\n";
                return false;
        }
        /* D */
        if ((1 < harmonics.DecayNoteWithHarmonics(nHarmonics, 0, Notes::Notes_t::note_C, i, i/20, saturation)) ||
            (-1 > harmonics.DecayNoteWithHarmonics(nHarmonics, 0, Notes::Notes_t::note_C, i, i/20, saturation))){
                std::cerr << "[FAIL] Error with DecayNoteWithHarmonics() function.\n         Exits expected bounds of -1 and 1." << "\n";
                return false;
        }
        /* S */
        if ((saturation < harmonics.PlayingNoteWithHarmonics(nHarmonics, 0, Notes::Notes_t::note_C, i, saturation)) ||
            (-saturation > harmonics.PlayingNoteWithHarmonics(nHarmonics, 0, Notes::Notes_t::note_C, i, saturation))){
                std::cerr << "[FAIL] Error with PlayingNoteWithHarmonics() function.\n       Exits expected bounds of +/-saturation." << "\n";
                std::cerr << "Expected +/-" << saturation << " but got " << harmonics.PlayingNoteWithHarmonics(nHarmonics, 0, Notes::Notes_t::note_C, i, saturation) << "\n";
                return false;
        }
        /* R */
        if ((saturation < harmonics.EndNoteWithHarmonics(nHarmonics, 0, Notes::Notes_t::note_C, i, i/20, saturation)) ||
            (-saturation > harmonics.EndNoteWithHarmonics(nHarmonics, 0, Notes::Notes_t::note_C, i, i/20, saturation))){
                std::cerr << "[FAIL] Error with EndNoteWithHarmonics() function.\n       Exits expected bounds of +/-saturation." << "\n";
                return false;
        }
    }
    

    std::cout << "[PASS] OctavesWithHarmonics passed all tests. \n";
    return true;
}

/* ========================================================================================== */
/*                                                                                            */
/*                                      Envelope                                              */
/*                                                                                            */
/* ========================================================================================== */

bool testEnvelope(){
    return true;
}

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
    std::this_thread::sleep_for(std::chrono::microseconds(10));


    /* Checking that there is a difference in time measured when a short period of time has elapsed. */
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
    success &= TestNotes();
    success &= TestOctavesWithHarmonics();

    /* Ternary method for finding if all test functions have passed the tests. */
    return (success) ? 0 : 1;
}