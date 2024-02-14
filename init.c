#include <xc.h>
#include "pins.h"
#include "timer.h"
#include "global.h"

// Forward declarations
// from timer.c
void initOscillator();
void initTimer0();
void initTimer1();
void initTimer2();
// from shift.c
void initShiftOut();
void initShiftIn();
// from input.c
void initRotaryEncoder();
// from sensor.c
void initDHT11();
// from state.c
void initStates();

// Functions
static inline void resetAllPins()
{     
    TRISA   = 0x00;
    TRISB   = 0x00;
    TRISC   = 0x00 | PORT_NTH_BIT(DHT11_PIN);
    PORTA   = 0x00 | PORT_NTH_BIT(SIPO_OE);
    PORTB   = 0x00;
    PORTC   = 0x00;
    ANSEL   = 0b00000000;
    ANSELH  = 0b00000000;
}

void init()
{
    resetAllPins();
    initDHT11();
    initRotaryEncoder();
    initShiftOut();
    initShiftIn();
    initOscillator();
    initTimer0();
    initTimer1();
    initTimer2();
    
    INTCONbits.GIE = 1;
    Global.clock_ticking    = 1;
    Global.snooze_minutes   = DEFAULT_SNOOZE;
    Global.audio_volume     = DEFAULT_VOLUME;
    //Global.user_brightness  = DEFAULT_BRIGHTNESS;
    
    startTimer0();
    startTimer1();
    initStates();
}