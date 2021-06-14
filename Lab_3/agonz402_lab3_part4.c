/*      Author: Alison Gonzales
 *  Partner(s) Name: Alison Gonzales
 *      Lab Section: 023
 *      Assignment: Lab # 3  Exercise # 4
 *      Exercise Description: [optional - include for your own benefit]
 *
 *      I acknowledge all content contained herein, excluding template or example
 *      code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {

    DDRA = 0x00; DDRB = 0xFF; DDRC = 0xFF;
    PORTA = 0xFF; PORTB = 0x00; PORTC = 0x00;

    unsigned char tmpA = 0x00;
    unsigned char tmpB = 0x00;
    unsigned char tmpC = 0x00;

    while (1) {
        //Read Input;
        tmpA = PINA;
        
        //PB3 ... PB0 => PORTB => output of upper PINA: A7...A4
        tmpB =  ( tmpA >> 4 ) & 0x0F; //take A7-A4, shift 4 right, then make 1s 
        PORTB = tmpB;

        //PC7 ... PC4 => PORTC => output of lower PINA: A3 ... A0
        tmpC =  (tmpA & 0x0F) << 4; //take A3-A0, shift 4 left, then make 1s 
        PORTC = tmpC;

    }
    return 1;
}