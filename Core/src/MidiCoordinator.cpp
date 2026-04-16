#include "MidiCoordinator.hpp"

MidiCoordinator::MidiCoordinator(){

}

void MidiCoordinator::RegisterCallback(CallbackInterface cb){
    callback = cb;
}

void MidiCoordinator::PressureEdge(bool on)
{
    switch (current_state)
    {
    case IDLE:
        if (on)
        {
            callback(builder.pitchBend(1, latestBend));
            callback(builder.expr(1, latestExpr));
            callback(builder.noteOn(1, latestNote, velocity));

            currentNote = latestNote;
            setState(PLAYING);
        }
        break;

    case PLAYING:
        if (!on)
        {
            callback(builder.noteOff(1, currentNote, velocity));
            setState(IDLE);
        }
        break;

    case ERROR:
        break;
    }
}

void MidiCoordinator::ChangeNote(int note)
{
    if (note < 0) note = 0;
    if (note > 127) note = 127;

    latestNote = note;

    if (current_state == PLAYING)
    {
        callback(builder.noteOn(1, latestNote, velocity));
        callback(builder.noteOff(1, currentNote, velocity));
        internal_synth.HandleMIDINoteOn(latestNote, currentNote);
        internal_synth.HandleMIDINoteOff(currentNote);
    }
}

void MidiCoordinator::setBend(int bend)
{
    if(bend < 0) bend = 0;
    if(bend > 8192) bend = 8192;
    latestBend = bend;

    if (current_state == PLAYING)
    {
        callback(builder.pitchBend(1, latestBend));
        internal_synth.HandleMIDIPitchBend(latestBend);
    }
}

void MidiCoordinator::setExpr(int expr)
{
    if (expr < 0) expr = 0;
    if (expr > 127) expr = 127;

    latestExpr = expr;

    if (current_state == PLAYING)
    {
        callback(builder.expr(1, latestExpr));
    }
}

void MidiCoordinator::setState(State newstate){
    current_state = newstate;
}