#ifndef TIMER_H
#define	TIMER_H

// TMR0: TIMER_RESET_VALUE = 256 - (Period * Fosc) / (4 * PS)
// Period = 0.001 sec, PS = prescaler value = 4, Fosc = 4000000
#define TIMER_RESET_VALUE 6

void initOscillator();
void initTimer0();
void initTimer1();
void initTimer2()

#endif	/* TIMER_H */

