#ifndef TUNES_H
#define	TUNES_H

typedef struct
{
    char name[20];
    uint16_t size;
    uint8_t* notes;
    uint8_t* durations;
} Tune_t;

#endif	/* TUNES_H */

