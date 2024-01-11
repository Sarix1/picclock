#include <xc.h>
#include <inttypes.h>
#include "pins.h"

void initShiftOut()
{
    TRIS_PIN(SIPO_LAT_PIN) = 0;
    TRIS_PIN(SIPO_DAT_PIN) = 0;
    TRIS_PIN(SIPO_CLK_PIN) = 0;
    
    PORT_PIN(SIPO_LAT_PIN) = 0;
    PORT_PIN(SIPO_DAT_PIN) = 0;
    PORT_PIN(SIPO_CLK_PIN) = 0;
}

void shiftOutLatch()
{
    // move written input bits to SIPO output memory
    PORT_PIN(SIPO_LAT_PIN) = 1;
    PORT_PIN(SIPO_LAT_PIN) = 0;
}

void shiftOut8(uint8_t shifted_data)
{
    // initialize
    uint8_t mask = 0b00000001;
    
    // write 8 bits
    do
    {
        // write bit
        PORT_PIN(SIPO_DAT_PIN) = !!(shifted_data & mask);
        // shift one bit forward
        PORT_PIN(SIPO_CLK_PIN) = 1;
        PORT_PIN(SIPO_CLK_PIN) = 0;
    }
    while ((mask<<=1) > 0);
}

void initShiftIn()
{
    TRIS_PIN(PISO_LAT_PIN) = 0;
    TRIS_PIN(PISO_DAT_PIN) = 1;
    TRIS_PIN(PISO_CLK_PIN) = 0;
    
    PORT_PIN(SIPO_LAT_PIN) = 0;
    PORT_PIN(SIPO_CLK_PIN) = 0;
}

void shiftInLatch()
{
    // move detected input bits to PISO output memory and latch the data
    PORT_PIN(PISO_LAT_PIN) = 1;
    PORT_PIN(PISO_CLK_PIN) = 1;
    PORT_PIN(PISO_CLK_PIN) = 0;
    PORT_PIN(PISO_LAT_PIN) = 0;
}

uint8_t shiftIn8()
{
    // initialize
    uint8_t shifted_data = 0;
    uint8_t mask = 1;
        
    // read 8 bits
    do
    {
        // read bit
        if (PORT_PIN(PISO_DAT_PIN))
            shifted_data |= mask;

        // shift the register
        PORT_PIN(PISO_CLK_PIN) = 1;
        PORT_PIN(PISO_CLK_PIN) = 0;
    } while (mask <<= 1);

    return shifted_data;
}

uint16_t shiftIn16()
{
    // initialize
    uint16_t shifted_data = 0;
    uint16_t mask = 1;
        
    // read 8 bits
    do
    {
        // read bit
        if (PORT_PIN(PISO_DAT_PIN))
            shifted_data |= mask;

        // shift the register
        PORT_PIN(PISO_CLK_PIN) = 1;
        PORT_PIN(PISO_CLK_PIN) = 0;
    } while (mask <<= 1);

    return shifted_data;
}

uint8_t shiftIn8_reverse()
{
    // initialize
    uint8_t shifted_data = 0;
    uint8_t mask = 0b10000000;
    

    // read 8 bits
    do
    {
        // read bit
        if (PORT_PIN(PISO_DAT_PIN))
            shifted_data |= mask;

        // shift the register
        PORT_PIN(PISO_CLK_PIN) = 1;
        PORT_PIN(PISO_CLK_PIN) = 0;
    } while (mask >>= 1);

    return shifted_data;
}

uint16_t shiftIn16_reverse()
{
    // initialize
    uint16_t shifted_data = 0;
    uint16_t mask = 1 << 15;
        
    // read 8 bits
    do
    {
        // read bit
        if (PORT_PIN(PISO_DAT_PIN))
            shifted_data |= mask;

        // shift the register
        PORT_PIN(PISO_CLK_PIN) = 1;
        PORT_PIN(PISO_CLK_PIN) = 0;
    } while (mask >>= 1);

    return shifted_data;
}

