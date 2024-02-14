#ifndef AUDIO_H
#define	AUDIO_H

#include <inttypes.h>

typedef struct
{
    uint16_t note_count;
    uint8_t* notes;
    uint8_t* durations;
} Tune_t;

#endif	/* AUDIO_H */

