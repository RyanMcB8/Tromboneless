#include "MidiCoordinator.hpp"

void MidiCoordinator::RegisterCallback(CallbackInterface cb){
    callback = cb;
}
/*
void MidiCoordinator::setGate(bool on){
    if(on || !noteActive){
        send Note On
        noteActive = true
    }

    if not on AND noteActive:
        send Note Off
        noteActive = false
}
        */