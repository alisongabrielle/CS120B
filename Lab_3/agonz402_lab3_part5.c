/*      Author: Alison Gonzales
 *  Partner(s) Name: Alison Gonzales
 *      Lab Section: 023
 *      Assignment: Lab # 3  Exercise # 5
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

    DDRD = 0x00; DDRB = 0xFE; 
    PORTD = 0xFF; PORTB = 0x01; 
    
    unsigned short weight = 0x00;
    unsigned char airbag = 0x00;
    unsigned char tmpD = 0x00;
    unsigned char tmpB0 = 0x00;

    while (1) {
        
        //Inputs: PD7..D0 + PB0
        tmpD = PIND;
        tmpB0 = PINB & 0x01;

        weight = (tmpD << 1) | tmpB0 ; //shift left once for 9 bit, then add B0

        //if weight >= 70, airbag (PB1) = 1; 
        if (weight >= 70) {
            airbag = ( airbag & 0xF9) | 0x02; //REMEMBER TO SET IT TO F9!
        }
        
        //if weight >5 && < 70, airbag(PB1) = 0, PB2 = 1
        else if ( (weight > 5) && (weight < 70) ) {
            airbag = (airbag & 0xF9) | 0x04;
        }

        //if weight < = 5, PB1 && PB2 = 0;
        else if ( weight <= 5) {
            airbag = (airbag & 0xF9) | 0x00;
        }

        else {
            airbag = (airbag & 0xF9) | 0x00;
        }

        PORTB = airbag;

    }
    return 1;
}