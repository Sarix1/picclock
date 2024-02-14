#include <xc.h>
#include <inttypes.h>
#include "pins.h"
#include "global.h"
#include "event.h"
#include "display.h"
#include "input.h"
#include "state.h"

// Locals
static uint8_t sensor_status; // 1 == response received, 0 == not received

// Functions
void initDHT11()
{
    TRIS_PIN(DHT11_PIN) = 1;
    PORT_PIN(DHT11_PIN) = 1;
}

static void sensorStart()
{        
    PORT_PIN(DHT11_PIN) = 0;
    TRIS_PIN(DHT11_PIN) = 0; // output 0
    __delay_ms(20);
    
    PORT_PIN(DHT11_PIN) = 1; // output 1
    __delay_us(30);
    
    TRIS_PIN(DHT11_PIN) = 1; // input
}

static inline void sensorStatus()
{
    sensor_status = 0;
    __delay_us(40);
    
    if (PORT_PIN(DHT11_PIN) == 0)
    {
        __delay_us(80);
        if (PORT_PIN(DHT11_PIN) == 1)
            sensor_status = 1;
        __delay_us(40);
    }
}

static inline void sensorRead()
{
    uint8_t i, j;
    uint8_t bytes[5] = {0};
    
    TMR0 = 0;
    Global.t0_expire = 0;
    
    for (j = 0; j < 5; j++)
    {
        for (i = 0; i < 8; i++)
        {
            while (PORT_PIN(DHT11_PIN) == 0 && Global.t0_expire == 0);
            __delay_us(35);
            if (PORT_PIN(DHT11_PIN) == 1)
                bytes[j] |= (1 << (7-i));
            while (PORT_PIN(DHT11_PIN) == 1 && Global.t0_expire == 0);
        }
    }
    
    if (Global.t0_expire == 1 || (uint8_t)(bytes[0]+bytes[1]+bytes[2]+bytes[3]) != bytes[5])
        Global.error |= ERROR_SENSOR;
    else
    {
        Global.sensor_data.humidity_int     = bytes[0];
        Global.sensor_data.humidity_dec     = bytes[1];
        Global.sensor_data.temperature_int  = bytes[2];
        Global.sensor_data.temperature_frac = bytes[3];
    }
}

void updateSensor()
{    
    PIE1bits.T1IE = 0;
    Global.error &= ~ERROR_SENSOR;

    sensorStart();
    sensorStatus();
    
    if (sensor_status == 1)
        sensorRead();
    else
        Global.error |= ERROR_SENSOR;
    
    PIE1bits.T1IE = 1;
}

/*** Sensor ***/
void handleEvent_Sensor(Event_t event)
{
    switch (event.ev_type)
    {
        case EV_ENTER_STATE:
            queueEvent(newEvent(EV_DISPLAY_MODE, DISP_MODE_SENSOR));
            break;
            
        case EV_INPUT:
            switch (event.input_ev.arg_button_id)
            {
                case BTN_ENCODER:
                    Global.sensor_mode ^= 1;
                    break;
            }
            break;
    }    
}

