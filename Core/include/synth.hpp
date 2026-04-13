/** @file       synth.hpp
 *  @author     Ryan McBride
 *  @brief      A header file containing all the necessary declarations and references to
 *              allow for the Tromboneless to synthesise the noise a trombone would make
 *              when playing the same notes.
 *  @note       The envelope class lacks set/get functionality for its initialised parameters
 *              such as the attack, decay and rest times. This should be added in the next
 *              revision to allow for the sound to be modified without needing to create a
 *              new object each time.
 */

/* Preventing recursion. */
#pragma once

/* Addition of all necessary header files. */
#include <cmath>
#include <sys/time.h>

/* ========================================================================================== */
/*                                  Class definitions                                         */
/* ========================================================================================== */

/** @brief      A class to store the frequency of the notes which may be referenced. The
 *              notes are stored in an enum `Notes_t` which allow for the higher level functions
 *              to call upon the specific note as opposed to frequency.
 */
class Notes{
    public:

        /** @brief Creation of the enumerated list of possible notes. */
        typedef enum{
            note_C,
            note_Db,
            note_D,
            note_Eb,
            note_E,
            note_F,
            note_Gb,
            note_G,
            note_Ab,
            note_A,
            note_Bb,
            note_B,
        
            /* Addition of sharp notes. */
            note_C_sharp = note_Db,
            note_D_sharp = note_Eb,
            note_F_sharp = note_Gb,
            note_G_sharp = note_Ab,
            note_A_sharp = note_Bb,
        }   Notes_t;
        
        /** @brief         Constructor for the `Notes` class to create a new 
         *                 set of notes and assign specified frequencies to
         *                 each. Each frequency is set to `Octave 0` by
         *                 default.
         *  @param  C_in   The frequency of the C key. 
         *  @param  Db_in  The frequency of the Db key. 
         *  @param  D_in   The frequency of the D key. 
         *  @param  Eb_in  The frequency of the Eb key.
         *  @param  E_in   The frequency of the E key.
         *  @param  F_in   The frequency of the F key.
         *  @param  Gb_in  The frequency of the Gb key. 
         *  @param  G_in   The frequency of the G key. 
         *  @param  Ab_in  The frequency of the Ab key.
         *  @param  A_in   The frequency of the A key.
         *  @param  Bb_in  The frequency of the Bb key.
         *  @param  B_in   The frequency of the B key.
         *  @note   This function simply sets the local frequenies
         *          to be equal to the passed in frequencies.
         * 
         */    
        Notes(float C_in    =   16.35, float Db_in   =   17.32,
            float D_in      =   18.35, float Eb_in   =   19.45,
            float E_in      =   20.60, float F_in    =   21.83,
            float Gb_in     =   23.12, float G_in    =   24.50,
            float Ab_in     =   25.96, float A_in    =   27.50,
            float Bb_in     =   29.14, float B_in    =   30.87);

        ~Notes() = default;

        /* ========================================================================================== */
        /*                               Note setters                                                 */
        /* ========================================================================================== */
        
        /** @brief A function that can change the value of the frequency that the C note plays.
         *  @param frequency The frequency which the C not should play.
         *  @note There is no return value from this function.
         */
        void setC(float frequency);

        /** @brief A function that can change the value of the frequency that the C# note plays.
         *  @param frequency The frequency which the C# not should play.
         *  @note There is no return value from this function.
         */
        void setC_sharp(float frequency);

        /** @brief A function that can change the value of the frequency that the Db note plays.
         *  @param frequency The frequency which the Db not should play.
         *  @note There is no return value from this function.
         */
        void setDb(float frequency);
        
        /** @brief A function that can change the value of the frequency that the D note plays.
         *  @param frequency The frequency which the D not should play.
         *  @note There is no return value from this function.
         */
        void setD(float frequency);

        /** @brief A function that can change the value of the frequency that the D# note plays.
         *  @param frequency The frequency which the D# not should play.
         *  @note There is no return value from this function.
         */
        void setD_sharp(float frequency);
        
        /** @brief A function that can change the value of the frequency that the Eb note plays.
         *  @param frequency The frequency which the Eb not should play.
         *  @note There is no return value from this function.
         */
        void setEb(float frequency);
        
        /** @brief A function that can change the value of the frequency that the E note plays.
         *  @param frequency The frequency which the E not should play.
         *  @note There is no return value from this function.
         */
        void setE(float frequency);
        
        /** @brief A function that can change the value of the frequency that the F note plays.
         *  @param frequency The frequency which the F not should play.
         *  @note There is no return value from this function.
         */
        void setF(float frequency);
        
        /** @brief A function that can change the value of the frequency that the F# note plays.
         *  @param frequency The frequency which the F# not should play.
         *  @note There is no return value from this function.
         */
        void setF_sharp(float frequency);
        
