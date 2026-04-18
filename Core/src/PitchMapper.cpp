#include "PitchMapper.hpp"

static const PitchMapper::PartialDef partials[] = {
    { 70, -1, 5, 62 },
    { 60, 65,  4, 58 },
    { 50, 55,  3, 53 },
    { 40, 45,  2, 46 },
    { 30, 35,  1, 34 },
};

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
    // For every possible partial
    for (const auto& p : partials)
    {
        // Non-hysteresis logic => Delta lies neatly within one band
        if (delta >= p.minDelta)
        {
            partial = p.partial;
            mouthpiece_MIDI_note = p.midiNote + trombone_type;
            return mouthpiece_MIDI_note;
        }
        // Hysteresis logic => Between upper and lower partial, prefer lower unless it was upper before
        if (p.hysteresisDelta > 0 && delta >= p.hysteresisDelta && partial == p.partial)
        {
            // Don't update partial — keep it as-is
            mouthpiece_MIDI_note = p.midiNote + trombone_type;
            return mouthpiece_MIDI_note;
        }
    }

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