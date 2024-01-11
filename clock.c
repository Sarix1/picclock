#include <inttypes.h>
#include "clock.h"

Clock_t TimeClock = {0};
Clock_t AlarmClock = {0};
Clock_t SnoozeClock = {0};

inline uint8_t getClockValue(Clock_t* clock, uint8_t unit)
{
    return (((clock->time) >> (unit)) & 0xF);
}

inline uint8_t setClockValue(Clock_t* clock, uint8_t unit, uint8_t value)
{
    clock->time &= (~(0xF << unit));
    clock->time |= (value << unit);
}

void clockForward(Clock_t* clock, uint8_t unit)
{
    uint8_t value = getClockValue(clock, unit) + 1;
    setClockValue(clock, unit, value);
    
    switch (unit)
    {
        case SEC1:  if (clock->sec1 >= 10)  { clock->sec1  = 0; clock->sec10++; }
        case SEC10: if (clock->sec10 >= 6)  { clock->sec10 = 0; clock->min1++;  }
        case MIN1:  if (clock->min1 >= 10)  { clock->min1  = 0; clock->min10++; }
        case MIN10: if (clock->min10 >= 6)  { clock->min10 = 0; clock->hour1++; }
        
        case HOUR1: if (clock->hour1 >= 10) { clock->hour1 = 0; clock->hour10++; }
               else if (clock->hour1 >= 4 && clock->hour10 >= 2)
                                            { clock->hour1 = 0; clock->hour10 = 0; }
            break;
            
        case HOUR10: if (clock->hour10 >= 3) {clock->hour10 = 0; }
        
        default:
            break;
    }
}

void clockBackward(Clock_t* clock, uint8_t unit)
{
    uint8_t value = (getClockValue(clock, unit) - 1) & 0xF;
    setClockValue(clock, unit, value);
    
    if (value == 0xF)
    {
        switch (unit)
        {
            case SEC1:  if (clock->sec1 == 0xF)    { clock->sec1 = 9;   clock->sec10--;  }
            case SEC10: if (clock->sec10 == 0xF)   { clock->sec10 = 5;  clock->min1--;   }
            case MIN1:  if (clock->min1 == 0xF)    { clock->min1 = 9;   clock->min10--;  }
            case MIN10: if (clock->min10 == 0xF)   { clock->min10 = 5;  clock->hour1--;  }
            case HOUR1: if (clock->hour1 == 0xF)   { clock->hour1 = 9;  clock->hour10--; }
            case HOUR10: if (clock->hour10 == 0xF) { clock->hour10 = 2; clock->hour1 = 3; }
            
            default:
                break;
        }
    }
}