        /** @brief A function that can change the value of the frequency that the Gb note plays.
         *  @param frequency The frequency which the Gb not should play.
         *  @note There is no return value from this function.
         */
        void setGb(float frequency);
        
        /** @brief A function that can change the value of the frequency that the G note plays.
         *  @param frequency The frequency which the G not should play.
         *  @note There is no return value from this function.
         */
        void setG(float frequency);
        
        /** @brief A function that can change the value of the frequency that the G# note plays.
         *  @param frequency The frequency which the G# not should play.
         *  @note There is no return value from this function.
         */
        void setG_sharp(float frequency);
        
        /** @brief A function that can change the value of the frequency that the Ab note plays.
         *  @param frequency The frequency which the Ab not should play.
         *  @note There is no return value from this function.
         */
        void setAb(float frequency);

        /** @brief A function that can change the value of the frequency that the A note plays.
         *  @param frequency The frequency which the A not should play.
         *  @note There is no return value from this function.
         */
        void setA(float frequency);

        /** @brief A function that can change the value of the frequency that the A# note plays.
         *  @param frequency The frequency which the A# not should play.
         *  @note There is no return value from this function.
         */
        void setA_sharp(float frequency);
        
        /** @brief A function that can change the value of the frequency that the Bb note plays.
         *  @param frequency The frequency which the Bb not should play.
         *  @note There is no return value from this function.
         */
        void setBb(float frequency);
        
        /** @brief A function that can change the value of the frequency that the B note plays.
         *  @param frequency The frequency which the B not should play.
         *  @note There is no return value from this function.
         */
        void setB(float frequency);
        
        /** @brief          An alternative function which can allow for the user to alter the
         *                  frequency of a specific note based upon the Notes_t enumerated
         *                  values rather than specifying a function call.
         *  @param  note    The note of which the user wants to change the frequency of.
         *  @param  freq    The frequency which the specified note should be changed to.
          */
        float setNote(Notes_t note, float freq);

        /* ========================================================================================== */
        /*                               Note getters                                                 */
        /* ========================================================================================== */

        /** @brief A function that returns the frequency of the C note for the specified object.
         *  @retval The frequency of the C note as a floating point value. 
         */
        float getC(void);

        /** @brief A function that returns the frequency of the C# note for the specified object.
         *  @retval The frequency of the C# note as a floating point value. 
         */
        float getC_sharp(void);

        /** @brief A function that returns the frequency of the Db note for the specified object.
         *  @retval The frequency of the Db note as a floating point value. 
         */
        float getDb(void);

        /** @brief A function that returns the frequency of the D note for the specified object.
         *  @retval The frequency of the D note as a floating point value. 
         */
        float getD(void);

        /** @brief A function that returns the frequency of the D# note for the specified object.
         *  @retval The frequency of the D# note as a floating point value. 
         */
        float getD_sharp(void);

        /** @brief A function that returns the frequency of the Eb note for the specified object.
         *  @retval The frequency of the Eb note as a floating point value. 
         */
        float getEb(void);

        /** @brief A function that returns the frequency of the E note for the specified object.
         *  @retval The frequency of the E note as a floating point value. 
         */
        float getE(void);

        /** @brief A function that returns the frequency of the F note for the specified object.
         *  @retval The frequency of the F note as a floating point value. 
         */
        float getF(void);

        /** @brief A function that returns the frequency of the F# note for the specified object.
         *  @retval The frequency of the F# note as a floating point value. 
         */
        float getF_sharp(void);

        /** @brief A function that returns the frequency of the Gb note for the specified object.
         *  @retval The frequency of the Gb note as a floating point value. 
         */
        float getGb(void);

        /** @brief A function that returns the frequency of the G note for the specified object.
         *  @retval The frequency of the G note as a floating point value. 
         */
        float getG(void);

        /** @brief A function that returns the frequency of the G# note for the specified object.
         *  @retval The frequency of the G# note as a floating point value. 
         */
        float getG_sharp(void);
        
        /** @brief A function that returns the frequency of the Ab note for the specified object.
         *  @retval The frequency of the Ab note as a floating point value. 
         */
        float getAb(void);

        /** @brief A function that returns the frequency of the A note for the specified object.
         *  @retval The frequency of the A note as a floating point value. 
         */
        float getA(void);

        /** @brief A function that returns the frequency of the A# note for the specified object.
         *  @retval The frequency of the A# note as a floating point value. 
         */
        float getA_sharp(void);

        /** @brief A function that returns the frequency of the Bb note for the specified object.
         *  @retval The frequency of the Bb note as a floating point value. 
         */
        float getBb(void);

        /** @brief A function that returns the frequency of the B note for the specified object.
         *  @retval The frequency of the C note as a floating point value. 
         */
        float getB(void);

        /** @brief          An alternative function which can allow for the user to request
         *                  a specific note based upon the Notes_t enumerated values rather
         *                  than specifying a function call.
         *  @param  note    The note of which the user wants to receive the frequency of.
         *  @retval         Returns a floating point value representing the frequency in
         *                  Hz of the note specified.
          */
        float getNote(Notes_t note);

