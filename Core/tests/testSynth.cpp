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

    std::cout << "[PASS] Octaves passed all tests. \n";
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
    if ((0.99 > harmonics.HarmonicDecay(0)) ||
        (1.01 < harmonics.HarmonicDecay(0))){
        std::cerr << "[FAIL] Error with HarmonicDecay() function.\n      Does not have a maxmimum of 1 at n=0." << "\n";
        return false;
    }
    if (0.00 < harmonics.HarmonicDecay(10e3) && 0.01 > harmonics.HarmonicDecay(10e3)){
        std::cerr << "[FAIL] Error with HarmonicDecay() function.\n      Does not have a minimum approach 0 at n=10e3." << "\n";
        return false;
    }
    

    std::cout << "[PASS] OctavesWithHarmonics passed all tests. \n";
    return true;
}

int main() {
    bool success = true;

    /* Calling upon the test funtions to run. */
    success &= TestNotes();
    success &= TestOctavesWithHarmonics();

    /* Ternary method for finding if all test functions have passed the tests. */
    return (success) ? 0 : 1;
}