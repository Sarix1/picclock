#include <inttypes.h>
#include "globals.h"
#include "input.h"
#include "pins.h"
#include "shift.h"
#include "state.h"

uint16_t readButtons()
{
    uint16_t bits = 0; 
    shiftInLatch();
    bits |= shiftIn8_reverse();
    bits |= shiftIn8_reverse() << 8;
    
    return bits;
}

void handleStateButton(uint16_t bits)
{
    if (!(bits & (bits-1))) // is only one bit set?
    {
        switch (bits)
        {
            case BTN_ALARM_SET:
                if (Globals.status.alarming == 1)
                    alarmOff();
                else
                    changeState(STATE_ALARM_SET);
                break;
                
            case BTN_TIME_SET:      changeState(STATE_TIME_SET);    break;
            case BTN_SEC_SET:       changeState(STATE_SEC_SET);     break;
            case BTN_TEMP_SHOW:     changeState(STATE_TEMP_SHOW);   break;
            case BTN_VOLUME_SET:    changeState(STATE_VOLUME_SET);  break;
            case BTN_TUNE_SET:      changeState(STATE_TUNE_SET);    break;
            case BTN_BRIGHT_SET:    changeState(STATE_BRIGHT_SET);  break;
            default:
                Globals.status.error = ERROR_INPUT_INVALID;
                break;
        }
    }
    else
        changeState(STATE_TIME_CLOCK);
}

void handleInput(uint16_t changed_bits)
{
    // handle state buttons; only one can be pressed at a time
    if (changed_bits & BTN_STATE_MASK)
        handleStateButton(Globals.input.button_bits & BTN_STATE_MASK);
    else
        changeState(STATE_TIME_CLOCK);
    
    // handle the snooze button
    if (Globals.input.button_bits & changed_bits & BTN_SNOOZE)
    {
        Globals.status.snoozing = 1;
        alarmOff();
        snooze();
    }
    
    // handle the rotary encoder push-button
    if (Globals.input.button_bits & BTN_ENCODER)
        stateHandleEncoderInput(ENC_PUSH_BUTTON);
    
    // handle rotary encoder turns
    while (Globals.input.enc_tail != Globals.input.enc_head)
    {
        uint8_t bit = !!(Globals.input.enc_buffer & (1 << Globals.input.enc_tail));
        Globals.input.enc_tail++;
        stateHandleEncoderInput(bit);
    }
}

void initRotaryEncoder()
{
    OPTION_REGbits.nRABPU = 0; // weak pull-up enable
    INTCONbits.RABIE      = 1; // PORTA/B interrupt on change enable
    
    IOC_PIN(ENC_PIN_A)  = 1; // interrupt-on-change on
    IOC_PIN(ENC_PIN_B)  = 0; // interrupt-on-change off
    WPU_PIN(ENC_PIN_A)  = 1; // weak pull-up on
    WPU_PIN(ENC_PIN_B)  = 1; // weak pull-up on
    TRIS_PIN(ENC_PIN_A) = 1; // digital input
    TRIS_PIN(ENC_PIN_B) = 1; // digital input
}

