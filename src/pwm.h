#ifndef PWM_H
#define	PWM_H

#include <inttypes.h>

void initPWM();
void quitPWM();
void PWM_SetFrequency(uint16_t frequency);
void PWM_SetDutyCycle(uint16_t duty_cycle);

#endif	/* PWM_H */

