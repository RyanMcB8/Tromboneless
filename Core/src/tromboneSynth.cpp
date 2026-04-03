/** @file       tromboneSynth.cpp
 *  @author     Ryan McBride
 *  @brief      A file to define the synthesiser parameters and functions
 *              for the trombone specifically using the classes and methods
 *              declared in the `synth.hpp` file.
 */

 /* Adding the necessary header files to be included. */
 #include "tromboneSynth.hpp"


 void TromboneSynth::NewTromboneNote(Notes::Notes_t note_in, int octave_in){
   if ((tromboneSynth.getTimeDifference()) < tromboneSynth.getAttackDecayTime()){
        tromboneSynth.ChangeNote(note_in, octave_in);
     }
   else{
      tromboneSynth.setNote(note_in);
      tromboneSynth.setOctave(octave_in);
      tromboneSynth.StartEnvelope();
   }
 }
 
 float TromboneSynth::ReadTromboneAudio(void){
   return tromboneSynth.GetAmplitude();
 }

 void TromboneSynth::EndTromboneAudio(void){
   tromboneSynth.EndEnvelope();
 }

 float TromboneSynth::getTimeDifference(){
    return tromboneSynth.getTimeDifference();
}