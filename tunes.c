#include <inttypes.h>
#include "notes.h"

const uint8_t MarioUW_notes[] = {

    NOTE_C4,  NOTE_C5,  NOTE_A3,  NOTE_A4,  NOTE_AS3, NOTE_AS4, 0,        0,
    NOTE_C4,  NOTE_C5,  NOTE_A3,  NOTE_A4,  NOTE_AS3, NOTE_AS4, 0,        0,
    NOTE_F3,  NOTE_F4,  NOTE_D3,  NOTE_D4,  NOTE_DS3, NOTE_DS4, 0,        0,
    NOTE_F3,  NOTE_F4,  NOTE_D3,  NOTE_D4,  NOTE_DS3, NOTE_DS4, 0,        0,
    NOTE_DS4, NOTE_CS4, NOTE_D4,  NOTE_CS4, NOTE_DS4, NOTE_DS4, NOTE_GS3, NOTE_G3,
    NOTE_CS4, NOTE_C4,  NOTE_FS4, NOTE_F4,  NOTE_E3,  NOTE_AS4, NOTE_A4,  NOTE_GS4,
    NOTE_DS4, NOTE_B3,  NOTE_AS3, NOTE_A3,  NOTE_GS3, 0,        0,        0 
};

const uint8_t MarioUW_durations[] =
{

    0,   147, 147, 147, 147, 147, 147, 196,
    123, 147, 147, 147, 147, 147, 147, 196,
    123, 147, 147, 147, 147, 147, 147, 196,
    123, 147, 147, 147, 147, 147, 147, 196,
    123, 74,  74,  74,  74,  74,  74,  74,
    74,  74,  221, 221, 221, 221, 221, 221,
    123, 123, 123, 123, 123, 123, 37,  37,
    37
};

const uint16_t MarioUW_note_count = sizeof(MarioUW_notes);