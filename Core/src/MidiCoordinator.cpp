#include "MidiCoordinator.hpp"

void MidiCoordinator::RegisterCallback(CallbackInterface cb){
    callback = cb;
}

void MidiCoordinator::setGate(bool on){
    if(on && !noteActive){ //if gate edge 'on' and no note is playing
        //send pitchbend -> expr -> NoteOn 
        message = MIDIMessageBuilder.pitchBend(1,latestBend);
        
        noteActive = true;
    }

    if(!on && noteActive){
        send Note Off
        noteActive = false
}

void MidiCoordinator::setNote(int note){

}

void MidiCoordinator::setBend(int bend){

}

void MidiCoordinator::setExpr(int expr){

}
