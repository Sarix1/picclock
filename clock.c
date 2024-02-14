#include "clock.h"
#include "event.h"
#include "global.h"
#include "display.h"
#include "input.h"

// Locals
static Clock_t Clocks[NUM_CLOCKS] = {0};

// Forward declarations
void clockForward(uint8_t clock_id, uint8_t unit);
void clockBackward(uint8_t clock_id, uint8_t unit);
static void snooze();

// Functions
void updateClock()
{
    while (Global.sec_tic_calls > 0)
    {
        Global.sec_tic_calls--;
        if (Global.clock_ticking == 1)
            clockForward(TIME_CLOCK, SEC1);
    }
}

void handleClockEvent(Event_t event)
{
    switch (event.clock_ev.ev_type)
    {
        case EV_CLOCK_STEP:
            if (event.clock_ev.arg_direction == CLOCK_FORWARD)
                clockForward(event.clock_ev.arg_clock_id, event.clock_ev.arg_time_unit);
            else // CLOCK_BACKWARD
                clockBackward(event.clock_ev.arg_clock_id, event.clock_ev.arg_time_unit);
            break;
    }
}

void handleAlarmEvent(Event_t event)
{
    switch (event.ev_type)
    {
        // toggle alarm timer on or off
        case EV_ALARM_TIMER:
            Global.alarm_timer_on = event.arg_state;
            if (event.arg_state == 0)
            {
                Global.alarm_snoozing = 0;
                if (Global.alarming == 1)
                    queueEvent(newEvent(EV_ALARM_STOP, 0));
            }
            break;

        case EV_ALARM_START:
            Global.alarming = 1;
            break;

        case EV_ALARM_STOP:
            Global.alarming = 0;
            break;
            
        case EV_ALARM_SNOOZE:
            snooze();
            break;
            
        default:
            break;
    }
}

static void snooze()
{
    uint8_t i;
    
    Global.alarm_snoozing = 1;
    Clocks[SNOOZE_CLOCK].time_bits = Clocks[TIME_CLOCK].time_bits;
    for (i = 0; i < Global.snooze_minutes; i++)
        clockForward(SNOOZE_CLOCK, MIN1);

    queueEvent(newEvent(EV_ALARM_STOP, 0));
}

static void minuteTick()
{
    uint8_t should_alarm = 0;
    
    if (Global.alarm_timer_on == 1)
    {
        if ((Clocks[TIME_CLOCK].hour_bits == Clocks[ALARM_CLOCK].hour_bits &&
            Clocks[TIME_CLOCK].min_bits == Clocks[ALARM_CLOCK].min_bits))
            should_alarm = 1;

        if (Global.alarm_snoozing == 1 &&
            (Clocks[TIME_CLOCK].hour_bits == Clocks[SNOOZE_CLOCK].hour_bits) &&
            (Clocks[TIME_CLOCK].min_bits == Clocks[SNOOZE_CLOCK].min_bits))
            should_alarm = 1;
            
        if (should_alarm == 1)
            queueEvent(newEvent(EV_ALARM_START, 0));
    }
}

Clock_t* getClock(uint8_t clock_id)
{
    if (clock_id >= 0 && clock_id < NUM_CLOCKS)
        return &Clocks[clock_id];
    else
        return 0;
}

static inline uint8_t getClockDigit(Clock_t* clock, uint8_t time_unit)
{
    return (((clock->time_bits) >> time_unit) & 0xF);
}

static inline void setClockDigit(Clock_t* clock, uint8_t time_unit, uint8_t value)
{
    clock->time_bits &= ~(((uint32_t)0xF) << time_unit);
    clock->time_bits |= ((uint32_t)value) << time_unit;
}

void clockForward(uint8_t clock_id, uint8_t time_unit)
{
    uint8_t minute = Clocks[TIME_CLOCK].min_bits;
    
    Clock_t* clock = getClock(clock_id);
    uint8_t value = getClockDigit(clock, time_unit) + 1;
    setClockDigit(clock, time_unit, value);
    
    switch (time_unit)
    {
        case SEC1:  if (clock->sec1 >= 10)  { clock->sec1 = 0;  clock->sec10++; } else break;
        case SEC10: if (clock->sec10 >= 6)  { clock->sec10 = 0; clock->min1++;  } else break;
        case MIN1:  if (clock->min1 >= 10)  { clock->min1 = 0;  clock->min10++; } else break;
        case MIN10: if (clock->min10 >= 6)  { clock->min10 = 0; clock->hour1++; } else break;
        case HOUR1: if (clock->hour1 >= 10) { clock->hour1 = 0; clock->hour10++;}
            else if (clock->hour1 >= 4 && clock->hour10 >= 2)
                { clock->hour1 = 0; clock->hour10 = 0; }
            else break;
        case HOUR10: if (clock->hour10 >= 3) { clock->hour10 = 0; } else break;
        default:
            break;
    }
    
    if (minute != Clocks[TIME_CLOCK].min_bits)
        minuteTick();
}

