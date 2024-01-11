#include "display.h"
#include "shift.h"
#include "globals.h"

const uint8_t seg7[10] =
{       
    SEG7_0,
    SEG7_1,
    SEG7_2,
    SEG7_3,
    SEG7_4,
    SEG7_5,
    SEG7_6,
    SEG7_7,
    SEG7_8,
    SEG7_9 
};

DisplayBits_t Display = {0};

void displayClock(Clock_t clock)
{
    Display.digits[0] = seg7[clock.hour10];
    Display.digits[1] = seg7[clock.hour1];
    Display.digits[2] = seg7[clock.min10];
    Display.digits[3] = seg7[clock.min1];
}

void displayMessage(uint32_t message_bits)
{
    Display.bits = message_bits;
}

inline void displayBlink()
{
    if (Globals.blink.digits_left && !(Globals.status.timer_quarter & Globals.blink.digits_left))
        Display.bits &= ~(CHAR1|CHAR2);
                
    if (Globals.blink.digits_right && !(Globals.status.timer_quarter & Globals.blink.digits_right))
        Display.bits &= ~(CHAR3|CHAR4);
                
    if (Globals.blink.dot_left && !(Globals.status.timer_quarter & Globals.blink.dot_left))
        Display.bits &= ~(DOT_LEFT);
    
    if (Globals.blink.dot_right && !(Globals.status.timer_quarter & Globals.blink.dot_right))
        Display.bits &= ~(DOT_RIGHT);
    
    if (Globals.blink.dot_upper && !(Globals.status.timer_quarter & Globals.blink.dot_upper))
        Display.bits &= ~(DOT_UPPER);
    
    if (Globals.blink.dot_lower && !(Globals.status.timer_quarter & Globals.blink.dot_lower))
        Display.bits &= ~(DOT_LOWER);
}

void refreshDisplay()
{   
    displayBlink();
    shiftOut8(Display.digits[3]);
    shiftOut8(Display.digits[2]);
    shiftOut8(Display.digits[1]);
    shiftOut8(Display.digits[0]);
    shiftOutLatch();
}
