#include <xc.h>
#include <pic16f690.h>
#include <inttypes.h>
#include "pins.h"
#include "global.h"
#include "timer.h"

// Forward declarations
static void inline setTimer1(uint16_t value);

// Functions
void initOscillator()
{
    // system clock
    // clock source
    OSCCONbits.SCS  = 1; // 0 = external on OSC1 and OSC2, 1 = internal
    // internal oscillator frequency selection
    // 000 = 31 KHz    001 = 125 kHz   010 = 250 kHz   011 = 500 kHz
    // 100 = 1 MHz     101 = 2 MHz     110 = 4 Mhz     111 = 8 MHz
    OSCCONbits.IRCF = 0b111; // 8 MHz internal oscillator
    // misc
    OSCCONbits.LTS  = 1; // low frequency internal oscillator is stable
    OSCCONbits.HTS  = 1; // high frequency internal oscillator is stable
    OSCCONbits.OSTS = 0; // oscillator start-up time-out status bit
}

void initTimer0()
{
    // If Fosc = 4 Mhz, Fcy = Fosc/4 = 1 MHz
    // If PS = 1:2, TMR0 = Fcy/4 = 250 KHz
    // TMR0 rollover and interrupt at 255 -> 0
    // If reset at 256-6, interrupts occur at 1 KHz
    
    // clock source
    OPTION_REGbits.T0CS = 0; // 0 = Fcy (Fosc/4), 1 = external from T0CKI
    OPTION_REGbits.PSA = 1; // Prescaler assigned to TMR0 (0) / WDT (1))
    // TMR0 Prescale values
    // 000 = 1:2   001 = 1:4   010 = 1:8   011 = 1:16
    // 100 = 1:32  101 = 1:64  110 = 1:128 111 = 1:256
    OPTION_REGbits.PS = 0b000; // TMR0 prescaler 1:2 (actually 1:1 because PSA = 1))
    // reset
    TMR0 = TIMER0_RESET_VALUE;
    INTCONbits.T0IF = 0;
    INTCONbits.T0IE = 0;
}

void initTimer1()
{
    // clock source
    T1CONbits.TMR1CS = 1; // 0 = Fcy (Fosc/4), 1 = external from T1CKI
    // TMR1 Prescale values: 00 = 1:1, 01 = 1:2, 10 = 1:4, 11 = 1:8
    T1CONbits.T1CKPS = 0b00; // TMR1 prescaler 1:4
    // external and internal oscillator are: 0 = synchronous, 1 = asynchronous
    // NOTE: TMR1 must be asynchronous if sourced by an external oscillator
    // otherwise it will not keep running during sleep --> no clock interrupts
    T1CONbits.nT1SYNC = 1;
    // misc
    T1CONbits.T1OSCEN = 1; // low-power oscillator enable
    T1CONbits.TMR1GE = 0; // TMR1 gate disabled
    T1CONbits.T1GINV = 0; // do not invert TMR1 gate
    PIE1bits.TMR1IE = 1;
    INTCONbits.PEIE = 1;
    
    // reset
    setTimer1(TIMER1_RESET_VALUE);
}

void initTimer2()
{
    // clock config
    T2CONbits.T2CKPS = 0b01;   // TMR2 prescaler 1:4
    T2CONbits.TOUTPS = 0b0000; // TMR2 postscaler 1:1
    // reset
    TMR2 = 0;
    T2CONbits.TMR2ON = 0;
}

void startTimer0()
{
    TMR0 = TIMER0_RESET_VALUE;
    INTCONbits.T0IF = 0; // Clear the interrupt flag
    INTCONbits.T0IE = 1; // Enable TMR0 interrupt
}

void startTimer1()
{
    TMR1H = 0;
    TMR1L = 0;
    T1CONbits.TMR1ON  = 1;  
}

void startTimer2()
{
    TMR2 = 0;
    T2CONbits.TMR2ON = 1;
}

/*
void stopTimer0()
{
    INTCONbits.T0IF = 0; // Clear the interrupt flag
    INTCONbits.T0IE = 0; // Disable TMR0 interrupt
}

void stopTimer1()
{
    T1CONbits.TMR1ON = 0;  
}
 */

void stopTimer2()
{
    T2CONbits.TMR2ON = 0;
}

/*
uint16_t getTimer1()
{
    return (uint16_t)(TMR1H << 8) | TMR1L;
}
*/

static void inline setTimer1(uint16_t value)
{
    TMR1L = (uint8_t)value;
    TMR1H = (uint8_t)(value >> 8);
}

void timer0Interrupt()
{
    // locals
    static struct counter_s
    {
        uint8_t value : 3;
        uint8_t unused : 5;
    } counter = {0};
    
    // reset
    INTCONbits.T0IF = 0;
    TMR0 = TIMER0_RESET_VALUE;
    Global.t0_expire = 1;
    
    // increment
    counter.value++;
    if (counter.value == 0)
        Global.timer_ms++;
}

void timer1Interrupt()
{
    // reset
    PIR1bits.T1IF = 0;
    setTimer1(TIMER1_RESET_VALUE);
    
    // increment
    Global.timer_q++;
    if (Global.timer_q == 0)
        Global.sec_tic_calls++;
}
