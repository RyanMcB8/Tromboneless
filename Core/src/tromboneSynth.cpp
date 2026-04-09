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
  attack_ms = attack;
  attack = (1000 * attack)/sampleRate;

  decay_ms = decay;
  decay = (1000 * decay)/sampleRate;

  rest_ms = rest;
  rest = (1000 * rest)/sampleRate;

  tromboneEnvelope.setAttack(attack);
  tromboneEnvelope.setDecay(decay);
  tromboneEnvelope.setSustain(sustain);
  tromboneEnvelope.setRest(rest);
 }

 void TromboneSynth::NewTromboneNote(Notes::Notes_t note_in, int octave_in){
    /* Checking the noise is already in the sustain, decay or attack stage*/
   if (tromboneEnvelope.getStage() <= Envelope::envelope_stages_t::sustain_stage
        && tromboneEnvelope.getStage() >= Envelope::envelope_stages_t::attack_stage){
        ChangeNote(note_in, octave_in);
     }
   else{
      StartEnvelope();
   }
 }
 
 float TromboneSynth::ReadTromboneAudio(void){
   return tromboneEnvelope.getAmplitude() * PlayingNoteWithHarmonics(nHarmonics, octave, note, time, saturation);
 }

 void TromboneSynth::EndTromboneAudio(void){
   EndEnvelope();
 }
