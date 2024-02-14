#ifndef DISPLAY_H
#define DISPLAY_H

#include <inttypes.h>
#include "seg7.h"

#define DISP_DIGIT1    (uint32_t)0x000000FE
#define DISP_DIGIT2    (uint32_t)0x0000FE00
#define DISP_DIGIT3    (uint32_t)0x00FE0000
#define DISP_DIGIT4    (uint32_t)0xFE000000

#define DISP_DOT_LEFT  (uint32_t)0x00000001
#define DISP_DOT_LOWER (uint32_t)0x00000100
#define DISP_DOT_UPPER (uint32_t)0x00010000
#define DISP_DOT_RIGHT (uint32_t)0x01000000

#define DISP_TEXT(a,b,c,d)  (((uint32_t)(a)) | ((uint32_t)(b)<<8) | ((uint32_t)(c)<<16) | ((uint32_t)(d)<<24))

#define DISP_HELLO      DISP_TEXT(SEG7_H, SEG7_e, SEG7_ll, SEG7_o)
#define DISP_POLLO      DISP_TEXT(SEG7_P, SEG7_o, SEG7_ll, SEG7_o)
#define DISP_ERROR(x)   DISP_TEXT(SEG7_E, SEG7_r, SEG7_r, x)

enum DISPLAY_MODE
{
    DISP_MODE_MESSAGE,
    DISP_MODE_TIME_CLOCK,
    DISP_MODE_TIME_SET,
    DISP_MODE_ALARM_SET,
    DISP_MODE_SEC_SET,
    DISP_MODE_SNOOZE_SET,
    DISP_MODE_SENSOR,
    //DISP_MODE_TUNE_ID,
    DISP_MODE_VOLUME_SET,
    //DISP_MODE_BRIGHT_SET,
    NUM_DISP_MODES
};

#endif/* DISPLAY_H */
