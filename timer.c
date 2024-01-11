/* 
 * TMR0 is clock source for timer_ms, used for program timing and time clock
 * TMR1 is temporarily used to time notes for audio playback, will change later
 * TMR2 is used with CCP module to produce PWM for audio and LED brightness
 */

#include <xc.h>
#include <pic16f690.h>
#include <inttypes.h>
#include "globals.h"
#include "timer.h"
#include "pins.h"

void initOscillator()
{
    OSCCONbits.SCS  = 1;     // use internal oscillator as system clock
    OSCCONbits.IRCF = 0b110; // select 4 MHz internal oscillator
    // misc
    OSCCONbits.LTS  = 1;     // low frequency internal oscillator is stable
    OSCCONbits.HTS  = 1;     // high frequency internal oscillator is stable
    OSCCONbits.OSTS = 0;     // oscillator start-up time-out status bit
}

// TMR0 Prescale values
/*
000 = 1:2
001 = 1:4
010 = 1:8
011 = 1:16
100 = 1:32
101 = 1:64
110 = 1:128
111 = 1:256
 */

void initTimer0()
{
    /* If Fosc = 4 Mhz, Fcy = Fosc/4 = 1 Mhz
     * If PS = 1:4, TMR0 = Fcy/4 = 250 KHz */
    
    // clock config
    OPTION_REGbits.T0CS = 0;    // 0 = Fcy (Fosc/4), 1 = external from T0CKI
    OPTION_REGbits.PS   = 0b001;// TMR0 prescaler 1:4
    
    // start
    TMR0 = TIMER_RESET_VALUE;
    INTCONbits.T0IF     = 0;    // Clear the interrupt flag
    INTCONbits.T0IE     = 1;    // Enable TMR0 interrupt
}

void initTimer1()
{
    // clock config
    T1CONbits.TMR1CS  = 0;    // 1 = Fcy (Fosc/4), 0 = external from T1CKI
    T1CONbits.T1CKPS  = 0b00; // TMR1 prescaler 1:1
    
    // misc (for external oscillator)
    T1CONbits.nT1SYNC = 0; // 0 = external clock input syncs with internal
    T1CONbits.T1OSCEN = 0; // low-power oscillator is off
    T1CONbits.TMR1GE  = 0; // TMR1 gate disabled
    T1CONbits.T1GINV  = 0; // do not invert TMR1 gate
    
    // start
    TMR1H = 0; // reset TMR1
    TMR1L = 0; // -"-
    T1CONbits.TMR1ON  = 1;  
}

void initTimer2()
{
    // clock config
    T2CONbits.T2CKPS = 0b01;   // TMR2 prescaler 1:4
    T2CONbits.TOUTPS = 0b0000; // TMR2 postscaler 1:1
    
    // start
    TMR2 = 0; // reset TMR2
    T2CONbits.TMR2ON = 1;
}

inline uint16_t getTimer1()
{
    return (uint16_t)(TMR1H << 8) | TMR1L;
}

inline void setTimer1(uint16_t value)
{
    TMR1L = (uint8_t)(value & 0xFF);
    TMR1H = (uint8_t)(value >> 8);
}

void PWM_SetFrequency(uint16_t frequency)
{
    // TMR2 period
    PR2  = (uint8_t)((_XTAL_FREQ / (frequency * 4 * T2CONbits.T2CKPS)) - 1);
    TMR2 = 0; // reset TMR2
}

void initPWM()
{
    initOscillator();   // oscillator provides clock for TMR2
    initTimer2();       // TMR2 is used to generate PWM

    TRIS_PIN(PWM_A)   = 0;          // set P1A as an output
    CCP1CONbits.CCP1M = 0b1100;     // PWM mode with all outputs active high
    CCP1CONbits.DC1B  = 0b11;       // duty cycle LSBs
    CCP1CONbits.P1M   = 0b00;       // single output from P1A
    CCPR1L            = 0b00110111; // duty cycle MSBs
    PWM1CONbits.PDC   = 0b1110000;  // delay for PWM going high = 224 cycles
    PWM1CONbits.PRSEN = 1;          // auto-shutdown sets ECCPASE=0, restarts PWM
}

void quitPWM()
{
    ;
}

void PWM_SetDutyCycle(uint16_t duty_cycle)
{
    CCPR1L = ((duty_cycle >> 2) & 0xFF);    // 8 x MSBs
    CCP1CONbits.DC1B = (duty_cycle & 0x03); // 2 x LSBs
}

inline void PWM_SetDutyPercent(uint8_t percent)
{
    PWM_SetDutyCycle((percent * (PR2+1)) / 25);
}

inline void PWM_SetDutyRatio(float ratio)
{
    PWM_SetDutyCycle((uint16_t)(ratio * 4 * (PR2+1)));
}
