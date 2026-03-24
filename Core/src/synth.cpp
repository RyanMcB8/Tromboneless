/** @file       synth.cpp
 *  @author     @RyanMcB8
 *  @brief      C file which contains the defintions for the 
 *              functions declared in the `synth.hpp` file.
 */

 /* Adding the necessary header files. */
#include "synth.hpp"

/* ========================================================================================== */
/*                                                                                            */
/*                                         Notes                                              */
/*                                                                                            */
/* ========================================================================================== */

Notes::Notes(float C_in, float Db_in, float D_in, float Eb_in, float E_in, float F_in,
    float Gb_in, float G_in, float Ab_in, float A_in, float Bb_in, float B_in){
        C = C_in;   Db = Db_in;     D = D_in;       Eb = Eb_in;
        E = E_in;   F = F_in;       Gb = Gb_in;     G = G_in;
        Ab = Ab_in; A = A_in;       Bb = Bb_in;     B = B_in;   
}

void Notes::setC(float frequency){
    C = frequency;
}

void Notes::setC_sharp(float frequency){
    Db = frequency;
}

void Notes::setDb(float frequency){
    Db = frequency;
}

void Notes::setD(float frequency){
    D = frequency;
}

void Notes::setD_sharp(float frequency){
    Eb = frequency;
}

void Notes::setEb(float frequency){
    Eb = frequency;
}

void Notes::setE(float frequency){
    E = frequency;
}

void Notes::setF(float frequency){
    F = frequency;
}

void Notes::setF_sharp(float frequency){
    Gb = frequency;
}

void Notes::setGb(float frequency){
    Gb = frequency;
}

void Notes::setG(float frequency){
    G = frequency;
}

void Notes::setG_sharp(float frequency){
    Ab = frequency;
}

void Notes::setAb(float frequency){
    Ab = frequency;
}

void Notes::setA(float frequency){
    A = frequency;
}

void Notes::setA_sharp(float frequency){
    Bb = frequency;
}

void Notes::setBb(float frequency){
    Bb = frequency;
}

void Notes::setB(float frequency){
    B = frequency;
}

void Notes::setNote(Notes_t note, float freq){
    switch(note){
        case note_C:
            setC(freq);

        case note_Db:
            setDb(freq);

        case note_D:
            setD(freq);

        case note_Eb:
            setEb(freq);

        case note_E:
            setE(freq);

        case note_F:
            setF(freq);

        case note_Gb:
            setGb(freq);

        case note_G:
            setG(freq);

        case note_Ab:
            setAb(freq);

        case note_A:
            setA(freq);

        case note_Bb:
            setBb(freq);

        case note_B:
            setB(freq);

        default:
            return;
    }
}

float Notes::getC(void){
    return C;
}

float Notes::getC_sharp(void){
    return Db;
}

float Notes::getDb(void){
    return Db;
}

float Notes::getD(void){
    return D;
}

float Notes::getD_sharp(void){
    return Eb;
}

float Notes::getEb(void){
    return Eb;
}

float Notes::getE(void){
    return E;
}

float Notes::getF(void){
    return F;
}

float Notes::getF_sharp(void){
    return Gb;
}

float Notes::getGb(void){
    return Gb;
}

float Notes::getG(void){
    return G;
}

float Notes::getG_sharp(void){
    return Ab;
}

float Notes::getAb(void){
    return Ab;
}

float Notes::getA(void){
    return A;
}

float Notes::getA_sharp(void){
    return Bb;
}

float Notes::getBb(void){
    return Bb;
}

float Notes::getB(void){
    return B;
}

float Notes::getNote(Notes_t note){
    switch(note){
        case note_C:
            return getC();
        case note_Db:
            return getDb();

        case note_D:
            return getD();

        case note_Eb:
            return getEb();

        case note_E:
            return getE();

        case note_F:
            return getF();

        case note_Gb:
            return getGb();

        case note_G:
            return getG();

        case note_Ab:
            return getAb();

        case note_A:
            return getA();

        case note_Bb:
            return getBb();

        case note_B:
            return getB();
        default:
            return 0.0f;
    }
}

/* ========================================================================================== */
/*                                                                                            */
/*                                        Octaves                                             */
/*                                                                                            */
/* ========================================================================================== */

Octaves::Octaves(){
    /* Looping through each of the notes and creating the next octave
    value based upon the previous octaves frequency at the same note. */
    for (int i=1; i < nOctaves; i++){
        octaves[i].setC (octaves[i-1].getC () + octaves[i-1].getC()); 
        octaves[i].setDb(octaves[i-1].getDb() + octaves[i-1].getDb());
        octaves[i].setD (octaves[i-1].getD() + octaves[i-1].getD()); 
        octaves[i].setEb(octaves[i-1].getEb() + octaves[i-1].getEb());
        octaves[i].setE (octaves[i-1].getE() + octaves[i-1].getE()); 
        octaves[i].setF (octaves[i-1].getF() + octaves[i-1].getF()); 
        octaves[i].setGb(octaves[i-1].getGb() + octaves[i-1].getGb());
        octaves[i].setG (octaves[i-1].getG() + octaves[i-1].getG()); 
        octaves[i].setAb(octaves[i-1].getAb() + octaves[i-1].getAb());
        octaves[i].setA (octaves[i-1].getA() + octaves[i-1].getA()); 
        octaves[i].setBb(octaves[i-1].getBb() + octaves[i-1].getBb());
        octaves[i].setB (octaves[i-1].getB() + octaves[i-1].getB()); 
    }
}

float Octaves::Clamp01(float value){
    return std::max(std::min(value, 1.0f), 0.0f);
}

float Octaves::TimeAscension(float t){
    return Clamp01(t);
}