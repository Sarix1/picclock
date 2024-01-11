/* 
 * PIC Digital Alarm Clock
 *
 * Created by Sari Jokinen in January 2023
 * sari.katariina.jokinen@protonmail.com
 * github.com/sarix1
 * 
 * Microcontroller Unit Pinout
 * ***************************
 *                         _______     _______
 *                        |       \___/       |
 *                  +5V --|1 VDD        VSS 20|-- 0V
 *                        |                   |
 *               Xtal A ->|2 RA5        RA0 19|-- Unused
 *                        |   osc1            |
 *               Xtal B ->|3 RA4        RA1 18|-- Unused
 *                        |   osc2            |
 *                Reset ->|4 RA3        RA2 17|-- Unused
 *                        |   mclr       in   |
 * PWM / Speaker Signal <~|5 RC5        RC0 16|-> PISO Clock
 *                        |   p1a        out  |
 *           SIPO Latch <-|6 RC4        RC1 15|-> PISO Latch
 *                        |   out        out  |
 * PWM / Speaker Volume <~|7 RC3        RC2 14|~> PWM / LED Brightness
 *                        |   p1c        p1d  |
 *            SIPO Data <-|8 RC6        RB4 13|~< ADC / Photoresistor
 *                        |   out        an10 |
 *           SIPO Clock <-|9 RC7        RB5 12|<- PISO Data
 *                        |   out        in   |
 *       Rot. Encoder B ->|10 RB7       RB6 11|<- Rot. Encoder A
 *                        |    in        in   |
 *                        '-------------------'
 *                              PIC16F690
 *
 * Rot. Encoder Pinout
 * *******************
 *        ______    
 *   A <-|  __  |-> Button: Min/Hour
 * Gnd --| (__) |
 *   B <-|______|-- Gnd
 * 
 *
 * Parallel-In, Serial-Out Shift Registers Pinout
 * **********************************************
 *                      ____   ____
 *      Button Input ->|PI8 \_/ VDD|-- +5V
 *            Unused <-|Q6      PI7|<- Button Input 
 *         PISO Data <-|Q8      PI6|<- Button Input
 *      Button Input ->|PI4     PI5|<- Button Input
 *      Button Input ->|PI3      Q7|-> Unused
 *      Button Input ->|PI2     SER|<- CD4021B 2 Q8 <-.
 *      Button Input ->|PI1     CLK|<- PISO Clock     |
 *                0V --|VSS     LAT|<- PISO Latch     |
 *                     '-----------'                  |
 *                       CD4021B 1                    |
 * .--------------------------------------------------'
 * |                    ____   ____
 * |          Unused ->|PI8 \_/ VDD|-- +5V
 * |          Unused <-|Q6      PI7|<- Unused
 * '-- CD4021B 1 SER <-|Q8      PI6|<- Unused
 *            Unused --|PI4     PI5|<- Unused
 *      Button Input ->|PI3      Q7|-> Unused
 *      Button Input ->|PI2     SER|-> Unused
 *      Button Input ->|PI1     CLK|<- PISO Clock
 *                0V --|VSS     LAT|<- PISO Latch
 *                     '-----------'
 *                       CD4021B 2
 *
 *
 * Serial-In, Parallel-Out Shift Registers Pinout
 * **********************************************
 *                      ____   ____
 *        SIPO Latch ->|LAT \_/ VDD|-- +5V
 *         SIPO Data ->|DAT      OE|<- PWM / LED Brightness
 *        SIPO Clock ->|CLK      Q5|-> 7-Seg 1 E
 *         7-Seg 1 A <-|Q1       Q6|-> 7-Seg 1 F
 *         7-Seg 1 B <-|Q2       Q7|-> 7-Seg 1 G
 *         7-Seg 1 C <-|Q3       Q8|-> Unused
 *         7-Seg 1 D <-|Q4      Q'S|-> Unused
 *                0V --|VSS      QS|-> CD4094 2 DAT --.
 *                     '-----------'                  |
 *                       CD4094B 1                    |
 * .--------------------------------------------------'
 * |                    ____   ____
 * |      SIPO Latch ->|LAT \_/ VDD|-- +5V
 * '--> CD4094B 1 QS ->|DAT      OE|<- PWM / LED Brightness
 *        SIPO Clock ->|CLK      Q5|-> 7-Seg 2 E
 *         7-Seg 2 A <-|Q1       Q6|-> 7-Seg 2 F
 *         7-Seg 2 B <-|Q2       Q7|-> 7-Seg 2 G
 *         7-Seg 2 C <-|Q3       Q8|-> Unused
 *         7-Seg 2 D <-|Q4      Q'S|-> Unused
 *                0V --|VSS      QS|-> CD4094 3 DAT --.
 *                     '-----------'                  |
 *                       CD4094B 2                    |
 * .--------------------------------------------------'
 * |                    ____   ____
 * |      SIPO Latch ->|LAT \_/ VDD|-- +5V
 * '--> CD4094B 2 QS ->|DAT      OE|<- PWM / LED Brightness
 *        SIPO Clock ->|CLK      Q5|-> 7-Seg 3 E
 *         7-Seg 3 A <-|Q1       Q6|-> 7-Seg 3 F
 *         7-Seg 3 B <-|Q2       Q7|-> 7-Seg 3 G
 *         7-Seg 3 C <-|Q3       Q8|-> Unused
 *         7-Seg 3 D <-|Q4      Q'S|-> Unused
 *                0V --|VSS      QS|-> CD4094 4 DAT --.
 *                     '-----------'                  |
 *                       CD4094B 3                    |
 * .--------------------------------------------------'
 * |                    ____   ____
 * |      SIPO Latch ->|LAT \_/ VDD|-- +5V
 * '--> CD4094B 3 QS ->|DAT      OE|<- PWM / LED Brightness
 *        SIPO Clock ->|CLK      Q5|-> 7-Seg 4 E
 *         7-Seg 4 A <-|Q1       Q6|-> 7-Seg 4 F
 *         7-Seg 4 B <-|Q2       Q7|-> 7-Seg 4 G
 *         7-Seg 4 C <-|Q3       Q8|-> Unused
 *         7-Seg 4 D <-|Q4      Q'S|-> Unused
 *                0V --|VSS      QS|-> Unused
 *                     '-----------'                  
 *                       CD4094B 4
 */

