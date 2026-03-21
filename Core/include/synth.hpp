/** @file       synth.hpp
 *  @author     @RyanMcB8
 *  @brief      A header file containing all the necessary declarations and references to
 *              allow for the Tromboneless to synthesise the noise a trombone would make
 *              when playing the same notes.
 */

/* Preventing recursion. */
#pragma once

/* Addition of all necessary header files. */

/* New type definitions. */

/* Creation of a class to store the frequency of notes. */
class Notes{
    public:
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

    private:
        /* Initialisation of the possible note frequencies. */
        float   C, Db, D, Eb, E, F,
                Gb, G, Ab, A, Bb, B;
        
};
    
    
/* Creation of a class to store all the synthesiser data. */
class Octaves
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
    private:
};

 