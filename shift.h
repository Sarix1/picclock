#ifndef SHIFT_H
#define	SHIFT_H

#include <inttypes.h>

void initShiftOut();
void shiftOutLatch();
void shiftOut8(uint8_t shifted_data);
void initShiftIn();
void shiftInLatch();
uint8_t shiftIn8();
uint16_t shiftIn16();
uint8_t shiftIn8_reverse();
uint16_t shiftIn16_reverse();

#endif	/* SHIFT_H */

