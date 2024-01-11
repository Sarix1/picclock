#ifndef DISPLAY_H
#define	DISPLAY_H

#include <inttypes.h>
#include "clock.h"
#include "seg7.h"

#define CHAR1     0x000000FE
#define CHAR2     0x0000FE00
#define CHAR3     0x00FE0000
#define CHAR4     0xFE000000
#define DOT_RIGHT 0x00000001
#define DOT_LOWER 0x00000100
#define DOT_UPPER 0x00010000
#define DOT_LEFT  0x01000000

#define DISPLAY_HELLO    = ((SEG7_H << 24) | (SEG7_e << 16) | (SEG7_ll << 8) | SEG7_o)
#define DISPLAY_POLLO    = ((SEG7_P << 24) | (SEG7_o << 16) | (SEG7_ll << 8) | SEG7_o)
#define DISPLAY_ERROR(x) = ((SEG7_E << 24) | (SEG7_r << 16) | (SEG7_r << 8) | seg7[x])

#define DISPLAY_CLOCK   = ((seg7[TimeClock.hour10] << 24) \
                         | (seg7[TimeClock.hour1]  << 16) \
                         | (seg7[TimeClock.min10]  <<  8) \
                         | (seg7[TimeClock.min1]) \
                         | (DOT_LOWER | DOT_UPPER))

#define DISPLAY_ALARM   = ((seg7[AlarmClock.hour10] << 24) \
                         | (seg7[AlarmClock.hour1]  << 16) \
                         | (seg7[AlarmClock.min10]  <<  8) \
                         | (seg7[AlarmClock.min1]) \
                         | (DOT_LOWER | DOT_UPPER))

#define DISPLAY_SECONDS = (seg7[TimeClock.sec10] << 8) \
                        | (seg7[TimeClock.sec1]) \
                        | (DOT_LOWER | DOT_UPPER))

typedef union
{
    uint32_t bits;
    uint8_t digits[4];
} DisplayBits_t;

void displayClock(Clock_t clock);
void refreshDisplay();

extern const uint8_t seg7[10];
extern DisplayBits_t Display;

#endif	/* DISPLAY_H */

