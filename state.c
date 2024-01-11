#include "state.h"
#include "globals.h"
#include "display.h"
#include "input.h"

void Hello_enter()
{
    Globals.blink.dotleft = 0b01;
    Globals.blink.dot_right = 0b10;
    speakerOn();
}

void Hello_show()
{
    displayMessage(DISPLAY_HELLO);
    Display.bits |= DOT_LEFT|DOT_RIGHT|DOT_LOWER;
}

void Hello_input_enc()
{
    
    changeState(STATE_TIME_CLOCK);
}

void TimeClock_show()
{
    displayClock(TimeClock);
    //Display.dot_right = 0;
    //Display.dot_upper = Display.dot_lower = !!(TMR0 && 0xFF00);
}

State_t States[NUM_STATES]
{
    {   // STATE_HELLO
        Hello_enter(),
        Hello_show(),
        Hello_input_enc(),
        NULL
    },
    {   // STATE_TIME_CLOCK
        TimeClock_enter(),
        TimeClock_show(),
        TimeClock_input_enc(),
        NULL
    },
    {   // STATE_TIME_SET
        TimeSet_enter(),
        TimeSet_show(),
        TimeSet_input_enc()
    }
};

void stateEnter()
{
    States[Globals.status.current_state].enter();
}

void stateShow()
{
    States[Globals.status.current_state].show();
}

void stateHandleEncoderInput(uint8_t value)
{
    States[Globals.status.current_state].enc(value);
}

void changeState(uint8_t state_id)
{
    Globals.status.current_state = state_id;
    States[Globals.status.current_state].enter();
}

 
/*
{
    Seconds_enter(),
    Seconds_show(),
    Seconds_input_enc()
},
{
    AlarmSet_enter(),
    AlarmSet_show(),
    AlarmSet_input_enc()
},
{
    Alarming_enter(),
    Alarming_show(),
    Alarming_input_enc()
},
{
    Temperature_enter(),
    Temperature_show(),
    Temperature_input_enc()
},
{
    TuneSet_enter(),
    TuneSet_show(),
    TuneSet_input_enc()
},
{
    VolumeSet_enter(),
    VolumeSet_show(),
    VolumeSet_input_enc()
},
{
    Brightness_enter(),
    Brightness_show(),
    Brightness_input_enc()
}
 */