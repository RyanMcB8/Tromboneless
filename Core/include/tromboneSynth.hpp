/** @file       tromboneSynth.hpp
 *  @author     Ryan McBride
 *  @brief      A file to declare the synthesiser parameters and functions
 *              for the trombone specifically using the classes and methods
 *              declared in the `synth.hpp` file.
 */

 /* Preventing recurssion */
 #pragma once

 /* Adding the necessary header files to be included. */
 #include "synth.hpp"

 class TromboneSynth{
    public:

        /**    @brief              A function which may be called to set the
         *                         note and octave of a new sound by the user.
         *                         If there is already a note playing, this will
         *                         stop the previous not and play this one
         *                         without phase shifting (initially).
         *     @param  note_in     The note which should start to play.
         *     @param  octave_in   The octave which the note is in.
         */
        void NewTromboneNote(Notes::Notes_t note_in, int octave_in);

        /**    @brief              A function which may be called to read the
         *                         current amplitude of the audio signal being 
         *                         generated.
         *     @retval             Returns a floating point representation of the
         *                         amplitude normalised between -1 and 1. 
         */
        float ReadTromboneAudio(void);

        /** @brief                  A function which sends the synthesiser into its
         *                          rest state.
        */
        void EndTromboneAudio(void);

        /** @brief          A function which returns the difference in time between
         *                  start and current in milliseconds.
         */
        float getTimeDifference();
        
    private:
        /* Initialising the envelope class which will be used to create the various
        different noises for the trombone. This sets the sound of the trombone
        permanently but the note and octave may be changed throughout the use of
        the device. */
        Envelope tromboneSynth = Envelope(6, 2, Notes::Notes_t::note_B, 10, 2, 0.6, 1);
 };