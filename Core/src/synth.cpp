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

float Notes::setNote(Notes_t note, float freq){
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
            return 0.0f;
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

float Octaves::TimeDecay(float t, float saturation){
    t = Clamp01(t);
    return ((1-saturation)*(1-t) + saturation);
}

float Octaves::TimeRest(float t, float saturation){
    t = Clamp01(t);
    return (saturation*(1 - Clamp01(t*t)));
}

float Octaves::PlayingNote(int octave, Notes_t note, float time){
    return (float) cos(octaves[octave].getNote(note) * time * 2 * M_PI);
}

float Octaves::StartNote(int octave, Notes_t note, float time, float t){
    return PlayingNote(octave, note, time) * TimeAscension(t);
}

float Octaves::SaturatedNote(int octave, Notes_t note, float time, float saturation){
    return (PlayingNote(octave, note, time) * saturation);
}

float Octaves::DecayNote(int octave, Notes_t note, float time, float t, float saturation){
    return PlayingNote(octave, note, time) * TimeDecay(t, saturation);
}

float Octaves::EndNote(int octave, Notes_t note, float time, float t, float saturation){
    return PlayingNote(octave, note, time) * TimeRest(t, saturation);
}

/* ========================================================================================== */
/*                                                                                            */
/*                                 OctavesWithHarmonics                                       */
/*                                                                                            */
/* ========================================================================================== */

OctavesWithHarmonics::OctavesWithHarmonics(){

}

float OctavesWithHarmonics::StartNoteWithHarmonics(int n, int octave, Notes_t note, float time, float t){
    float outputAmplitude = 0;
    for (int i=0; i < n; i++){
        outputAmplitude += HarmonicDecay(n, octave, note) * StartNote(n, note, time, t);
    } 
    return (outputAmplitude/n);
}

float OctavesWithHarmonics::DecayNoteWithHarmonics(int n, int octave, Notes_t note, float time, float t, float saturation){
    float outputAmplitude = 0;
    for (int i=0; i < n; i++){
        outputAmplitude += HarmonicDecay(n, octave, note) * StartNote(n, note, time, t);
    } 
    return (outputAmplitude/n);
}

float OctavesWithHarmonics::PlayingNoteWithHarmonics(int n, int octave, Notes_t note, float time, float saturation){
    float outputAmplitude = 0;
    for (int i=0; i < n; i++){
        outputAmplitude += HarmonicDecay(n, octave, note) * PlayingNote(n, note, time);
    } 
    return (outputAmplitude/n);
}

float OctavesWithHarmonics::EndNoteWithHarmonics(int n, int octave, Notes_t note, float time, float t, float saturation){
    float outputAmplitude = 0;
    for (int i=0; i < n; i++){
        outputAmplitude += HarmonicDecay(n, octave, note) * EndNote(n, note, time, t, saturation);
    } 
    return (outputAmplitude/n);
}

float OctavesWithHarmonics::HarmonicDecay(int n, int octave, Notes::Notes_t note){
    return Clamp01(exp(-(n * (octave + note) * decayConstant)/100));
}

/* ========================================================================================== */
/*                                                                                            */
/*                                      Envelope                                              */
/*                                                                                            */
/* ========================================================================================== */

Envelope::Envelope(int n_in, int octave_in, Notes::Notes_t note_in, float ascendT_in,
                 float decayT_in, float saturation_in, float restT_in){
    /** Setting the initial values of the parameters. */
    n = n_in;   octave = octave_in;     note = note_in;     ascendT = ascendT_in;
    decayT = decayT_in;     saturation = saturation_in;     restT = restT_in;


}

void Envelope::StartEnvelope(){
    /* Determining the time at the start of the envelope. */
    deltaTime.setStartTime();
    currentSynthState = attackState;
    return;
}

void Envelope::ChangeNote(Notes::Notes_t note_in, int octave_in){
    note = note_in;
    octave = octave_in;
    return;
}

float Envelope::GetAmplitude(){
    deltaTime.setCurrentTime();
    float time = deltaTime.getDifference();

    /* Checking if the note has been stopped. */
    if (currentSynthState == noSoundState){
        return 0;
    }

    else if(!ending){
        /* The sound is still in the ascent range. */
        if (ascendT >= time){
            currentSynthState = attackState;
            float t = 1 - std::min((ascendT/time), (float)1);
            return StartNoteWithHarmonics(n, octave, note, time, t);
        }
        
        /* The sound is in the decay stage. */
        else if ((ascendT+decayT) >= time){
            currentSynthState = decayState;
            float t = 1 - (decayT/(time-ascendT));
            return DecayNoteWithHarmonics(n, octave, note, time, t, saturation);
        }

        /* The sound is still in the saturation range. */
        else if ((ascendT+decayT) < time){
            currentSynthState = sustainState;
            return PlayingNoteWithHarmonics(n, octave, note, time, saturation);
        }
    }
    else{
        /* The system is now nearing the end. */
        float t = (float)(endTime.getDifference() / restT);
        currentSynthState = restState;
        if (t >= 1){
            ending = 0;
            currentSynthState = noSoundState;
        }
        return EndNoteWithHarmonics(n, octave, note, time, t, saturation);
    }
}

void Envelope::EndEnvelope(){
    ending = 1;
    //gettimeofday(&endTime, NULL);

}
void Envelope::KillEnvelope(){
    this->~Envelope();
}

struct timeval Envelope::getStartTime(){
    return deltaTime.getStartTime();
}

float Envelope::getTimeDifference(){
    return deltaTime.getDifference();
}

float Envelope::getAttackDecayTime(){
    return (float)(ascendT + decayT);
}

void Envelope::setNote(Notes_t note_in){
    note = note_in;
    return;
}

void Envelope::setOctave(int octave_in){
    octave = octave_in;
    return;
}

/* ========================================================================================== */
/*                                                                                            */
/*                                      DeltaTime                                             */
/*                                                                                            */
/* ========================================================================================== */

DeltaTime::DeltaTime(){
    gettimeofday(&startTime, NULL);
}

void DeltaTime::setStartTime(){
    gettimeofday(&startTime, NULL);
}

void DeltaTime::resetStartTime(){
    startTime.tv_sec = 0;
    startTime.tv_usec = 0;
}

void DeltaTime::setCurrentTime(){
    gettimeofday(&currentTime, NULL);
}

struct timeval DeltaTime::getStartTime(){
    return startTime;
}

float DeltaTime::getDifference(){
    float delta = ((currentTime.tv_sec - startTime.tv_sec)/1e6);
    delta += ((currentTime.tv_usec - startTime.tv_usec));
    return delta;
}
