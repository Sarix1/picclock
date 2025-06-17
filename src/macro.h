/* Shortcut Macros for PIC16F690
 *
 * Created by: Sari Jokinen, in January 2023
 * sari.katariina.jokinen@protonmail.com
 * github.com/sarix1
 *
 * Example usage:
 *
 * ANSEL_PIN(5) = 0;
 * PORT_PIN(C,2) = 1;
 *
 * char mask = PORT_NTH_BIT(A,3) | PORT_NTH_BIT(A,4);
 * if (PORT_LETTER(A,3) & mask)
 *    PORT_BIT_ON(B,2);
 */

#ifndef MACRO_H
#define	MACRO_H

#define ANSEL0  ANSELbits.ANS0   // pin 19
#define ANSEL1  ANSELbits.ANS1   // pin 18
#define ANSEL2  ANSELbits.ANS2   // pin 17
#define ANSEL3  ANSELbits.ANS3   // pin 3
#define ANSEL4  ANSELbits.ANS4   // pin 16
#define ANSEL5  ANSELbits.ANS5   // pin 15
#define ANSEL6  ANSELbits.ANS6   // pin 14
#define ANSEL7  ANSELbits.ANS7   // pin 7
#define ANSEL8  ANSELHbits.ANS8  // pin 8
#define ANSEL9  ANSELHbits.ANS9  // pin 9
#define ANSEL10 ANSELHbits.ANS10 // pin 13
#define ANSEL11 ANSELHbits.ANS11 // pin 12

#define ANSEL_PIN(number))            ANSEL##number
#define PORT_NUMBER_(letter, number)  number
#define PORT_LETTER_(letter, number)  PORT##letter
#define PORT_NTH_BIT_(letter, number) (1U<<number)
#define PORT_BIT_(letter, number)     PORT##letter & (1U<<number)
#define PORT_BIT_ON_(letter, number)  PORT##letter |= (1U<<number)
#define PORT_BIT_OFF_(letter, number) PORT##letter &= ~(1U<<number)
#define PORT_PIN_(letter, number)     PORT##letter##bits.R##letter##number
#define TRIS_PIN_(letter, number)     TRIS##letter##bits.TRIS##letter##number
#define IOC_PIN_(letter, number)      IOC##letter##bits.IOC##letter##number
#define WPU_PIN_(letter, number)      WPU##letter##bits.WPU##letter##number

#define PORT_NUMBER(x)    PORT_NUMBER_(x)                
#define PORT_LETTER(x)    PORT_LETTER_(x) 
#define PORT_NTH_BIT(x)   PORT_NTH_BIT_(x)
#define PORT_BIT(x)       PORT_BIT_(x)    
#define PORT_BIT_ON(x)    PORT_BIT_ON_(x) 
#define PORT_BIT_OFF(x)   PORT_BIT_OFF_(x)
#define PORT_PIN(x)       PORT_PIN_(x)    
#define TRIS_PIN(x)       TRIS_PIN_(x)    
#define IOC_PIN(x)        IOC_PIN_(x)     
#define WPU_PIN(x)        WPU_PIN_(x)       

#define PWM_A C,5 // pin 5
#define PWM_B C,4 // pin 6
#define PWM_C C,3 // pin 7
#define PWM_D C,2 // pin 14

#endif	/* MACRO_H */
