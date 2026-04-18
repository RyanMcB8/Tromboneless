    /** @file       tromboneSynth.cpp
     *  @author     Ryan McBride
     *  @brief      A file to define the synthesiser parameters and functions
     *              for the trombone specifically using the classes and methods
     *              declared in the `synth.hpp` file.
     */

 /* Adding the necessary header files to be included. */
 #include "tromboneSynth.hpp"
 #include <algorithm>
 #include <iostream>

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

void TromboneSynth::updatePitchCache()
{
    cachedFrequency = getAdjustedFrequency();
    cachedPhaseIncrement = 2.0f * static_cast<float>(M_PI) * cachedFrequency / static_cast<float>(sampleRate);
    pitchCacheDirty = false;
}

 void TromboneSynth::StartTromboneNote(Notes::Notes_t note_in, int octave_in){
    note = note_in;
    octave = octave_in;
    pitchCacheDirty = true;
    tromboneEnvelope.startEnvelope();
 }

void TromboneSynth::ChangeTromboneNote(Notes::Notes_t note_in, int octave_in){
    note = note_in;
    octave = octave_in;
    pitchCacheDirty = true;
}
 
float TromboneSynth::ReadTromboneAudio(void){
    if (pitchCacheDirty) {
        updatePitchCache();
    }

    phase += cachedPhaseIncrement;
    if (phase >= 2.0f * static_cast<float>(M_PI))
    {
        phase = std::fmod(phase, 2.0f * static_cast<float>(M_PI));
    }

    return tromboneEnvelope.getAmplitude() * PlayingFrequencyWithHarmonics(nHarmonics, phase);
}

 void TromboneSynth::StopTromboneNote(void){
   tromboneEnvelope.endEnvelope();
 }

void TromboneSynth::NewTromboneNoteMIDI(int MIDINote){
    MIDINote = std::clamp(MIDINote, 0, 127);

    const int nNotes = 12;

    note = static_cast<Notes::Notes_t>(MIDINote % nNotes);
    octave = (MIDINote / nNotes) - 1;
    octave = std::clamp(octave, 0, nOctaves - 1);

    pitchCacheDirty = true;
}

void TromboneSynth::HandleMIDINoteOn(){
    std::cout << "int_synth Note On" << std::endl;
    StartTromboneNote(note, octave);
};

void TromboneSynth::HandleMIDINoteOff(){
    std::cout << "int_synth Note Off" << std::endl;
    StopTromboneNote();
};

void TromboneSynth::HandleMIDIPitchBend(int midipitchbend){
    std::cout << "int_synth Pitch Bend" << std::endl;
    setPitchBend(midipitchbend);
};

void TromboneSynth::HandleMIDINoteChange(int new_note){
    std::cout << "int_synth Note Change" << std::endl;
    NewTromboneNoteMIDI(new_note);
};


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
    pitchCacheDirty = true;
}

int TromboneSynth::getPitchBend(void){
    return pitchBend;
}

float TromboneSynth::getAdjustedFrequency(void){

    /*  Clamp the bend to the expected one-sided range.
        8192 = no bend, 0 = maximum downward bend. */
    int clampedBend = std::clamp(pitchBend, 0, 8192);

    /*  Convert the bend into a downward semitone offset.
        8192 -> 0 semitones, 0 -> -pitchBendSpread semitones. */
    float semitoneOffset =
        -static_cast<float>(pitchBendSpread) *
        (1.0f - (static_cast<float>(clampedBend) / 8192.0f));

    /*  Split into neighbouring semitone positions for interpolation. */
    int lowerSemitone = static_cast<int>(std::floor(semitoneOffset));
    int upperSemitone = lowerSemitone + 1;

    /*  Interpolation between the two neighbouring semitone frequencies. */
    float interpolationValue = semitoneOffset - static_cast<float>(lowerSemitone);

    /*  Represent the current note and octave as a single linear note index. */
    int baseNoteIndex = (octave * nNotes) + static_cast<int>(note);

    int lowerNoteIndex = baseNoteIndex + lowerSemitone;
    int upperNoteIndex = baseNoteIndex + upperSemitone;

    /*  Helper to convert a linear note index back into octave + note. */
    auto getFrequencyFromIndex = [&](int noteIndex) {
        int octaveIndex = noteIndex / nNotes;
        int noteInOctave = noteIndex % nNotes;

        /*  Correct negative modulo behaviour. */
        if (noteInOctave < 0) {
            noteInOctave += nNotes;
            octaveIndex -= 1;
        }

        /*  Clamp octave to the available synth range. */
        octaveIndex = std::clamp(octaveIndex, 0, nOctaves - 1);

        return octaves[octaveIndex].getNote(static_cast<Notes::Notes_t>(noteInOctave));
    };

    float lowerFreq = getFrequencyFromIndex(lowerNoteIndex);
    float upperFreq = getFrequencyFromIndex(upperNoteIndex);

    /*  Linearly interpolate between the two neighbouring semitone frequencies. */
    float frequency =
        (1.0f - interpolationValue) * lowerFreq +
        interpolationValue * upperFreq;

    return frequency;
}

Envelope TromboneSynth::getEnvelope(void){
    return tromboneEnvelope;
}