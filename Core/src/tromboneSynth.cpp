/** @file       tromboneSynth.cpp
 *  @author     Ryan McBride
 *  @brief      A file to define the synthesiser parameters and functions
 *              for the trombone specifically using the classes and methods
 *              declared in the `synth.hpp` file.
 */

 /* Adding the necessary header files to be included. */
 #include "tromboneSynth.hpp"

 TromboneSynth::TromboneSynth(int sampleRate, float attack, float decay, float sustain, float rest){
    /*  Storing the ADR inputs as their time in milliseconds and then passing the N sample equivalent
        into the envelope object. */
    /* Ignoring any negative frequencies or times being input*/
    sampleRate = abs(sampleRate);
    attack_ms = abs(attack);
    attack = (abs(attack) / 1000) * sampleRate;

    decay_ms = abs(decay);
    decay = (abs(decay) / 1000) * sampleRate;

    rest_ms = abs(rest);
    rest = (abs(rest) / 1000) * sampleRate;

    tromboneEnvelope.setAttack(attack);
    tromboneEnvelope.setDecay(decay);
    tromboneEnvelope.setSustain(abs(sustain));
    tromboneEnvelope.setRest(rest);
 }

 void TromboneSynth::NewTromboneNote(Notes::Notes_t note_in, int octave_in){
    /* Checking the noise is already in the sustain, decay or attack stage*/
    if (tromboneEnvelope.getStage() <= Envelope::envelope_stages_t::sustain_stage
        && tromboneEnvelope.getStage() >= Envelope::envelope_stages_t::attack_stage){
        ChangeNote(note_in, octave_in);
    }
    else if (tromboneEnvelope.getStage() == Envelope::envelope_stages_t::rest_stage){
        float currentAmplitude = tromboneEnvelope.getAmplitude();
        if (currentAmplitude > (sustain - currentAmplitude)){
            /*  The amplitude is closer to sustain than to 0 so keep the signal at sustain
                This will reduce the sharp difference between different volume level.
                To make this better, there should be an interpolated stage that would be
                able to map the values so it sounds smoother. */
            tromboneEnvelope.setStage(Envelope::envelope_stages_t::sustain_stage);
        }
        else{
            tromboneEnvelope.setStage(Envelope::envelope_stages_t::attack_stage);
        }
    }
    else{
        /*  This will put the trombone into the attack mode if it is already inactive. */
        tromboneEnvelope.setStage(Envelope::envelope_stages_t::attack_stage);
    }
 }
 
 float TromboneSynth::ReadTromboneAudio(void){
    samples += 1;
    samples = samples % sampleRate;
   return tromboneEnvelope.getAmplitude() * PlayingNoteWithHarmonics(nHarmonics, octave, note, (float)(samples/sampleRate), sustain);
 }

 void TromboneSynth::EndTromboneAudio(void){
   tromboneEnvelope.endEnvelope();
 }

 void TromboneSynth::NewTromboneNoteMIDI(int MIDINote){
    int nNotes = 12;            /*  The number of notes per octave. */
    int octaveOffset = 0;       /*  set the octave which matches the note of 0. Set to octave 0.*/
    octave = (int)(MIDINote/nNotes) + octaveOffset;
    note = (Notes::Notes_t)(MIDINote % nNotes);
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