#ifndef STATE_H
#define	STATE_H

#include <inttypes.h>

enum STATE_ID
{
    STATE_HELLO,
    STATE_TIME_CLOCK,
    STATE_TIME_SET,
    STATE_SEC_SET,
    STATE_ALARM_SET,
    STATE_ALARMING,
    STATE_TEMP_SHOW,
    STATE_TUNE_SET,
    STATE_VOLUME_SET,
    STATE_BRIGHT_SET,
    NUM_STATES
};

typedef void (*fnp)(void);
typedef void (*fnp_i)(uint8_t);
typedef uint8_t (*i_fnp)(void);
typedef uint8_t (*i_fnp_i)(void);

typedef struct
{
    fnp   enter;     // Called when entering the state
    fnp   show;      // Updates the display digit data
    fnp_i input_enc; // Rotary encoder turn left/right
} State_t;

void changeState(uint8_t state_id);
void stateHandleEncoderInput(uint8_t value);

#endif	/* STATE_H */

