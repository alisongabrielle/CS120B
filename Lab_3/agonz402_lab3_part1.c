/*      Author: agonz402
 *  Partner(s) Name: Alison Gonzales
 *      Lab Section: 023
 *      Assignment: Lab # 3  Exercise # 1
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

    DDRA = 0x00; DDRB = 0x00; DDRC = 0xFF;
    PORTA = 0xFF; PORTB = 0xFF; PORTC = 0x00;

    unsigned char tmpA = 0x00;
    unsigned char tmpB = 0x00;
    unsigned char num1s_total = 0x00;
    unsigned char currBit;
    unsigned char i;

    while (1) {

        tmpA = PINA;
        tmpB = PINB;
        num1s_total = 0;

        for (i = 0; i < 8; ++i) {
            currBit = (tmpA >> i) & 0x01;
            num1s_total = currBit + num1s_total;
            currBit = (tmpB >> i) & 0x01;
            num1s_total = currBit + num1s_total;
        }

        PORTC = num1s_total;
        
    }
    return 1;
}