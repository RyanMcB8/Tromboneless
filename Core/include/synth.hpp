/** @file       synth.hpp
 *  @author     @RyanMcB8
 *  @brief      A header file containing all the necessary declarations and references to
 *              allow for the Tromboneless to synthesise the noise a trombone would make
 *              when playing the same notes.
 */

/* Preventing recursion. */
#pragma once

/* Addition of all necessary header files. */
#include <cmath>

/* ========================================================================================== */
/*                                  Class definitions                                         */
/* ========================================================================================== */

/* Creation of a class to store the frequency of notes. */
class Notes{
    public:

        /* Creation of the enumerated list of possible notes. */
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
            float Bb_in     =   29.14, float B_in    =   30.87){
                C = C_in;   Db = Db_in;     D = D_in;       Eb = Eb_in;
                E = E_in;   F = F_in;       Gb = Gb_in;     G = G_in;
                Ab = Ab_in; A = A_in;       Bb = Bb_in;     B = B_in;
                
            }

        /* ========================================================================================== */
        /*                               Note setters                                                 */
        /* ========================================================================================== */
        
        /** @brief A function that can change the value of the frequency that the C note plays.
         *  @param frequency The frequency which the C not should play.
         *  @note There is no return value from this function.
         */
        void setC(float frequency){
            C = frequency;
        }

        /** @brief A function that can change the value of the frequency that the C# note plays.
         *  @param frequency The frequency which the C# not should play.
         *  @note There is no return value from this function.
         */
        void setC_sharp(float frequency){
            Db = frequency;
        }

        /** @brief A function that can change the value of the frequency that the Db note plays.
         *  @param frequency The frequency which the Db not should play.
         *  @note There is no return value from this function.
         */
        void setDb(float frequency){
            Db = frequency;
        }
        
        /** @brief A function that can change the value of the frequency that the D note plays.
         *  @param frequency The frequency which the D not should play.
         *  @note There is no return value from this function.
         */
        void setD(float frequency){
            D = frequency;
        }

        /** @brief A function that can change the value of the frequency that the D# note plays.
         *  @param frequency The frequency which the D# not should play.
         *  @note There is no return value from this function.
         */
        void setD_sharp(float frequency){
            Eb = frequency;
        }
        
        /** @brief A function that can change the value of the frequency that the Eb note plays.
         *  @param frequency The frequency which the Eb not should play.
         *  @note There is no return value from this function.
         */
        void setEb(float frequency){
            Eb = frequency;
        }
        
        /** @brief A function that can change the value of the frequency that the E note plays.
         *  @param frequency The frequency which the E not should play.
         *  @note There is no return value from this function.
         */
        void setE(float frequency){
            E = frequency;
        }
        
        /** @brief A function that can change the value of the frequency that the F note plays.
         *  @param frequency The frequency which the F not should play.
         *  @note There is no return value from this function.
         */
        void setF(float frequency){
            F = frequency;
        }
        
        /** @brief A function that can change the value of the frequency that the F# note plays.
         *  @param frequency The frequency which the F# not should play.
         *  @note There is no return value from this function.
         */
        void setF_sharp(float frequency){
            Gb = frequency;
        }
        
        /** @brief A function that can change the value of the frequency that the Gb note plays.
         *  @param frequency The frequency which the Gb not should play.
         *  @note There is no return value from this function.
         */
        void setGb(float frequency){
            Gb = frequency;
        }
        
        /** @brief A function that can change the value of the frequency that the G note plays.
         *  @param frequency The frequency which the G not should play.
         *  @note There is no return value from this function.
         */
        void setG(float frequency){
            G = frequency;
        }
        
        /** @brief A function that can change the value of the frequency that the G# note plays.
         *  @param frequency The frequency which the G# not should play.
         *  @note There is no return value from this function.
         */
        void setG_sharp(float frequency){
            Ab = frequency;
        }
        
        /** @brief A function that can change the value of the frequency that the Ab note plays.
         *  @param frequency The frequency which the Ab not should play.
         *  @note There is no return value from this function.
         */
        void setAb(float frequency){
            Ab = frequency;
        }

        /** @brief A function that can change the value of the frequency that the A note plays.
         *  @param frequency The frequency which the A not should play.
         *  @note There is no return value from this function.
         */
        void setA(float frequency){
            A = frequency;
        }

        /** @brief A function that can change the value of the frequency that the A# note plays.
         *  @param frequency The frequency which the A# not should play.
         *  @note There is no return value from this function.
         */
        void setA_sharp(float frequency){
            Bb = frequency;
        }
        
        /** @brief A function that can change the value of the frequency that the Bb note plays.
         *  @param frequency The frequency which the Bb not should play.
         *  @note There is no return value from this function.
         */
        void setBb(float frequency){
            Bb = frequency;
        }
        
        /** @brief A function that can change the value of the frequency that the B note plays.
         *  @param frequency The frequency which the B not should play.
         *  @note There is no return value from this function.
         */
        void setB(float frequency){
            B = frequency;
        }
        
        /** @brief          An alternative function which can allow for the user to alter the
         *                  frequency of a specific note based upon the Notes_t enumerated
         *                  values rather than specifying a function call.
         *  @param  note    The note of which the user wants to change the frequency of.
         *  @param  freq    The frequency which the specified note should be changed to.
          */
        float setNote(Notes_t note, float freq){
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

        /* ========================================================================================== */
        /*                               Note getters                                                 */
        /* ========================================================================================== */

        /** @brief A function that returns the frequency of the C note for the specified object.
         *  @retval The frequency of the C note as a floating point value. 
         */
        float getC(void){
            return C;
        }

        /** @brief A function that returns the frequency of the C# note for the specified object.
         *  @retval The frequency of the C# note as a floating point value. 
         */
        float getC_sharp(void){
            return Db;
        }

        /** @brief A function that returns the frequency of the Db note for the specified object.
         *  @retval The frequency of the Db note as a floating point value. 
         */
        float getDb(void){
            return Db;
        }

        /** @brief A function that returns the frequency of the D note for the specified object.
         *  @retval The frequency of the D note as a floating point value. 
         */
        float getD(void){
            return D;
        }

        /** @brief A function that returns the frequency of the D# note for the specified object.
         *  @retval The frequency of the D# note as a floating point value. 
         */
        float getD_sharp(void){
            return Eb;
        }

        /** @brief A function that returns the frequency of the Eb note for the specified object.
         *  @retval The frequency of the Eb note as a floating point value. 
         */
        float getEb(void){
            return Eb;
        }

        /** @brief A function that returns the frequency of the E note for the specified object.
         *  @retval The frequency of the E note as a floating point value. 
         */
        float getE(void){
            return E;
        }

        /** @brief A function that returns the frequency of the F note for the specified object.
         *  @retval The frequency of the F note as a floating point value. 
         */
        float getF(void){
            return F;
        }

        /** @brief A function that returns the frequency of the F# note for the specified object.
         *  @retval The frequency of the F# note as a floating point value. 
         */
        float getF_sharp(void){
            return Gb;
        }

        /** @brief A function that returns the frequency of the Gb note for the specified object.
         *  @retval The frequency of the Gb note as a floating point value. 
         */
        float getGb(void){
            return Gb;
        }

        /** @brief A function that returns the frequency of the G note for the specified object.
         *  @retval The frequency of the G note as a floating point value. 
         */
        float getG(void){
            return G;
        }

        /** @brief A function that returns the frequency of the G# note for the specified object.
         *  @retval The frequency of the G# note as a floating point value. 
         */
        float getG_sharp(void){
            return Ab;
        }
        
        /** @brief A function that returns the frequency of the Ab note for the specified object.
         *  @retval The frequency of the Ab note as a floating point value. 
         */
        float getAb(void){
            return Ab;
        }

        /** @brief A function that returns the frequency of the A note for the specified object.
         *  @retval The frequency of the A note as a floating point value. 
         */
        float getA(void){
            return A;
        }

        /** @brief A function that returns the frequency of the A# note for the specified object.
         *  @retval The frequency of the A# note as a floating point value. 
         */
        float getA_sharp(void){
            return Bb;
        }

        /** @brief A function that returns the frequency of the Bb note for the specified object.
         *  @retval The frequency of the Bb note as a floating point value. 
         */
        float getBb(void){
            return Bb;
        }

        /** @brief A function that returns the frequency of the B note for the specified object.
         *  @retval The frequency of the C note as a floating point value. 
         */
        float getB(void){
            return B;
        }

        /** @brief          An alternative function which can allow for the user to request
         *                  a specific note based upon the Notes_t enumerated values rather
         *                  than specifying a function call.
         *  @param  note    The note of which the user wants to receive the frequency of.
         *  @retval         Returns a floating point value representing the frequency in
         *                  Hz of the note specified.
          */
        float getNote(Notes_t note){
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

    private:
        /* Initialisation of the possible note frequencies. */
        float   C, Db, D, Eb, E, F,
                Gb, G, Ab, A, Bb, B;
        
};
    
    
/* Creation of a class to store the frequencies of notes at different octaves. */
class Octaves   : public Notes
{
    public:    
    
        /* Stores the number of octaves being used. */
        static const char nOctaves = 9;
    
        /* Initialising an array of the octaves and their notes. */
        Notes octaves[nOctaves];
        
        /* Constructor for the class*/
        Octaves(){
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

        /** @brief          A function which clamps the value passed to it to be
         *                  within 0 and 1 so that it can remain within normalisation
         *                  bounds;
         *  @param  value   The value of which that should be clamped.
         *  @retval         The floating point value which has been clamped between
         *                  0 and 1.
         */
        float Clamp01(float value){
            return std::max(std::min(value, 1.0f), 0.0f);
        }

        /** @brief          A function which determines the relative maximum
         *                  amplitude of a sound depending on how long ago 
         *                  the user started playing the note. This grows as 
         *                  time goes on.
         *  @param  t       The relative time (between 0 and 1) in which the
         *                  note has been playing for.
         *  @retval         Returns a floating point value between 0 and 1
         *                  representing the maximum amplitude that the note
         *                  may reach depending on the t parameter.
         *  @note           Currently this function is just a ramp function but should be
         *                  modified in the future to better match that of real world noise.
         */
        float TimeAscension(float t){
            t = Clamp01(t);
            return Clamp01(t);
        }
        
        /** @brief          A function which determines the relative maximum
         *                  amplitude of sound depending on how long ago the
         *                  note was released.
         *  @param  t       The normalised amount of time in which the note
         *                  was released.
         *  @retval         Returns a floating point value between 0 and 1
         *                  representing the maximum amplitude.
         */
        float TimeDecay(float t){
            t = Clamp01(t);
            return Clamp01(1 - (t*t));
        }

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
        float PlayingNote(int octave, Notes_t note, float time){
            return (float) cos(octaves[octave].getNote(note) * time * 2 * M_PI);
        }

        /** @brief          A function which determines that maximum amplitude the note may
         *                  reach depending upon the amount of time it has been since the
         *                  user started the input.
         *  @param  octave  The octave in which the first harmonic is in.
         *  @param  note    The note which is starting to be played.
         *  @param  time    The current time. This is used to set phase of the sinusoidal
         *                  wave used to synthesise the note.
         *  @param  t       This is a parametric value representing the relative time of
         *                  the ascension. At `t=0`, the maximum amplitude is 0, and at 
         *                  `t=1`, the maximum amplitude is 1.
         *  @retval         Returns a floating point value between 0 and 1 representing
         *                  the notes maximmum frequency.
         */
        float StartNote(int octave, Notes_t note, float time, float t){
            return PlayingNote(octave, note, time) * TimeAscension(t);
        }

        /** @brief          A function which may be called upon to play a single note
         *                  in a specified octave.
         *  @param  octave  The octave in which the first harmonic is in.
         *  @param  note    The note which is being played.
         *  @param  time    The total amount of time the signal has been playing for.
         *  @param  t       A parametric value which is used to determine how much the
         *                  amplitudes of the waveforms should be reduced. When `t=0`, the
         *                  waveforms are at their maximum amplitude and when`t=0`, their
         *                  amplitudes = 0.
         *  @retval         This function will return a floating point value representing
         *                  the normalised changed amplitude of the sigal at that point in
         *                  time.
         *  @note           The number of steps between 0 and 1 for the time value should 
         *                  be kept as high as possible as this represents the stage of
         *                  oscillation the cosine wave is at.
        */
        float EndNote(int octave, Notes_t note, float time, float t){
            return PlayingNote(octave, note, time) * TimeDecay(t);
        }
    private:
};

class OctavesWithHarmonics :    public Octaves
{
    public:

        /* Constructor. */
        OctavesWithHarmonics(){

        }

        
        /** @brief          A function which may be called upon to stop playing a range
         *                  of harmonic notes when a specific note is called upon to
         *                  be played.
         *  @param  n       The number of harmonics which should be applied to the signal.
         *  @param  octave  The octave in which the first harmonic is in.
         *  @param  note    The note which is being played.
         *  @param  time    The amount of time the signal has been playing. This corresponds
         *                  to the phase that the waveform is at.
         *  @param  t       A parametric value which is used to determine how much the
         *                  amplitudes of the waveforms should be reduced. When `t=0`, the
         *                  waveforms are at their maximum amplitude and when`t=0`, their
         *                  amplitudes = 0.
         *  @retval         This function will return a floating point value representing
         *                  the normalised changed amplitude of the sigal at that point in
         *                  time.
         *  @note           This function could be made to produce a much cleaner response
         *                  by increasing the number of harmonics present and using non
         *                  integer harmonics too.
         */
        float StartNoteWithHarmonics(int n, int octave, Notes_t note, float time, float t){
            float outputAmplitude = 0;
            for (int i=0; i < n; i++){
                outputAmplitude += HarmonicDecay(n, octave, note) * StartNote(n, note, time, t);
            } 
            return (outputAmplitude/n);
        }
        
        /** @brief          A function which may be called upon to stop playing a range
         *                  of harmonic notes when a specific note is called upon to
         *                  be played.
         *  @param  n       The number of harmonics which should be applied to the signal.
         *  @param  octave  The octave in which the first harmonic is in.
         *  @param  note    The note which is being played.
         *  @param  time    The amount of time the signal has been playing. This corresponds
         *                  to the phase that the waveform is at.
         *  @param  t       A parametric value which is used to determine how much the
         *                  amplitudes of the waveforms should be reduced. When `t=0`, the
         *                  waveforms are at their maximum amplitude and when`t=0`, their
         *                  amplitudes = 0.
         *  @retval         This function will return a floating point value representing
         *                  the normalised changed amplitude of the sigal at that point in
         *                  time.
         *  @note           This function could be made to produce a much cleaner response
         *                  by increasing the number of harmonics present and using non
         *                  integer harmonics too.
         */
        float PlayingNoteWithHarmonics(int n, int octave, Notes_t note, float time, float t){
            float outputAmplitude = 0;
            for (int i=0; i < n; i++){
                outputAmplitude += HarmonicDecay(n, octave, note) * PlayingNote(n, note, time);
            } 
            return (outputAmplitude/n);
        }
        
        /** @brief          A function which may be called upon to stop playing a range
         *                  of harmonic notes when a specific note is called upon to
         *                  be played.
         *  @param  n       The number of harmonics which should be applied to the signal.
         *  @param  octave  The octave in which the first harmonic is in.
         *  @param  note    The note which is being played.
         *  @param  time    The amount of time the signal has been playing. This corresponds
         *                  to the phase that the waveform is at.
         *  @param  t       A parametric value which is used to determine how much the
         *                  amplitudes of the waveforms should be reduced. When `t=0`, the
         *                  waveforms are at their maximum amplitude and when`t=0`, their
         *                  amplitudes = 0.
         *  @retval         This function will return a floating point value representing
         *                  the normalised changed amplitude of the sigal at that point in
         *                  time.
         *  @note           This function could be made to produce a much cleaner response
         *                  by increasing the number of harmonics present and using non
         *                  integer harmonics too.
         */
        float EndNoteWithHarmonics(int n, int octave, Notes_t note, float time, float t){
            float outputAmplitude = 0;
            for (int i=0; i < n; i++){
                outputAmplitude += HarmonicDecay(n, octave, note) * EndNote(n, note, time, t);
            } 
            return (outputAmplitude/n);
        }
        
        private:
        float decayConstant = 2;
        
        /** @brief          A function which calculates how much the amplitude of 
         *                  the harmonic should decay in comparison to the first
         *                  harmonic.
         *  @param  n       The integer multiple of the harmonic whose relative
         *                  amplitude is being calculated.
         *  @param  octave  The octave in which the first harmonic is in.
         *  @param  note    The note which is being played.
         *  @retval         Returns a floating point value which represents the 
         *                  ratio of amplitudes. 
         *  @note           This function needs to be modified to better match the
         *                  relationship between the frequency and the decay rate
         *                  over the various harmonic frequencies. 
         */
        float HarmonicDecay(int n, int octave, Notes::Notes_t note){
            return Clamp01(exp(-(n * (octave + note) * decayConstant)/100));
        }
    };
    
    