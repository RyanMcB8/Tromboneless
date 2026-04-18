    /*** @file PitchMapper.hpp
     *   @author Aidan McIntosh
    */

#include <cstdint>
#include "tromboneless_data.hpp"

    /** @brief Takes raw input values from the LIDAR and
     * mouthpiece/embouchure sensors and maps them to appropriate values for
     * the musical coordinator to work with.
     */
class PitchMapper{
    private:

        typedef enum{
            piccolo = 6,
            soprano = 4,
            alto = 2,
            tenor = 0,
            bass = -2,
            contrabass = -4
        } Trombone_type_t;

        int slide_max_limit_mm = 500; /* Max limit of virtual trombone slide in mm.*/
        int slide_min_limit_mm = 0; /* Min limit of virtual trombone slide in mm.*/
        int mouthpiece_MIDI_note = 34; /* MIDI note produced by mouthpiece.*/ 

        int trombone_type = Trombone_type_t::piccolo;

    public:

        /** @brief Constructor. */
        PitchMapper();

        /** @brief Maps raw LiDAR time-of-flight values to MIDI pitch bend scaled
         *         and clamped to range of 0-8192. 
         *  @param tof_distance Raw value returned from time-of-flight sensor.
         *  @return Integer 0-8192
        */

        int tof_to_MIDI_bend(uint16_t tof_distance);

        /** @brief Maps capacitative mouthpiece readings to discrete MIDI notes. 
         * @param delta Raw value supplied by the cap1188
        */
        int mouthpiece_to_MIDI_note(int8_t delta);


        /** @brief Set maximum trombone slide limit in mm.*/
        void SetSlideMaxLimit(int new_slide_max_limit_mm);

        /** @brief Return current max trombone slide limit in mm.*/    
        int GetSlideMaxLimit();

        /** @brief Set minimum trombone slide limit in mm.*/
        void SetSlideMinLimit(int new_slide_min_limit_mm);

        /** @brief Return current min trombone slide limit in mm.*/    
        int GetSlideMinLimit();

        /** @brief Set current mouthpiece MIDI note, bypassing mapping function.*/    
        void SetMouthMIDI(int new_MIDI_note);

        /** @brief Return current mouthpiece MIDI note.*/    
        int GetMouthMIDI();

        void SetTromboneType(ShiftKeyingOptions_t trombone_choice);
};
