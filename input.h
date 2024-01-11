#ifndef INPUT_H
#define	INPUT_H

// All buttons are active high, with pull-down resistors to hold them low
// Push = When hit once, triggers a function at a rising edge signal
// Hold = When held down, selects and displays a specific program state
// Switch = Sets program options depending on logic high or low
enum BUTTON_BITS
{
    // PISO 1 (front board CD4021B)
    BTN_ALARM_ON    = 0,  // PI1 (Switch)
    BTN_ENCODER     = 1,  // PI2 (Push)
    BTN_ALARM_SET   = 2,  // PI3 (Hold)
    BTN_TIME_SET    = 3,  // PI4 (Hold)
    BTN_SEC_SET     = 4,  // PI5 (Hold)
    BTN_TEMP_SHOW   = 5,  // PI6 (Hold)
    
    // PISO 2 (rear board CD4021B)
    BTN_VOLUME_SET  = 8,  // PI1 (Hold)
    BTN_TUNE_SET    = 9,  // PI2 (Hold)
    BTN_BRIGHT_SET  = 10, // PI3 (Hold)
    BTN_SNOOZE      = 11, // PI4 (Push)
};

// Buttons that cannot be pushed simultaneously
#define BTN_STATE_MASK (\
    (1<<BTN_ALARM_SET) | \
    (1<<BTN_TIME_SET)  | \
    (1<<BTN_SEC_SET)   | \
    (1<<BTN_TEMP_SHOW) | \
    (1<<BTN_VOLUME_SET)| \
    (1<<BTN_TUNE_SET)  | \
    (1<<BTN_BRIGHT_SET))

#define ENC_COUNTER_CLOCKWISE   0
#define ENC_CLOCKWISE           1
#define ENC_PUSH_BUTTON         2

#endif	/* INPUT_H */

