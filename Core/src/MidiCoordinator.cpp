#include "MidiCoordinator.hpp"

MidiCoordinator::MidiCoordinator(AudioRender& synth)
    : synthRef(synth)
{
}

void MidiCoordinator::RegisterCallback(CallbackInterface cb)
{
    callback = cb;
}

void MidiCoordinator::PressureEdge(bool on)
{
    switch (current_state)
    {
    case IDLE:
        if (on)
        {
            if (latestNote < 0)
                return;

            if (latestBend < 0)
                latestBend = 8192;

            if (latestExpr < 0)
                latestExpr = 127;

            if (current_output == EXTERNAL)
            {
                if (callback)
                {
                    callback(builder.pitchBend(1, latestBend));
                    callback(builder.expr(1, latestExpr));
                    callback(builder.noteOn(1, latestNote, velocity));
                }
            }
            else
            {
                synthRef.pitchBendFromMidi(latestBend);
                synthRef.noteOnFromMidi(latestNote);
            }

            currentNote = latestNote;
            lastSentBend = latestBend;
            lastSentExpr = latestExpr;

            setState(PLAYING);
        }
        break;

    case PLAYING:
        if (!on)
        {
            if (current_output == EXTERNAL)
            {
                if (currentNote >= 0 && callback)
                    callback(builder.noteOff(1, currentNote, velocity));
            }
            else
            {
                synthRef.noteOffFromMidi();
            }

            currentNote = -1;
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

    if (current_state != PLAYING)
        return;

    if (latestNote == currentNote)
        return;

    if (current_output == EXTERNAL)
    {
        if (currentNote >= 0 && callback)
            callback(builder.noteOff(1, currentNote, velocity));

        if (callback)
            callback(builder.noteOn(1, latestNote, velocity));
    }
    else
    {
        synthRef.noteChangeFromMidi(latestNote);
    }

    currentNote = latestNote;
}

void MidiCoordinator::setBend(int bend)
{
    if (bend < 0) bend = 0;
    if (bend > 8192) bend = 8192;

    latestBend = bend;

    if (current_state != PLAYING)
        return;

    if (lastSentBend == latestBend)
        return;

    if (current_output == EXTERNAL)
    {
        if (callback)
            callback(builder.pitchBend(1, latestBend));
    }
    else
    {
        synthRef.pitchBendFromMidi(latestBend);
    }

    lastSentBend = latestBend;
}

void MidiCoordinator::setExpr(int expr)
{
    if (expr < 0) expr = 0;
    if (expr > 127) expr = 127;

    latestExpr = expr;

    if (current_state != PLAYING)
        return;

    if (lastSentExpr == latestExpr)
        return;

    if (current_output == EXTERNAL)
    {
        if (callback)
            callback(builder.expr(1, latestExpr));
    }

    lastSentExpr = latestExpr;
}

void MidiCoordinator::setState(State newstate)
{
    current_state = newstate;
}

AudioRender& MidiCoordinator::getSynth()
{
    return synthRef;
}

void MidiCoordinator::setDevice(bool external_device_present)
{
    if (external_device_present)
        current_output = EXTERNAL;
    else
        current_output = INTERNAL;
}