// includes
#include <xc.h>
#include <pic16f690.h>
#include <inttypes.h>

#include "globals.h"
#include "pins.h"
#include "state.h"
#include "input.h"
#include "timer.h"
#include "clock.h"
#include "display.h"
#include "audio.h"

// configs
#pragma config FOSC  = INTRCIO // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE  = OFF     // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF     // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON      // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP    = OFF     // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD   = OFF     // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF     // Brown-out Reset Selection bits (BOR enabled)
#pragma config IESO  = ON      // Internal External Switchover bit (Internal External Switchover mode is enabled)
#pragma config FCMEN = ON      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)

// globals
Globals_t Globals = {0};
uint32_t loop_next;

// functions
void resetAllPins()
{     
    TRISA   = 0x00;
    TRISB   = 0x00;
    TRISC   = 0x00;
    PORTA   = 0x00;
    PORTB   = 0x00;
    PORTC   = 0x00;
    ANSEL   = 0b00000000;
    ANSELH  = 0b00000000;
}

void init()
{
    // global interrupt disable
    INTCONbits.GIE = 0;
    
    // initialize    
    resetAllPins();
    initRotaryEncoder();
    initShiftOut();
    initShiftIn();
    changeState(STATE_HELLO);
    Globals.status.update_call = 1;
    loop_next = INIT_DELAY_MS;
            
    // global interrupt enable
    INTCONbits.GIE = 1;
}

