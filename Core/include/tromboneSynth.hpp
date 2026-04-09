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
 #include "envelope.hpp"

 class TromboneSynth    :   public OctavesWithHarmonics
 {
    public:
        /** @brief                  The constructor function for the `TromboneSynth`
         *                          class.
         *  @param  sampleRate      The sample rate which will be used to measure
         *                          the amplitude of the sound wave being synthesised.
         */
        TromboneSynth(int sampleRate, int attack, int decay, float sustain, int rest);

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


        
    private:
        /* Initialising the envelope class which will be used to create the various
        different noises for the trombone. This sets the sound of the trombone
        permanently. The ADSR values may need to be adjusted but this should sound
        brassy. */
        Envelope tromboneEnvelope = Envelope(100, 10, 0.95, 50);
        float attack_ms;        /*  The attack time in milliseconds of the envelope. */
        float decay_ms;         /*  The decay time in milliseconds of the envelope. */
        float rest_ms;          /*  The rest time in milliseconds of the envelope. */
        float saturation;       /*  The saturation level of the envelope. */

        /*  synth   */
        int nHarmonics = 2;     /*  The number of harmonics which will be present in the final signal. */
        int octave = 2;         /*  The octave which the note will be present in. */
        Notes::Notes_t note;    /*  The note which will be played. */
 };