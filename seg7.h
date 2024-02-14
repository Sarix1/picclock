/* 7-Segment Character Bytes
 *
 * Created by: Sari Jokinen, in January 2023
 * sari.katariina.jokinen@protonmail.com
 * github.com/sarix1
 */

#ifndef SEG7_HEADER_H
#define	SEG7_HEADER_H

               //ABCDEFG.
#define SEG7_0 0b11111100 //    _____   
#define SEG7_1 0b01100000 //  _|__A__|_ 
#define SEG7_2 0b11011010 // | |     | |
#define SEG7_3 0b11110010 // |F|     |B|
#define SEG7_4 0b01100110 // |_|_____|_|
               //ABCDEFG.     _|__G__|_ 
#define SEG7_5 0b10110110 // | |     | |
#define SEG7_6 0b10111110 // |E|     |C|
#define SEG7_7 0b11100000 // |_|_____|_|
#define SEG7_8 0b11111110 //   |__D__|  
#define SEG7_9 0b11110110 // 
               //ABCDEFG.
#define SEG7_A 0b11101110
#define SEG7_a 0b11111010
#define SEG7_b 0b00111110
#define SEG7_C 0b10011100
#define SEG7_c 0b00011010
               //ABCDEFG.
#define SEG7_d 0b01111010
#define SEG7_E 0b10011110
#define SEG7_e 0b11011110
#define SEG7_F 0b10001110
#define SEG7_G 0b10111100
#define SEG7_g 0b11110110
               //ABCDEFG.
#define SEG7_H 0b01101110
#define SEG7_h 0b00101110
#define SEG7_I 0b01100000
#define SEG7_J 0b01111000
#define SEG7_k 0b10101110
               //ABCDEFG.
#define SEG7_L 0b00011100
#define SEG7_l 0b00001100
#define SEG7_ll 0b01101100
#define SEG7_N 0b11101100
#define SEG7_n 0b00101010
#define SEG7_O 0b11111100
               //ABCDEFG.
#define SEG7_o 0b00111010
#define SEG7_P 0b11001110
#define SEG7_q 0b11100110
#define SEG7_R 0b11001000
#define SEG7_r 0b00001010
               //ABCDEFG.
#define SEG7_S 0b10110110
#define SEG7_t 0b00011110
#define SEG7_U 0b01111100
#define SEG7_u 0b00111000
#define SEG7_y 0b01100110

#endif	/* SEG7_HEADER_H */
