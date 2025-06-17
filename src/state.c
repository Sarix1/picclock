#include <inttypes.h>
#include "state.h"
#include "event.h"
#include "global.h"
#include "display.h"
#include "input.h"

// Definitions
typedef void (*fnp)(void);
typedef void (*fnp_i)(uint8_t);
typedef uint8_t (*i_fnp)(void);
typedef uint8_t (*i_fnp_i)(void);
typedef void (*fnp_ev)(Event_t);

typedef struct
{
    fnp_ev handleEvent;
} State_t;

// Forward declarations
void handleStateEvent(Event_t event);
static inline void changeState(uint8_t state_id);
static void handleEvent_Message(Event_t event);
// From clock.c
void handleEvent_TimeClock(Event_t event);
void handleEvent_TimeSet(Event_t event);
void handleEvent_AlarmSet(Event_t event);
void handleEvent_SecSet(Event_t event);
void handleEvent_SnoozeSet(Event_t event);
// From audio.c
void handleEvent_VolumeSet(Event_t event);
// From sensor.c
void handleEvent_Sensor(Event_t event);
// From display.c
//void handleEvent_BrightSet(Event_t event);

// Locals
static State_t States[NUM_STATES];

void initStates()
{
    States[STATE_MESSAGE].handleEvent     = handleEvent_Message;
    States[STATE_TIME_CLOCK].handleEvent  = handleEvent_TimeClock;
    States[STATE_TIME_SET].handleEvent    = handleEvent_TimeSet;
    States[STATE_ALARM_SET].handleEvent   = handleEvent_AlarmSet;
    States[STATE_SEC_SET].handleEvent     = handleEvent_SecSet;
    States[STATE_SNOOZE_SET].handleEvent  = handleEvent_SnoozeSet;
    States[STATE_SENSOR].handleEvent      = handleEvent_Sensor;
    //States[STATE_TUNE_SET].handleEvent    = handleEvent_Message;
    States[STATE_VOLUME_SET].handleEvent  = handleEvent_VolumeSet;
    //States[STATE_BRIGHT_SET].handleEvent  = handleEvent_BrightSet;
}

// Functions
void handleStateEvent(Event_t event)
{
    switch (event.ev_type)
    {
        case EV_CHANGE_STATE:
            changeState(event.arg_state);
            break;
        case EV_INPUT:
        default:
            States[Global.current_state].handleEvent(event);
            break;
    }
}

static inline void changeState(uint8_t state_id)
{
    Global.current_state = state_id;
    Global.audio_test = 0;
    States[Global.current_state].handleEvent(newEvent(EV_ENTER_STATE, 0));
}

/*** Hello World ***/
static void handleEvent_Message(Event_t event)
{
    switch (event.ev_type)
    {
        case EV_ENTER_STATE:
            queueEvent(newEvent(EV_DISPLAY_MODE, DISP_MODE_MESSAGE));
            break;
            
        case EV_INPUT:
            queueEvent(newEvent(EV_CHANGE_STATE, STATE_TIME_CLOCK));
            break;
    }    
}

