#include "event.h"
#include "global.h"
#include "input.h"
#include "state.h"

// Locals
static EventBus_t EventBus = {0};

// Forward declarations
void handleEvent(Event_t event);
void handleInputEvent(Event_t event);   // from input.c
void handleStateEvent(Event_t event);   // from state.c
void handleClockEvent(Event_t event);   // from clock.c
void handleAlarmEvent(Event_t event);   // from clock.c
void handleAudioEvent(Event_t event);   // from audio.c
void handleDisplayEvent(Event_t event); // from display.c

// Functions
Event_t newEvent(uint8_t ev_type, uint8_t arg)
{
    Event_t event;
    event.ev_type = ev_type;
    event.arg_state = arg;

    return event;
}

void queueEvent(Event_t event)
{
    if (EventBus.count < MAX_EVENTS)
    {
        EventBus.count++;
        EventBus.events[EventBus.head] = event;
                
        EventBus.head++;
        if (EventBus.head >= MAX_EVENTS)
            EventBus.head = 0;
    }
}

void handleEvents()
{
    while (EventBus.count > 0)
    {
        EventBus.count--;
        handleEvent(EventBus.events[EventBus.tail]);

        EventBus.tail++;
        if (EventBus.tail >= MAX_EVENTS)
            EventBus.tail = 0;
    }
}

void handleEvent(Event_t event)
{
    switch (event.ev_type)
    {
        case EV_CHANGE_STATE:
        case EV_ENTER_STATE:
            handleStateEvent(event);
            break;
            
        case EV_INPUT:
            handleInputEvent(event);
            break;
            
        case EV_CLOCK_STEP:
            handleClockEvent(event);
            break;
           
        case EV_ALARM_TIMER:
        case EV_ALARM_START:
        case EV_ALARM_STOP:
        case EV_ALARM_SNOOZE:
            handleAlarmEvent(event);
            break;

        case EV_AUDIO_PLAY:
        case EV_AUDIO_STOP:
            handleAudioEvent(event);
            break;

        case EV_DISPLAY_MODE:
            handleDisplayEvent(event);
            break;
            
        default:
            break;
    }
}
