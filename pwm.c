#include <xc.h>
#include "pwm.h"
#include "pins.h"
#include "timer.h"

// Forward declarations
void PWM_SetFrequency(uint16_t frequency);
void PWM_SetDutyPercent(uint8_t percent);

// Functions
void initPWM()
{
    TRIS_PIN(PWM_A)   = 0;          // set P1A as an output
    
    CCP1CONbits.CCP1M = 0b1100;     // PWM mode with all outputs active high
    CCP1CONbits.P1M   = 0b00;       // single output from P1A
    PWM1CONbits.PRSEN = 1;          // auto-shutdown sets ECCPASE=0, restarts PWM
    
    CCPR1L            = 0b11111111; // duty cycle MSBs
    CCP1CONbits.DC1B  = 0b11;       // duty cycle LSBs
    //PWM1CONbits.PDC   = 0b1110000;  // delay for PWM going high = 224 cycles (what did I need this for?)
    
    PWM_SetFrequency(0);
    PWM_SetDutyCycle(500);
    
    startTimer2();
}

void quitPWM()
{
    TRIS_PIN(PWM_A) = 1; // set P1A as an input
    stopTimer2();
}

void PWM_SetFrequency(uint16_t frequency)
{
    // TMR2 period
    PR2  = (uint8_t)((_XTAL_FREQ / (frequency * 4 * T2CONbits.T2CKPS)) - 1);
    TMR2 = 0; // reset TMR2
}

void PWM_SetDutyCycle(uint16_t duty_cycle)
{
    CCPR1L = ((duty_cycle >> 2) & 0xFF);    // 8 x MSBs
    CCP1CONbits.DC1B = (duty_cycle & 0x03); // 2 x LSBs
}

/*
void PWM_SetDutyPercent(uint8_t percent)
{
    PWM_SetDutyCycle((percent * (PR2+1)) / 25);
}

void PWM_SetDutyRatio(float ratio)
{
    PWM_SetDutyCycle((uint16_t)(ratio * 4 * (PR2+1)));
}
 */