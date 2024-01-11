#ifndef PINS_H
#define	PINS_H

#include "macro.h"

#define AUDIO_PIN  PWM_A // pin 5
#define VOLUME_PIN PWM_C // pin 7
#define BRIGHT_PIN PWM_D // pin 14

#define SIPO_LAT_PIN C,4 // pin 6
#define SIPO_CLK_PIN C,7 // pin 9
#define SIPO_DAT_PIN C,6 // pin 8

#define PISO_LAT_PIN C,1 // pin 15
#define PISO_CLK_PIN C,0 // pin 16
#define PISO_DAT_PIN B,5 // pin 12

#define ENC_PIN_A    B,6 // pin 11
#define ENC_PIN_B    B,7 // pin 10
#define ENC_MASK    (PORT_NTH_BIT(ENC_PIN_A) | PORT_NTH_BIT(ENC_PIN_B))

enum PIN_IDS
{
    ID_AUDIO_PIN,
    ID_VOLUME_PIN,
    ID_BRIGHT_PIN
};

#endif	/* PINS_H */
