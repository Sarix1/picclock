#include <stdlib.h>
#include "audio.h"
#include "global.h"
#include "event.h"
#include "pwm.h"
#include "tunes.h"
#include "input.h"
#include "display.h"

// Definitions
#define NOTE_DURATION_FACTOR    1
#define AUDIO_DC_MAX            (1024/(2*4)) // 1/4th of 50% duty cycle
#define AUDIO_DC_DIVISOR        ((MAX_VOLUME*MAX_VOLUME)/(AUDIO_DC_MAX))

// Locals
static uint32_t audio_next = 0;
static uint16_t note_index = 0;

// Forward declarations
static inline void audioPlay();
static inline void audioCheck();

// Functions
void updateAudio()
{    
    audioCheck();
    
    if (Global.audio_playing == 1)
        audioPlay();
}

static inline void audioPlay()
{
    uint16_t dc = ((uint16_t)Global.audio_volume * Global.audio_volume) / AUDIO_DC_DIVISOR;
    PWM_SetDutyCycle(dc);

    if (Global.timer_ms >= audio_next)
    {
        audio_next += MarioUW_durations[note_index] * NOTE_DURATION_FACTOR;
        PWM_SetFrequency(500 + MarioUW_notes[note_index]);
        note_index++;

        if (note_index >= MarioUW_note_count)
            note_index = 0;
    }
}

static inline void audioCheck()
{
    if (Global.audio_test == 1 || Global.alarming == 1)
    {
        if (Global.audio_playing == 0)
            queueEvent(newEvent(EV_AUDIO_PLAY, Global.alarm_song_id));
    }
    else if (Global.audio_playing == 1)
        queueEvent(newEvent(EV_AUDIO_STOP, 0));
}

void handleAudioEvent(Event_t event)
{
    switch (event.ev_type)
    {
        case EV_AUDIO_PLAY:
            Global.audio_playing = 1;
            initPWM();
            audio_next = Global.timer_ms;
            note_index = 0;
            break;
            
        case EV_AUDIO_STOP:
            Global.audio_playing = 0;
            quitPWM();
            break;
            
        default:
            break;
    }
}

/*** Volume Set ***/
void handleEvent_VolumeSet(Event_t event)
{
    switch (event.ev_type)
    {
        case EV_ENTER_STATE:
            queueEvent(newEvent(EV_DISPLAY_MODE, DISP_MODE_VOLUME_SET));
            Global.audio_test = 1;
            break;
            
        case EV_INPUT:
            switch (event.input_ev.arg_button_id)
            {
                //case BTN_ENCODER:
                //    break;
                case BTN_ENCODER_CW:
                    if (Global.audio_volume < MAX_VOLUME)
                        Global.audio_volume++;
                    break;
                case BTN_ENCODER_CCW:
                    if (Global.audio_volume > MIN_VOLUME)
                        Global.audio_volume--;
                    break;
            }
            break;
    }    
}