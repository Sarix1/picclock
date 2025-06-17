#ifndef EVENT_H
#define	EVENT_H

#include <inttypes.h>

#define MAX_EVENTS 16

enum EVENT_TYPE
{
    EV_ERROR,
    EV_CHANGE_STATE,
    EV_ENTER_STATE,
    EV_INPUT,
    EV_CLOCK_STEP, // used to step forward/backward clocks
    EV_ALARM_TIMER, // toggles alarm timer (will clock alarm or not?)
    EV_ALARM_START,
    EV_ALARM_STOP,
    EV_ALARM_SNOOZE,
    EV_AUDIO_PLAY,
    EV_AUDIO_STOP,
    EV_DISPLAY_MODE,
    NUM_EVENT_TYPES
};

struct clock_ev_s
{
    uint8_t ev_type       : 4;
    uint8_t arg_clock_id  : 2;
    uint8_t arg_direction : 2;
    uint8_t arg_time_unit;
};

struct input_ev_s
{
    uint8_t ev_type : 4;
    uint8_t arg_button_id : 4; // push-button
};

struct display_ev_s
{
    uint8_t ev_type : 4;
    uint8_t arg_display_mode : 4;
};

struct audio_ev_s
{
    uint8_t ev_type : 4;
    uint8_t arg_state : 4;
    uint8_t arg_song_id;
};

typedef struct
{
    union
    {
        uint16_t ev_data_bits;
        
        struct
        {
            uint8_t ev_type  : 4;
            uint8_t arg1     : 4;
            uint8_t arg2     : 4;
            uint8_t arg3     : 4;
        };
        
        struct
        {
            uint8_t _ev_type : 4;
            uint8_t arg_state : 4;
            uint8_t _unused;
        };
        
        struct clock_ev_s   clock_ev;
        struct input_ev_s   input_ev;
        struct display_ev_s display_ev;
        struct audio_ev_s   audio_ev;
    };
} Event_t;

typedef struct
{
    Event_t events[MAX_EVENTS];
    uint8_t count;
    uint8_t head;
    uint8_t tail;
} EventBus_t;

Event_t newEvent(uint8_t ev_type, uint8_t arg);
void queueEvent(Event_t event);
void handleEvent(Event_t event);
void handleEvents();

#endif	/* EVENT_H */
