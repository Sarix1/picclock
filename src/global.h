#ifndef GLOBAL_H
#define	GLOBAL_H

#define _XTAL_FREQ          8000000

#define LOOP_INTERVAL_MS    10   // 10 milliseconds
#define SENSOR_INTERVAL_MS  3000 // 3 seconds
#define INIT_DELAY_MS       2000 // 1 sec delay until main loop starts
#define ERROR_DELAY_MS      2000 // 2 sec pause to display an error

#define MAX_SNOOZE          60
#define MIN_SNOOZE          1
#define DEFAULT_SNOOZE      10

#define MAX_VOLUME          100
#define MIN_VOLUME          0
#define DEFAULT_VOLUME      50

//#define MAX_BRIGHTNESS      100
//#define MIN_BRIGHTNESS      1
//#define DEFAULT_BRIGHTNESS  50

#define ERROR_SENSOR        (1 << 0)

#include "sensor.h"

typedef struct
{
    // Sensor
    struct sensor_data_s sensor_data;
    // Message
    uint32_t message;
    // Timer
    volatile uint32_t timer_us;
    volatile uint32_t timer_ms;
    volatile uint8_t timer_q : 2;
    volatile uint8_t sec_tic_calls : 5;
    volatile uint8_t t0_expire : 1;
    // State
    uint8_t current_state;
    // Alarm
    uint8_t snooze_minutes;
    uint8_t alarm_song_id;
    uint8_t alarm_timer_on : 1;
    uint8_t alarm_snoozing : 1;
    uint8_t alarming : 1;
    // Sensor
    uint8_t sensor_mode : 2; // (C)elsius, (F)ahrenheit, (H)umidity, (L)ux
    // Audio
    uint8_t audio_test : 1;
    uint8_t audio_random : 1;
    uint8_t audio_playing : 1;
    uint8_t audio_volume;
    // Brightness
    //uint8_t user_brightness;
    //uint8_t auto_brightness : 1;
    // Timer settings
    uint8_t time_set_minutes : 1;
    uint8_t clock_ticking : 1;
    // Misc
    uint8_t error : 6;
} Global_t;

extern Global_t Global;

#endif	/* GLOBAL_H */

