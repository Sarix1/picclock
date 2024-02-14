#include <xc.h>
#include "pins.h"
#include "global.h"
#include "input.h"
#include "shift_in.h"
#include "event.h"
#include "state.h"

// Locals
static struct input_s
{
    uint16_t button_bits;
    uint16_t button_bits_last;
    volatile uint16_t enc_buffer;
    volatile uint8_t enc_count;
    volatile uint8_t enc_head;
    volatile uint8_t enc_tail;
} input = {0};

// Forward declarations
static inline uint16_t readButtons();
static inline void handleHoldButtons(uint16_t bits);
void handleStateEvent(Event_t event); // from state.c

// Functions
void updateInput()
{
    uint16_t changed_bits;
    uint8_t enc_dir;
    
    // Poll buttons
    input.button_bits = readButtons();
    changed_bits = input.button_bits ^ input.button_bits_last;
    input.button_bits_last = input.button_bits;
    
    // Button pushes
    if (changed_bits)
    {
        // handle state-changing buttons (only 1 allowed to be held at a time))
        if (changed_bits & BTN_HOLD_MASK)
            handleHoldButtons(input.button_bits & BTN_HOLD_MASK);
        // toggle alarm timer on/off, triggered during push and release
        if (changed_bits & (1<<BTN_ALARM_TIMER_ON))
            queueEvent(newEvent(EV_ALARM_TIMER, !!(input.button_bits & (1<<BTN_ALARM_TIMER_ON))));
        // buttons that are triggered once during the instant they're pushed down
        if ((changed_bits & (1<<BTN_ENCODER)) & input.button_bits)
            queueEvent(newEvent(EV_INPUT, BTN_ENCODER));
    }
    
    // Rotary encoder turns (queued by encoderInterrupt())
    while (input.enc_count > 0)
    {
        input.enc_count--;
        enc_dir = !!(input.enc_buffer & (1 << input.enc_tail));
        
        input.enc_tail++;
        if (input.enc_tail >= ENC_MAX_EVENTS)
            input.enc_tail = 0;
        
        handleStateEvent(newEvent(EV_INPUT, (enc_dir == 1) ? BTN_ENCODER_CW : BTN_ENCODER_CCW));
    }
}

static inline uint16_t readButtons()
{
    uint16_t bits = 0; 
    
    shiftInLatch();
    bits |= shiftIn8_reverse();
    bits |= shiftIn8_reverse() << 8;
    
    return bits;
}

static inline void handleHoldButtons(uint16_t bits)
{
    uint8_t index = 0;
    
    if ((bits != 0) && !(bits & (bits-1))) // is only one bit set?
    {
        while ((bits & 1) == 0)
        {
            bits >>= 1;
            index++;
        }
        queueEvent(newEvent(EV_INPUT, index));
    }
    else
        queueEvent(newEvent(EV_INPUT, BTN_NONE));
}

void handleInputEvent(Event_t event)
{
    switch (event.input_ev.arg_button_id)
    {
        // encoder input is always state-specific
        case BTN_ENCODER:
        case BTN_ENCODER_CW:
        case BTN_ENCODER_CCW:
            handleStateEvent(event);
            break;
            
        case BTN_SNOOZE:
            if (Global.alarming == 1)
                queueEvent(newEvent(EV_ALARM_SNOOZE, 0));
            else
                queueEvent(newEvent(EV_CHANGE_STATE, STATE_SNOOZE_SET));
            break;
            
        case BTN_ALARM_SET:
            if (Global.alarming == 1)
                queueEvent(newEvent(EV_ALARM_STOP, 0));
            else
                queueEvent(newEvent(EV_CHANGE_STATE, STATE_ALARM_SET));
            break;
            
        case BTN_NONE:          queueEvent(newEvent(EV_CHANGE_STATE, STATE_TIME_CLOCK));    break;
        case BTN_TIME_SET:      queueEvent(newEvent(EV_CHANGE_STATE, STATE_TIME_SET));      break;
        case BTN_SEC_SET:       queueEvent(newEvent(EV_CHANGE_STATE, STATE_SEC_SET));       break;
        //case BTN_BRIGHT_SET:    queueEvent(newEvent(EV_CHANGE_STATE, STATE_BRIGHT_SET));    break;
        case BTN_SENSOR:        queueEvent(newEvent(EV_CHANGE_STATE, STATE_SENSOR));        break;
        case BTN_VOLUME_SET:    queueEvent(newEvent(EV_CHANGE_STATE, STATE_VOLUME_SET));    break;
        //case BTN_TUNE_SET:      queueEvent(newEvent(EV_CHANGE_STATE, STATE_TUNE_SET));      break;
        
        default:
            break;
    }
}

void initRotaryEncoder()
{
    OPTION_REGbits.nRABPU = 0; // weak pull-up enable
    INTCONbits.RABIE = 1; // PORTA/B interrupt on change enable
    
    IOC_PIN(ENC_PIN_A)  = 1; // interrupt-on-change on
    IOC_PIN(ENC_PIN_B)  = 0; // interrupt-on-change off
    WPU_PIN(ENC_PIN_A)  = 1; // weak pull-up on
    WPU_PIN(ENC_PIN_B)  = 1; // weak pull-up on
    TRIS_PIN(ENC_PIN_A) = 1; // digital input
    TRIS_PIN(ENC_PIN_B) = 1; // digital input
}

void encoderInterrupt()
{
    uint8_t enc_bits;
    INTCONbits.RABIE = 0;
    
    if (input.enc_count < ENC_MAX_EVENTS)
    {
        input.enc_count++;
        enc_bits = PORT_LETTER(ENC_PIN_A) & (PORT_NTH_BIT(ENC_PIN_A) | PORT_NTH_BIT(ENC_PIN_B));
        
        switch (enc_bits)
        {
            // clockwise if only 1 bit is set
            case PORT_NTH_BIT(ENC_PIN_A):
            case PORT_NTH_BIT(ENC_PIN_B):
                input.enc_buffer |= (1 << input.enc_head);
                break;
            // counter-clockwise if 2 or 0 bits are set
            case PORT_NTH_BIT(ENC_PIN_A) | PORT_NTH_BIT(ENC_PIN_B):
            case 0:
                input.enc_buffer &= ~(1 << input.enc_head);
                break;
        }
        
        input.enc_head++;
        if (input.enc_head >= ENC_MAX_EVENTS)
            input.enc_head = 0;
    }
    
    INTCONbits.RABIF = 0;
    INTCONbits.RABIE = 1;
}