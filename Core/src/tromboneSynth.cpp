/** @file       tromboneSynth.cpp
 *  @author     Ryan McBride
 *  @brief      A file to define the synthesiser parameters and functions
 *              for the trombone specifically using the classes and methods
 *              declared in the `synth.hpp` file.
 */

 /* Adding the necessary header files to be included. */
 #include "tromboneSynth.hpp"
 #include <algorithm>

 TromboneSynth::TromboneSynth(int sampleRate_in, float attack, float decay, float sustain, float rest){
    /*  Storing the ADR inputs as their time in milliseconds and then passing the N sample equivalent
        into the envelope object. */
    /* Ignoring any negative frequencies or times being input*/

    using std::abs;
    sampleRate = abs(sampleRate_in);

    attack_ms = abs(attack);
    attack = (abs(attack) / 1000.0) * float(sampleRate);

    decay_ms = abs(decay);
    decay = (abs(decay) / 1000.0) * sampleRate;

    rest_ms = abs(rest);
    rest = (abs(rest) / 1000.0) * sampleRate;

    tromboneEnvelope.setAttack(attack);
    tromboneEnvelope.setDecay(decay);
    tromboneEnvelope.setSustain(abs(sustain));
    tromboneEnvelope.setRest(rest);
 }

 void TromboneSynth::StartTromboneNote(Notes::Notes_t note_in, int octave_in){
    note = note_in;
    octave = octave_in;
    tromboneEnvelope.startEnvelope();
 }

 void TromboneSynth::ChangeTromboneNote(Notes::Notes_t note_in, int octave_in){
    note = note_in;
    octave = octave_in;
 }
 
 /* Change this to be based on frequency with the base note and using the pitch bend.*/
 float TromboneSynth::ReadTromboneAudio(void){
    //samples = samples % sampleRate;
    samples += 1;
    float time_increment = 1.0f / static_cast<float>(sampleRate);
    float accum_time = time_increment * static_cast<float>(samples);
    return tromboneEnvelope.getAmplitude() * PlayingFrequencyWithHarmonics(nHarmonics, getAdjustedFrequency(), accum_time);
 }

 void TromboneSynth::StopTromboneNote(void){
   tromboneEnvelope.endEnvelope();
 }

void TromboneSynth::NewTromboneNoteMIDI(int MIDINote){
    MIDINote = std::clamp(MIDINote, 0, 127);

    const int nNotes = 12;

    note = static_cast<Notes::Notes_t>(MIDINote % nNotes);

    // MIDI standard: C4 = 60 → octave 4
    octave = (MIDINote / nNotes) - 1;

    // clamp to available octave range
    octave = std::clamp(octave, 0, nOctaves - 1);
}

 void TromboneSynth::setAttackMS(float attack_time){
    attack_ms = attack_time;
    return;
 }

float TromboneSynth::getAttackMS(void){
    return attack_ms;
}

void TromboneSynth::setDecayMS(float decay_time){
    decay_ms = decay_time;
    return;
}

float TromboneSynth::getDecayMS(void){
    return decay_ms;
}

void TromboneSynth::setRestMS(float rest_time){
    rest_ms = rest_time;
    return;
}

float TromboneSynth::getRestMS(void){
    return rest_ms;
}

void TromboneSynth::setSustain(float sustain_in){
    sustain = sustain_in;

}

float TromboneSynth::getSustain(void){
    return sustain;
}

void TromboneSynth::setNHarmonics(int n){
    nHarmonics = n;
}

int TromboneSynth::getNHarmonics(void){
    return nHarmonics;
}

int TromboneSynth::getOctave(void){
    return octave;
}

Notes::Notes_t TromboneSynth::getNote(void){
    return note;
}

void TromboneSynth::setPitchBend(int bend){
    pitchBend = bend;
    return;
}

int TromboneSynth::getPitchBend(void){
    return pitchBend;
}

float TromboneSynth::getAdjustedFrequency(void){

    /*  Finding the relative note indices of the pitch bend notes which the true pitch bend frequency is between. */
    int rangeNoteHigh = pitchBendSpread - (int) (pitchBend / semitoneSpread);
    int rangeNoteLow = rangeNoteHigh + 1;

    /* Finding the LERP of the pitch bend between the the 2 nearest semitones. This will be between 0 and 1. */
    float interpolationValue = (float)(((pitchBend - rangeNoteHigh) % int(semitoneSpread)) / semitoneSpread);

    /* Changing the octave and notes to be repesented as one value instead of 2. */
    int initialNote = (octave * nNotes) + note;
    int highNote = initialNote - rangeNoteHigh;
    int lowNote = initialNote - rangeNoteLow;

    /*  Finding the frequencies of the chosen values.   */
    float highNoteFreq = octaves[(int)(highNote / nNotes)].getNote((Notes::Notes_t) (highNote % nNotes));
    float lowNoteFreq = octaves[(int)(lowNote / nNotes)].getNote((Notes::Notes_t) (lowNote % nNotes));

    /*  Finding the interpolated frequency of due to the pitch bend. */
    float frequency = (highNoteFreq * interpolationValue) + (lowNoteFreq * (1 - interpolationValue));
    
    return frequency;
}