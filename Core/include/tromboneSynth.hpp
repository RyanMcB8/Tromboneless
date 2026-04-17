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

 /**    @brief                      A class which may be used to create a custom synth
  *                                 with an envelope attached to it.
  */
 class TromboneSynth    :   public OctavesWithHarmonics
 {
    public:
        /** @brief                  The constructor function for the `TromboneSynth`
         *                          class.
         *  @param  sampleRate      The sample rate which will be used to measure
         *                          the amplitude of the sound wave being synthesised.
         *                          This should be passed through in Hz.
         *  @param  attack          The amount of time in ms that the attack should
         *                          last for.
         *  @param  decay           The amount of time in ms that the decay should
         *                          last for.
         *  @param  sustain         The relative amplitude that is being output at the
         *                          steady state.
         *  @param  rest            The amount of time in milliseconds that the rest
                                    stage should last for.
         */
        TromboneSynth(int sampleRate = 44100, float attack = 500.0, float decay = 50.0,
             float sustain = 0.96, float rest = 10.0);

        /**    @brief              A function which may be called to set the
         *                         note and octave of a new sound by the user.
         *                         If there is already a note playing, this will
         *                         stop the previous not and play this one
         *                         without phase shifting (initially).
         *     @param  note_in     The note which should start to play.
         *     @param  octave_in   The octave which the note is in.
         */
        void StartTromboneNote(Notes::Notes_t note_in, int octave_in);

        /** @brief                  A function that can change the current note
         *                          without needed to change the stage that the 
         *                          envelope is in.
         *  @param  note_in         The note which the synthesiser should start
         *                          to play.
         *  @param  octave_in       The octave in which the note is in.
         */
        void ChangeTromboneNote(Notes::Notes_t note_in, int octave_in);

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
        void StopTromboneNote(void);

        /** @brief                  A function which can take in the MIDI equivalent
         *                          note and map it to the Notes_t options to match
         *                          the rest of the synth interface.
         *  @param  MIDINote        The integer value between 0 and 127 representing
         *                          the note which is being played.
         */
        void NewTromboneNoteMIDI(int MIDINote);

        /** @brief                  A function to set the amount of time in milliseconds
         *                          that the attack stage takes.
         *  @param  attack_time     The time in milliseconds.
         */
        void setAttackMS(float attack_time);

        /** @brief                  A function to get the current attack time in
         *                          milliseconds.
         *  @retval                 The time in milliseconds as a floating point
         *                          value.
         */
        float getAttackMS(void);

        /** @brief                  A function to set the amount of time in milliseconds
         *                          that the decay stage takes.
         *  @param  decay_time     The time in milliseconds.
         */
        void setDecayMS(float decay_time);

        /** @brief                  A function to get the current decay time in
         *                          milliseconds.
         *  @retval                 The time in milliseconds as a floating point
         *                          value.
         */
        float getDecayMS(void);

        /** @brief                  A function to set the amount of time in milliseconds
         *                          that the rest stage takes.
         *  @param  rest_time     The time in milliseconds.
         */
        void setRestMS(float rest_time);

        /** @brief                  A function to get the current rest time in
         *                          milliseconds.
         *  @retval                 The time in milliseconds as a floating point
         *                          value.
         */
        float getRestMS(void);

        /** @brief                  A function to set the relative amplitude of the
         *                          sustain stage.
         *  @param  sustain_in      The floating point relative amplitde between 0
         *                          and 1.
         */
        void setSustain(float sustain_in);

        /** @brief                  A function to get the current sustain value.
         *  @retval                 The floating point relative amplitude
         */
        float getSustain(void);

        /** @brief                  A function to set the number of harmonics which are
         *                          being generated for every note being played.
         *  @param  n               The number of harmonics which should be added to each
         *                          note.
         */
        void setNHarmonics(int n);

        /** @brief                  A function to get the number of harmonics which are
         *                          being generated for every note being played.
         *  @retval                 The integer representation of the number of harmonics.
         */
        int getNHarmonics(void);

        /** @brief                  A function to get the current octave which the current
         *                          note is in.
         *  @retval                 The octave as an integer representation.
         */
        int getOctave(void);

        /** @brief                  A function to get the current note which is
         *                          being used.
         *  @retval                 The note of type Notes::Notes_t being played.
         */
        Notes::Notes_t getNote(void);

        /** @brief                  A function to set the pitch bend of the device.
         *  @param  bend            The integer representation between 8192 and 0 of how
         *                          much of the pitch has been bent.
         */
        void setPitchBend(int bend);

        /** @brief                  A function to get the pitch bend of the device.
         *  @retval                 The integer representation between 8192 and 0 of how
         *                          much of the pitch has been bent.
         */
        int getPitchBend(void);

        /** @brief                  A function which determines the new frequency which should
         *                          be being played depending upon the current note and octave
         *                          as well as the pitch bend being input.
         *  @retval                 The floating point representation of the frequency.
         */
        float getAdjustedFrequency(void);

    private:
        /* Initialising the envelope class which will be used to create the various
        different noises for the trombone. This sets the sound of the trombone
        permanently. The ADSR values may need to be adjusted but this should sound
        brassy. */
        Envelope tromboneEnvelope = Envelope(100, 50, 0.95f, 10);
        float attack_ms;        /*  The attack time in milliseconds of the envelope. */
        float decay_ms;         /*  The decay time in milliseconds of the envelope. */
        float rest_ms;          /*  The rest time in milliseconds of the envelope. */
        float sustain;          /*  The sustain level of the envelope. */

        float phase = 0.0f; /* Phase of oscillator wave */

        /*  synth   */
        int nHarmonics = 17;    /*  The number of harmonics which will be present in the final signal. */
        int octave = 2;         /*  The octave which the note will be present in. */
        Notes::Notes_t note = Notes::Notes::note_A;  /*  The note which will be played. */
        int pitchBend   =   8192;  /*  The MIDI message pitch bend dependent upon the position of the slider. */

        unsigned int samples = 0;
        unsigned int sampleRate = 44100;

        const unsigned int pitchBendMax = 8192;    /*  The maximum number which may be passed to the the pitch bending function by the MIDI input. */
        const unsigned int pitchBendSpread = 7;    /*  The maximum number of semitones that the pitch bend may change the playing note by*/
        const float semitoneSpread = pitchBendMax/pitchBendSpread;  /*   The number of data points between each adjacent semitone. */
        const int nNotes = 12;          /*  The number of notes per octave. */
};