void clockBackward(uint8_t clock_id, uint8_t time_unit)
{
    Clock_t* clock = getClock(clock_id);
    uint8_t value = (getClockDigit(clock, time_unit) - 1) & 0xF;
    setClockDigit(clock, time_unit, value);
    
    if (value == 0xF)
    {
        switch (time_unit)
        {
            case SEC1:   if (clock->sec1 == 0xF)   { clock->sec1 = 9;   clock->sec10--;  } else break;
            case SEC10:  if (clock->sec10 == 0xF)  { clock->sec10 = 5;  clock->min1--;   } else break;
            case MIN1:   if (clock->min1 == 0xF)   { clock->min1 = 9;   clock->min10--;  } else break;
            case MIN10:  if (clock->min10 == 0xF)  { clock->min10 = 5;  clock->hour1--;  } else break;
            case HOUR1:  if (clock->hour1 == 0xF)  { clock->hour1 = 9;  clock->hour10--; } else break;
            case HOUR10: if (clock->hour10 == 0xF) { clock->hour10 = 2; clock->hour1 = 3;} else break;
            default: break;
        }
    }
}

/*** Time Clock ***/
void handleEvent_TimeClock(Event_t event)
{
    switch (event.ev_type)
    {
        case EV_ENTER_STATE:
            queueEvent(newEvent(EV_DISPLAY_MODE, DISP_MODE_TIME_CLOCK));
            Global.clock_ticking = 1;
            break;
    }    
}

/*** Time Set ***/
void handleEvent_TimeSet(Event_t event)
{
    switch (event.ev_type)
    {
        case EV_ENTER_STATE:
            queueEvent(newEvent(EV_DISPLAY_MODE, DISP_MODE_TIME_SET));
            break;
            
        case EV_INPUT:
            switch (event.input_ev.arg_button_id)
            {
                case BTN_ENCODER:
                    Global.time_set_minutes ^= 1;
                    break;
                case BTN_ENCODER_CW:
                    clockForward(TIME_CLOCK, (Global.time_set_minutes ? MIN1 : HOUR1));
                    break;
                case BTN_ENCODER_CCW:
                    clockBackward(TIME_CLOCK, (Global.time_set_minutes ? MIN1 : HOUR1));
                    break;
            }
            break;
    }    
}

/*** Alarm Set ***/
void handleEvent_AlarmSet(Event_t event)
{
    switch (event.ev_type)
    {
        case EV_ENTER_STATE:
            queueEvent(newEvent(EV_DISPLAY_MODE, DISP_MODE_ALARM_SET));
            break;
            
        case EV_INPUT:
            switch (event.input_ev.arg_button_id)
            {
                case BTN_ENCODER:
                    Global.time_set_minutes ^= 1;
                    break;
                case BTN_ENCODER_CW:
                    clockForward(ALARM_CLOCK, (Global.time_set_minutes ? MIN1 : HOUR1));
                    break;
                case BTN_ENCODER_CCW:
                    clockBackward(ALARM_CLOCK, (Global.time_set_minutes ? MIN1 : HOUR1));
                    break;
            }
            break;
    }    
}

/*** Seconds Set ***/
void handleEvent_SecSet(Event_t event)
{
    switch (event.ev_type)
    {
        case EV_ENTER_STATE:
            queueEvent(newEvent(EV_DISPLAY_MODE, DISP_MODE_SEC_SET));
            break;
            
        case EV_INPUT:
            switch (event.input_ev.arg_button_id)
            {
                case BTN_ENCODER:
                    Global.clock_ticking ^= 1;
                    break;
                case BTN_ENCODER_CW:
                    clockForward(TIME_CLOCK, SEC1);
                    break;
                case BTN_ENCODER_CCW:
                    clockBackward(TIME_CLOCK, SEC1);
                    break;
            }
            break;
    }    
}

/*** Snooze Set ***/
void handleEvent_SnoozeSet(Event_t event)
{
    switch (event.ev_type)
    {
        case EV_ENTER_STATE:
            queueEvent(newEvent(EV_DISPLAY_MODE, DISP_MODE_SNOOZE_SET));
            break;
            
        case EV_INPUT:
            switch (event.input_ev.arg_button_id)
            {
                case BTN_ENCODER:
                    break;
                case BTN_ENCODER_CW:
                    if (Global.snooze_minutes < MAX_SNOOZE)
                        Global.snooze_minutes++;
                    break;
                case BTN_ENCODER_CCW:
                    if (Global.snooze_minutes > MIN_SNOOZE)
                        Global.snooze_minutes--;
                    break;
            }
            break;
    }    
}