inline void timerInterrupt()
{
    TMR0 = TIMER_RESET_VALUE;

    Globals.status.timer_ms++;
    Globals.status.timer_ints++;

    if (Globals.status.timer_ints >= 250)
    {
        Globals.status.timer_ints = 0;
        Globals.status.timer_quarter++;
        Globals.status.update_call = 1;

        if (Globals.status.timer_quarter >= 4)
        {
            Globals.status.timer_quarter = 0;
            Globals.status.sec_tic_calls++;
        }
    }
}

inline void encoderInterrupt()
{
    if (Globals.input.enc_head+1 != Globals.input.enc_tail)
    {
        uint8_t enc_bits = PORT_LETTER(ENC_PIN_A) &
                          (PORT_NTH_BIT(ENC_PIN_A) | PORT_NTH_BIT(ENC_PIN_B));
        
        switch (enc_bits)
        {
            // clockwise if only 1 bit is set
            case PORT_NTH_BIT(ENC_PIN_A):
            case PORT_NTH_BIT(ENC_PIN_B):
                Globals.input.enc_buffer |= (1 << Globals.input.enc_head);
                break;
            // counter-clockwise if 2 or 0 bits are set
            case PORT_NTH_BIT(ENC_PIN_A) | PORT_NTH_BIT(ENC_PIN_B):
            case 0:
                Globals.input.enc_buffer &= ~(1 << Globals.input.enc_head);
                break;
        }
        Globals.input.enc_head++;
    }
    Globals.status.update_call = 1;
}

void isr(void) __interrupt()
{
    if (INTCONbits.T0IF)
    {
        INTCONbits.T0IF = 0;
        timerInterrupt();
    }
    
    if (INTCONbits.RABIF)
    {
        encoderInterrupt();
        INTCONbits.RABIF = 0;
    }
}

inline void pollInput()
{
    Globals.input.button_bits = readButtons();
    
    if (Globals.input.button_bits != Globals.input.button_last)
    {
        uint16_t changed_bits = Globals.input.button_bits ^ Globals.input.button_last;
        handleInput(changed_bits);
        Globals.status.update_call = 1;
    }   
    Globals.input.button_last = Globals.input.button_bits;
}

inline void handleError()
{
    if (Globals.status.error)
    {
        Globals.status.error = 0;
        displayMessage(DISPLAY_ERROR(Globals.status.error));
        refreshDisplay();
        loop_next += ERROR_DELAY_MS;
    }
}


inline void updateTimeClock()
{
    while (Globals.status.sec_tic_calls > 0)
    {
        Globals.status.sec_tic_calls--;
        if (Globals.status.clock_ticking == 1)
        {
            Globals.status.update_call = 1;
            clockForward(&TimeClock, SEC1);
        }
    }
}

inline void updateState()
{
    if (Globals.status.update_call == 1)
    {
        Globals.status.update_call = 0;
        handleInput();
        stateShow();
        refreshDisplay();
    }
}

inline void updateAudio()
{
    static uint32_t audio_next = 0;
    if (Globals.audio.playing && Globals.status.timer_ms >= audio_next)
        // playNote returns duration value of played note
        audio_next += (uint32_t)(playNote()) * NOTE_DURATION_MULTIPLIER;
}

inline void updateSensorData()
{
    static uint32_t sensor_next = 0;
    if (Globals.status.timer_ms >= sensor_next)
    {
        sensor_next += SENSOR_INTERVAL_MS;
        //getSensorData();
    }
}

inline void loop()
{
    updateTimeClock();
    pollInput();
    handleError();
    updateState();
    updateAudio();
    updateSensorData();
}

void main(void)
{
    // delay the first loop iteration by 1 second from power-up
    init();
    
    // the main loop runs at given intervals, then sends the CPU to sleep
    // timer (1000 Hz) and encoder interrupts wake up the CPU from sleep
    while (1)
    {
        if (Globals.status.timer_ms >= loop_next)
        {
            loop_next += LOOP_INTERVAL_MS;
            loop();
        }
        __delay_ms(10);
        // Change to SLEEP(); after installing external oscillator on TMR1
        //SLEEP();
    }
}