    private:
        /* Initialisation of the possible note frequencies. */
        float   C, Db, D, Eb, E, F,
                Gb, G, Ab, A, Bb, B;
        
};
    
    
/** @brief  Creation of a class to store the frequencies of notes at different octaves. 
 *          Inherits from `Notes` class.            
*/
class Octaves   : public Notes
{
    public:    
    
        /** @brief Stores the number of octaves being used. */
        static const char nOctaves = 9;
    
        /** @brief Initialising an array of the octaves and their notes. */
        Notes octaves[nOctaves];
        
        /* Constructor for the class*/
        Octaves();

        ~Octaves() = default;        

        /** @brief          A function which may be called to determine the amplitude of 
         *                  a specified note at a specified time.
         *  @param  octave  The octave in which the note being played is in.
         *  @param  note    The note which is currently being played.
         *  @param  time    The time at which the sound should be sampled at.
         *  @retval         Returns a floating point value representing the ampliude
         *                  of the note at the time it was sampled.
         *  @note           This function only uses unit amplitudes and does not change
         *                  depending on the volume. The maxmimum and minimum will always
         *                  be 1 and -1 respectively.
        */
        float PlayingNote(int octave, Notes_t note, float phase);

        /** @brief          A function which may be called to determine the amplitude of 
         *                  a specified frequency at a specified time.
         *  @param  freq    The frequency (in Hz) which should be played.
         *  @param  time    The time at which the sound should be sampled at.
         *  @retval         Returns a floating point value representing the ampliude
         *                  of the note at the time it was sampled.
         *  @note           This function only uses unit amplitudes and does not change
         *                  depending on the volume. The maxmimum and minimum will always
         *                  be 1 and -1 respectively.
        */
        float PlayingFrequency(float phase);

        
    private:
};

/** @brief      A class which inherits functions and variables from the `Octaves` class
 *              allowing for the notes being output to have a set of harmonic frequencies 
 *              attached to them too.
 */
class OctavesWithHarmonics :    public Octaves
{
    public:

        /** @brief  Creates an instance of the `OctavesWithHarmonics`.*/
        OctavesWithHarmonics();

        ~OctavesWithHarmonics() = default;

        /** @brief          A function which may be called upon to stop playing a range
         *                  of harmonic notes when a specific note is called upon to
         *                  be played.
         *  @param  n       The number of harmonics which should be applied to the signal.
         *  @param  octave  The octave in which the first harmonic is in.
         *  @param  note    The note which is being played.
         *  @param  time    The amount of time the signal has been playing. This corresponds
         *                  to the phase that the waveform is at.
         *  @retval         This function will return a floating point value representing
         *                  the normalised changed amplitude of the sigal at that point in
         *                  time.
         *  @note           This function could be made to produce a much cleaner response
         *                  by increasing the number of harmonics present and using non
         *                  integer harmonics too.
         */
        float PlayingNoteWithHarmonics(int n, int octave, Notes_t note, float phase);

        /** @brief          A function which may be called upon to stop playing a range
         *                  of harmonic frequencies when a specific note is called upon to
         *                  be played.
         *  @param  freq    The frequency in Hz that should be played.
         *  @param  time    The amount of time the signal has been playing. This corresponds
         *                  to the phase that the waveform is at.
         *  @retval         This function will return a floating point value representing
         *                  the normalised changed amplitude of the sigal at that point in
         *                  time.
         */
        float PlayingFrequencyWithHarmonics(int n, float phase);
        
        /** @brief          A function which calculates how much the amplitude of 
         *                  the harmonic should decay in comparison to the first
         *                  harmonic.
         *  @param  n       The harmonic which is being used.
         *  @retval         Returns a floating point value which represents the 
         *                  ratio of amplitudes. 
         *  @note           This function needs to be modified to better match the
         *                  relationship between the frequency and the decay rate
         *                  over the various harmonic frequencies. 
         */
        float HarmonicDecay(int n);

        /** @brief          A function which returns the maximum amplitude possible
         *                  for a set number of harmonics if they are all constructively
         *                  interfering.
         *  @param  n       The number of harmonics
         *  @retval         The maximum amplitude possible as a floating point value.
         */
        float getHarmomicDecayMax(int n);

        /** @brief          A function that can change the value of the decay constant
         *                  used to set the maxmimum amplitude of each successive 
         *                  harmonic.
         *  @param  decayC  The floating point value which the decay constant should be
         *                  set to.
         */
        void setDecayConstant(float decayC);

        /** @brief          A function which returns the decay constant being used
         *                  to determine the relative ampltiude of each successive
         *                  harmonic.
         *  @retval         The floating point representation of the decay constant.
         */
        float getDecayConstant(void);
        
    private:
        float decayConstant = 0.1;
        
};


