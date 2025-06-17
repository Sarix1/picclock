#ifndef PINS_H
#define	PINS_H

#include "macro.h"

#define AUDIO_PIN    PWM_A // pin 5
#define DHT11_PIN    B,4 // pin 13
#define SIPO_OE      A,2 // pin 17

#define SIPO_LAT_PIN C,4 // pin 6
#define SIPO_CLK_PIN C,7 // pin 9
#define SIPO_DAT_PIN C,6 // pin 8

#define PISO_LAT_PIN C,1 // pin 15
#define PISO_CLK_PIN C,0 // pin 16
#define PISO_DAT_PIN B,5 // pin 12

#define ENC_PIN_A    B,6 // pin 11
#define ENC_PIN_B    B,7 // pin 10
#define ENC_PIN_BOTH (PORT_NTH_BIT(ENC_PIN_A) | PORT_NTH_BIT(ENC_PIN_B))

#endif	/* PINS_H */
