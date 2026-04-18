#include "PitchMapper.hpp"

PitchMapper::PitchMapper(){

}

int PitchMapper::tof_to_MIDI_bend(uint16_t tof_distance){

    if (tof_distance > slide_limit_mm) // Clamp to max value
        tof_distance = slide_limit_mm;

    if (slide_limit_mm <= 0)
        slide_limit_mm = 1; // Guard against division by zero

    return 8192 - (static_cast<int>(tof_distance) * 8192) / slide_limit_mm;
}

int PitchMapper::mouthpiece_to_MIDI_note(int8_t delta){
        if (delta >= 75)
            mouthpiece_MIDI_note = 62;
        else if (delta >= 70)
            mouthpiece_MIDI_note = 58;
        else if (delta >= 60)
            mouthpiece_MIDI_note = 53;
        else if (delta >= 50)
            mouthpiece_MIDI_note = 46;
        else if (delta >= 40)
            mouthpiece_MIDI_note = 34;
        else
            mouthpiece_MIDI_note = 0;
            
        return mouthpiece_MIDI_note;
}

void PitchMapper::SetSlideLimit(int new_slide_limit_mm){
    slide_limit_mm = new_slide_limit_mm;
}

int PitchMapper::GetSlideLimit(){
    return slide_limit_mm;
}

void PitchMapper::SetMouthMIDI(int new_MIDI_note){
    mouthpiece_MIDI_note = new_MIDI_note;
}

int PitchMapper::GetMouthMIDI(){
    return mouthpiece_MIDI_note;
}