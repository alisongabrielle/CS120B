/*      Author: Alison Gonzales
 *  Partner(s) Name: Alison Gonzales
 *      Lab Section: 023
 *      Assignment: Lab # 2  Exercise # 2
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

    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    //A3..A0 -> Parking Space Sensor [ 1 = car parked ] 
    //Output => PORTC , num of available spaces

    unsigned char cntavail = 0x00;
    unsigned char tmpA0 = 0x00;
    unsigned char tmpA1 = 0x00;
    unsigned char tmpA2 = 0x00;
    unsigned char tmpA3 = 0x00;

    while (1) {
        //Read inputs!
        tmpA0 = PINA & 0x01; //A0
        tmpA1 = PINA & 0x02; //A1
        tmpA2 = PINA & 0x04; //A2
        tmpA3 = PINA & 0x08; //A3

        cntavail = (cntavail & 0xF8) | 0x00;

        if (tmpA0 == 0x00) {
                cntavail += 1;
        }

        if (tmpA1 == 0x00) {
                cntavail += 1;
        }

        if (tmpA2 == 0x00) {
                cntavail += 1;
        }
        
        if (tmpA3 == 0x00) {
                cntavail += 1;
        }

        PORTC = cntavail;
    }
    return 1;
}