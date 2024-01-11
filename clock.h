#ifndef CLOCK_H
#define	CLOCK_H

#include <inttypes.h>

enum TIME_UNITS
{
    SEC1   = 0,
    SEC10  = 4,
    MIN1   = 8,
    MIN10  = 12,
    HOUR1  = 16,
    HOUR10 = 18
};

enum CLOCK_ID
{
    TIME_CLOCK,
    ALARM_CLOCK,
    SNOOZE_CLOCK
};

typedef union
{
    uint32_t time;
    
    struct
    {
        uint8_t sec_bits;
        uint8_t min_bits;
        uint8_t hour_bits;
    };
    
    struct
    {
        uint8_t sec1   : 4;
        uint8_t sec10  : 4;
        uint8_t min1   : 4;
        uint8_t min10  : 4;
        uint8_t hour1  : 4;
        uint8_t hour10 : 4;
    };
} Clock_t;

void clockForward(Clock_t* clock, uint8_t unit);
void clockBackward(Clock_t* clock, uint8_t unit);

extern Clock_t TimeClock;
extern Clock_t AlarmClock;
extern Clock_t SnoozeClock;

#endif	/* CLOCK_H */

