#include "PitchMapper.hpp"

PitchMapper::PitchMapper(){

}

int PitchMapper::tof_to_MIDI_bend(uint16_t tof_distance){

    if (tof_distance > slide_max_limit_mm) // Clamp to max value
        tof_distance = slide_max_limit_mm;

    if (tof_distance < slide_min_limit_mm) // Clamp to min value
        tof_distance = slide_min_limit_mm;    

    if (slide_max_limit_mm <= 0)
        slide_max_limit_mm = 1; // Guard against division by zero

    return 8192 - (static_cast<int>(tof_distance) * 8192) / (slide_max_limit_mm-slide_min_limit_mm);
}

int PitchMapper::mouthpiece_to_MIDI_note(int8_t delta){
        if (delta >= 75)
            mouthpiece_MIDI_note = 62 + trombone_type;
        else if (delta >= 70)
            mouthpiece_MIDI_note = 58 + trombone_type;
        else if (delta >= 60)
            mouthpiece_MIDI_note = 53 + trombone_type;
        else if (delta >= 50)
            mouthpiece_MIDI_note = 46 + trombone_type;
        else if (delta >= 40)
            mouthpiece_MIDI_note = 34 + trombone_type;
        else
            mouthpiece_MIDI_note = 0 + trombone_type;
            
        return mouthpiece_MIDI_note;
}

void PitchMapper::SetSlideMaxLimit(int new_slide_max_limit_mm){
    slide_max_limit_mm = new_slide_max_limit_mm;
}

int PitchMapper::GetSlideMaxLimit(){
    return slide_max_limit_mm;
}

void PitchMapper::SetSlideMinLimit(int new_slide_min_limit_mm){
    slide_min_limit_mm = new_slide_min_limit_mm;
}

int PitchMapper::GetSlideMinLimit(){
    return slide_min_limit_mm;
}


void PitchMapper::SetMouthMIDI(int new_MIDI_note){
    mouthpiece_MIDI_note = new_MIDI_note;
}

int PitchMapper::GetMouthMIDI(){
    return mouthpiece_MIDI_note;
}

void PitchMapper::SetTromboneType(ShiftKeyingOptions_t trombone_choice){
    switch(trombone_choice){
        case(SKOpt_CONTRABASS):
            trombone_type = Trombone_type_t::contrabass;
        case(SKOpt_BASS):
            trombone_type = Trombone_type_t::bass;
        case(SKOpt_TENOR):
            trombone_type = Trombone_type_t::tenor;
        case(SKOpt_ALTO):
            trombone_type = Trombone_type_t::alto;
        case(SKOpt_SOPRANO):
            trombone_type = Trombone_type_t::soprano;
        case(SKOpt_PICCOLO):
            trombone_type = Trombone_type_t::piccolo;
    }


};