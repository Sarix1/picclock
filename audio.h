#ifndef AUDIO_H
#define	AUDIO_H

#include <inttypes.h>

// if multiplier is 10 and note duration value is 25, note duration is 250 ms
// this gives a range of 0.01s - 2.56s
#define NOTE_DURATION_MULTIPLIER 10

void speakerOn();
void speakerOff();
uint8_t playNote();

#endif	/* AUDIO_H */

