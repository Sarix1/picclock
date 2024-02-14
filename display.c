#include "display.h"
#include "clock.h"
#include "event.h"
#include "global.h"
#include "shift_out.h"
#include "input.h"

// Definitions
typedef union
{
    uint32_t digits_bits;
    uint8_t digits[4];
} Display_t;

const uint8_t seg7[10] =
{       
    SEG7_0, SEG7_1, SEG7_2, SEG7_3, SEG7_4,
    SEG7_5, SEG7_6, SEG7_7, SEG7_8, SEG7_9 
};

// Locals
static Display_t Display = {0};
static uint8_t display_mode = DISP_MODE_TIME_CLOCK;

// Forward declarations
static inline void setDisplayMode(uint8_t mode);
static void refreshDisplay();

static void displaySeconds();
static void displayClock(uint8_t clock_id);
static void displayClockSet(uint8_t clock_id);
static void displayValue(uint16_t value, uint8_t alignment);

static inline void displayTime();
static inline void displayTimeSet();
static inline void displayAlarmSet();
static inline void displayMessage(uint32_t message_bits);
static inline void displaySensor();

// Functions
void updateDisplay()
{
    setDisplayMode(display_mode);
    refreshDisplay();
}

static inline void setDisplayMode(uint8_t mode)
{
    Display.digits_bits = 0;
    
    switch (mode)
    {
        case DISP_MODE_MESSAGE:    displayMessage(Global.message);          break;
        case DISP_MODE_TIME_CLOCK: displayTime();                           break;
        case DISP_MODE_TIME_SET:   displayTimeSet();                        break;
        case DISP_MODE_ALARM_SET:  displayAlarmSet();                       break;
        case DISP_MODE_SEC_SET:    displaySeconds();                        break;
        case DISP_MODE_SNOOZE_SET: displayValue(Global.snooze_minutes, 3);  break;
        case DISP_MODE_VOLUME_SET: displayValue(Global.audio_volume, 3);    break;
        case DISP_MODE_SENSOR:     displaySensor();                         break;
        //case DISP_MODE_BRIGHT_SET: displayValue(Global.user_brightness, 3); break;
    }
}

void handleDisplayEvent(Event_t event)
{
    switch (event.ev_type)
    {
        case EV_DISPLAY_MODE:
            display_mode = event.arg_state;
            break;
    }
}

static void displaySeconds()
{
    Clock_t* clock = getClock(TIME_CLOCK);
    
    Display.digits[0] = seg7[clock->min10];
    Display.digits[1] = seg7[clock->min1];
    Display.digits[2] = seg7[clock->sec10];
    Display.digits[3] = seg7[clock->sec1];
    
    if ((Global.timer_q & 2) && Global.clock_ticking == 1)
        Display.digits_bits |= DISP_DOT_LOWER;
    
    if (Global.alarm_timer_on == 1)
        Display.digits_bits |= DISP_DOT_LEFT;
}

static void displayClock(uint8_t clock_id)
{
    Clock_t* clock = getClock(clock_id);
    Display.digits[0] = seg7[clock->hour10];
    Display.digits[1] = seg7[clock->hour1];
    Display.digits[2] = seg7[clock->min10];
    Display.digits[3] = seg7[clock->min1];
    
    if (Global.timer_q & 2)
        Display.digits_bits |= DISP_DOT_UPPER | DISP_DOT_LOWER;
}

static void displayClockSet(uint8_t clock_id)
{
    displayClock(clock_id);
    
    if (!(Global.timer_q & 1))
    {
        if (Global.time_set_minutes == 0)
            Display.digits_bits &= ~(DISP_DIGIT1 | DISP_DIGIT2);
        else
            Display.digits_bits &= ~(DISP_DIGIT3 | DISP_DIGIT4);
    }
}

static void displayValue(uint16_t value, uint8_t alignment)
{
    uint8_t digits[4];
    uint8_t digit_count = 0;
    uint8_t i;
    
    do
    {
        digits[digit_count] = value % 10;
        value /= 10;
        digit_count++;
        
    } while (value != 0 && digit_count < 4);
    
    if (digit_count-1 > alignment)
        digit_count = alignment+1;
    
    for (i = 0; i < digit_count; i++)
        Display.digits[alignment-i] = seg7[digits[i]];
}

static inline void displayTime()
{
    displayClock(TIME_CLOCK);
    
    if (Global.alarm_timer_on == 1)
        Display.digits_bits |= DISP_DOT_LEFT;
}

static inline void displayTimeSet()
{
    displayClockSet(TIME_CLOCK);
    if (Global.alarm_timer_on == 1)
        Display.digits_bits |= DISP_DOT_LEFT;
}

static inline void displayAlarmSet()
{
    displayClockSet(ALARM_CLOCK);
    
    if (Global.timer_q & 1)
        Display.digits_bits |= DISP_DOT_LEFT;
}

static inline void displayMessage(uint32_t message_bits)
{
    Display.digits_bits = message_bits;
}

static inline void displaySensor()
{
    /*
    if (Global.error & ERROR_SENSOR)
        displayMessage(DISP_ERROR(SEG7_0));
    else
    {
     */
        switch (Global.sensor_mode)
        {
            case 0:
                displayValue(Global.sensor_data.temperature_frac, 2);
                displayValue(Global.sensor_data.temperature_int, 1);
                Display.digits[3] = SEG7_C;
                Display.digits_bits |= DISP_DOT_RIGHT | DISP_DOT_LOWER;
                break;

            case 1:
                displayValue(Global.sensor_data.humidity_int, 2);
                Display.digits[3] = SEG7_H;
                break;
        }
    //}
}

static void refreshDisplay()
{   
    shiftOut8(Display.digits[3]);
    shiftOut8(Display.digits[2]);
    shiftOut8(Display.digits[1]);
    shiftOut8(Display.digits[0]);
    shiftOutLatch();
}

/*** Brightness Set ***/
/*
void handleEvent_BrightSet(Event_t event)
{
    switch (event.ev_type)
    {
        case EV_ENTER_STATE:
            queueEvent(newEvent(EV_DISPLAY_MODE, DISP_MODE_BRIGHT_SET));
            break;
            
        case EV_INPUT:
            switch (event.input_ev.arg_button_id)
            {
                case BTN_ENCODER:
                    Global.auto_brightness ^= 1;
                    break;
                case BTN_ENCODER_CW:
                    if (Global.user_brightness < MAX_BRIGHTNESS)
                        Global.user_brightness++;
                    break;
                case BTN_ENCODER_CCW:
                    if (Global.user_brightness > MIN_BRIGHTNESS)
                        Global.user_brightness--;
                    break;
            }
            break;
    }    
}
*/