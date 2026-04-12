/** @file       envelope.hpp
 *  @author     @RyanMcB8
 *  @brief      A header file containing all the necessary declarations and references to
 *              create a functional envelope class using the ADSR standard.
 */

/* Preventing recursion. */
#pragma once

/* Addition of all necessary header files. */
#include <cmath>

class Envelope{
    public:

        /*  Enumerated type to describe which part of the ADSR the object is in. */
        typedef enum{
            no_stage,
            attack_stage,
            decay_stage,
            sustain_stage,
            rest_stage,
        }   envelope_stages_t;
        
        /** @brief                  The constructor function for the `Envelope` class
         *                          which sets the values of the ADSR of the envelope
         *                          and initialises the beginning of it.
         *  @param  attack_in       The number of times the `getAmplitude()` function
         *                          should be called before the envelope reaches its
         *                          peak of 1.
         *  @param  decay_in        The number of times the `getAmplitude()` function
         *                          should be called after the attack stage to reach
         *                          the sustain level.
         *  @param  sustain_in      The relative amplitude of the signal after the
         *                          decay stage.
         *  @param  rest_in         The number of times the 'getAmplitde()` function
         *                          should be called once the endNote flag has been
         *                          triggered before the noise reaches an amplitude
         *                          of 0.
         *  @note                   This class does not depend upon the real time
         *                          or clock of the device, but instead the number of
         *                          times the `getAmplitude()` function is called
         *                          upon.
         */
        Envelope(int attack_in = 100, int decay_in = 20, float sustain_in = 0.98, int rest_in = 10);

        /** @brief                  The destructor function for the `Envelope` class
         *                          which is currently just set to default.
         */
        ~Envelope() = default;

        /** @brief                  A function that will return the current maximum
         *                          amplitude of the envelope function. Depending
         *                          one how many times this has been called sets at
         *                          which stage the envelope is in.
         *  @retval                 A floating point value between 0 and 1 representing
         *                          the gain which would be applied to a signal it is
         *                          attached to.
         */
        float getAmplitude(void);

        /** @brief                  A function that enters the device into the attack stage. */
        void startEnvelope(void);

        /** @brief                  A function which sets the envelope to enter the rest
         *                          stage and begin to approach 0.
         */
        void endEnvelope(void);

        /** @brief                  A function which can allow for the attack value to be
         *                          changed externally from the class.
         *  @param  attack_in       The number of times the `getAmplitude()` function
         *                          should be called before the envelope reaches its
         *                          peak of 1.
         */
        void setAttack(int attack_in);

        /** @brief                  A function which returns the number of times the
         *                          `getAmplitude()` function needs to be called to go
         *                          from 0 to 1 for the envelope gain value.
         *  @retval                 The integer representation of how many times the 
         *                          the `getAmplitude()` function needs to be called
         *                          to exit the attack stage.
         */
        int getAttack(void);

        /** @brief                  A function which can allow for the decay value to be
         *                          changed externally from the class.
         *  @param  decay_in        The number of times the `getAmplitude()` function
         *                          should be called before the output goes from 1 to the
         *                          chosen sustain value.
         */
        void setDecay(int decay_in);

        /** @brief                  A function which returns the number of times the
         *                          `getAmplitude()` function needs to be called to go
         *                          from 1 to the sustain level for the envelope gain
         *                          value.
         *  @retval                 The integer representation of how many times the 
         *                          the `getAmplitude()` function needs to be called
         *                          to exit the decay stage and enter sustain.
         */
        int getDecay(void);

        /** @brief                  A function which can allow for the sustain value to be
         *                          changed externally from the class.
         *  @param  sustain_in      The relative amplitude that the envelope should hold
         *                          after the decay stage. This should be a floating point
         *                          value between 0 and 1.
         */
        void setSustain(float sustain_in);

        /** @brief                  A function which returns the value of the sustain level
         *                          currently set for the object.
         *  @retval                 A floating point representation of the sustian amplitude.
         */
        float getSustain(void);

        /** @brief                  A function which can allow for the rest value to be
         *                          changed externally from the class.
         *  @param  rest_in         The number of times the `getAmplitude()` function
         *                          should be called before the envelope reaches its
         *                          peak of 0 from the sustain value.
         */
        void setRest(int rest_in);

        /** @brief                  A function which can allow for the rest value to be
         *                          used outside of the object without affecting the
         *                          objects instance of the value from changing.
         *  @retval                 Returns the number of times the `getAmplitude()`
         *                          function needs to be called before the ampltude
         *                          reaches 0 from the sustain value.
         */
        int getRest(void);

        /** @brief                  This function determines the shape of the attack curve.
         *                          It needs to start from 0 and end at 1.
         *  @retval                 Returns the amplitude at the time of calling.
         *  @note                   This is currently a linear ramp function between 0 and 1.
         */
        float attackFunction(void);

        /** @brief                  This function determines the shape of the decay curve.
         *                          It needs to start from 1 and finish at the sustain value
         *                          which has been set by the object.
         *  @retval                 Returns the floating point representation of the ampltiude
         *                          at the time of request.
         *  @note                   This is currently a linear ramp function between 1 and
         *                          the sustain value which has been set.
         */
        float decayFunction(void);

        /** @brief                  This function determines the shape of the rest curve.
         *                          It needs to start from the sustain value and end at 0.
         *  @retval                 Returns the floating point representation of the amplitude
         *                          at the time of request.
         *  @note                   This is currently a linear ramp function between the sustain    
         *                          value set and 0. This may be modified to match the shape of
         *                          the instrument better.
         */
        float restFunction(void);

        envelope_stages_t getStage(void);

        void setStage(envelope_stages_t stage_in);

    private:
        /*  ADSR Variables */
        int attack;
        int decay;
        float sustain;
        int rest;

        /** @brief              A function which ensure that the value passed is clamped
         *                      to be between 0 and 1.
         *  @retval             A floating point value between 0 and 1. This is equal to
         *                      the input if the input it also between 0 and 1.
         */
        float clamp01(float value);
        /*  Timing Variables */
        int counter = 0;
        envelope_stages_t stage = no_stage;        
};