#ifndef CLOCK_H
#define	CLOCK_H

#define SEC1    0
#define SEC10   4
#define MIN1    8
#define MIN10   12
#define HOUR1   16
#define HOUR10  20

#define SEC_BITS    0x000000FFul
#define MIN_BITS    0x0000FF00ul
#define HOUR_BITS   0x00FF0000ul
#define MINS_HOURS  0x00FFFF00ul

#define CLOCK_FORWARD 1
#define CLOCK_BACKWARD 0

#include <inttypes.h>

enum CLOCK_ID
{
    TIME_CLOCK,
    ALARM_CLOCK,
    SNOOZE_CLOCK,
    NUM_CLOCKS
};

typedef union
{
    uint32_t time_bits;
    struct
    {
        uint8_t sec_bits;
        uint8_t min_bits;
        uint8_t hour_bits;
        uint8_t _unused;
    };
    struct
    {
        uint8_t sec1   : 4;
        uint8_t sec10  : 4;
        uint8_t min1   : 4;
        uint8_t min10  : 4;
        uint8_t hour1  : 4;
        uint8_t hour10 : 4;
        uint8_t __unused;
    };
} Clock_t;

Clock_t* getClock(uint8_t clock_id);

#endif	/* CLOCK_H */