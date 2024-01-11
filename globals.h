#ifndef GLOBALS_H
#define	GLOBALS_H

#include <inttypes.h>

#define _XTAL_FREQ          4000000
#define LOOP_INTERVAL_MS    10   // 10 milliseconds
#define SENSOR_INTERVAL_MS  7000 // 7 seconds
#define INIT_DELAY_MS       1000 // 1 sec delay until main loop starts
#define ERROR_DELAY_MS      2000 // 2 sec pause to display an error

enum ERRORS
{
    NO_ERROR,
    ERROR_OSCILLATOR        = 1,
    ERROR_INPUT_INVALID     = 2,
    ERROR_TUNE_ID_INVALID   = 3,
    ERROR_TUNE_LENGTH_ZERO  = 4,
    ERROR_TUNE_NULL_POINTER = 5,

    NUM_ERRORS
};

struct input_s // 64
{
    // circular buffer, queues up to 15 rotary encoder turns
    volatile uint16_t enc_buffer;
    volatile uint8_t enc_head : 4;
    volatile uint8_t enc_tail : 4;
    // push-button status bits
    uint16_t button_bits;
    uint16_t button_last;
};

struct status_s // 64
{
    volatile uint32_t timer_ms;           // millisecond timer
    volatile uint8_t timer_ints;          // timer interrupts counter 0-250
    volatile uint8_t sec_tic_calls;       // advance time clock by n seconds
    volatile uint8_t timer_quarter  : 3;  // counts quarter seconds for blinking
    volatile uint8_t clock_ticking  : 1;  // advance clock every second
    
    uint8_t current_state           : 4;  // program state
    volatile uint8_t error          : 4;  // error 1-15
    volatile uint8_t update_call    : 1;  // update input and display
    
    uint8_t alarm_on                : 1;  // clock will alarm
    uint8_t alarming                : 1;  // alarming currently
    uint8_t snoozing                : 1;  // snoozing currently
};

struct audio_s // 40
{
    uint16_t note_index;   // note index
    uint8_t song_id   : 6; // which tune to play, 0-63
    uint8_t random_id : 1; // tune id is randomized
    uint8_t playing   : 1; // playing a tune
    uint8_t volume;  // 0-200
    uint8_t speed;   // 0-200
};

struct sensor_s // 32
{
    uint8_t brightness;  // sensor-detected light level
    uint8_t temperature; // temperature in celsius
    uint8_t humidity    : 7; // 0-100
    uint8_t fahrenheit  : 1; // convert temp unit to fahrenheit
    uint8_t bright_user : 7; // 0-100 user adjustment
    uint8_t autobright  : 1; // auto-brightness on
};

struct blink_s // 16
{
    // 00 no blink, 01 fast blink, 10 slow blink
    uint8_t digits_left  : 2;
    uint8_t digits_right : 2;
    uint8_t dot_left     : 2;
    uint8_t dot_right    : 2;
    uint8_t dot_upper    : 2;
    uint8_t dot_lower    : 2;
    uint8_t unused       : 4;
};

typedef struct // 216
{
    struct status_s status; // 64 //affects actual program state
    struct input_s  input;  // 64 // stores user input
    struct audio_s  audio;  // 40 // controls audio playback
    struct sensor_s sensor; // 32 // sensor readings, user settings
    struct blink_s  blink;  // 16 // controls display blinking
} Globals_t;

extern Globals_t Globals;

#endif	/* GLOBALS_H */

