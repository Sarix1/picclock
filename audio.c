#include <inttypes.h>
#include "globals.h"
#include "audio.h"
#include "timer.h"
#include "tunes.h"

Tune_t CurrentTune = {0};

void speakerOn()
{
    PWM_SetDutyPercent(50);
    PWM_SetFrequency(1000);
    initPWM();
}

void speakerOff()
{
    PWM_SetDutyPercent(0);
    PWM_SetFrequency(0);
    quitPWM();
}

uint8_t loadTune(uint8_t song_id)
{
    uint8_t error = NO_ERROR;
    
    // testing
    // later, load a song from a 28C256 EEPROM
    if (song_id == 0)
    {
        CurrentTune.name = "Mario Underwater";
        CurrentTune.size = sizeof(MarioUW_notes);
        CurrentTune.notes = MarioUW_notes;
        CurrentTune.durations = MarioUW_durations;
    }
    else
        error = ERROR_TUNE_LENGTH_ZERO;
    
    if (CurrentTune.size == 0)
        error = ERROR_TUNE_LENGTH_ZERO;
    else if (CurrentTune.notes == 0 || CurrentTune.durations == 0)
        error = ERROR_TUNE_NULL_POINTER;
    
    return error;
}

void startTune(uint8_t song_id)
{
    uint8_t error = loadTune(song_id) == NO_ERROR;
    if (error)
    {
        speakerOff();
        Globals.audio.playing = 0;
        Globals.status.error = error;
    }
    else
    {
        speakerOn();
        Globals.audio.playing = 1;
    }
}

void stopTune()
{
    Globals.audio.playing = 0;
    speakerOff();
}

uint8_t playNote()
{
    uint8_t duration = CurrentTune.durations[Globals.audio.note_index];
    PWM_SetFrequency(CurrentTune.notes[Globals.audio.note_index]); 

    if (++Globals.audio.note_index >= CurrentTune.size)
        Globals.audio.note_index = 0;

    return duration;
}
