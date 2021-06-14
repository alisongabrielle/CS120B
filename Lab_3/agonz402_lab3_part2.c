/*      Author: Alison Gonzales
 *  Partner(s) Name: Alison Gonzales
 *      Lab Section: 023
 *      Assignment: Lab # 3  Exercise # 2
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

    DDRA = 0x00; DDRC = 0xFF; 
    PORTA = 0xFF; PORTC = 0x00;

    unsigned char tmpC = 0x00;
    unsigned char fuel_level= 0x00;

    while (1) {
        //Read Input;
        fuel_level= PINA & 0x0F; //Takes input A3...A0
        
        if ( fuel_level == 0x00) {
            tmpC = (tmpC & 0xBF) | 0x40;
        }

        if ( (fuel_level== 0x01) | (fuel_level== 0x02) ) {
            tmpC = (tmpC & 0x80) | 0x60; //PC5 = 1, PC6 = 1, 0x60 
        }

        if ( (fuel_level == 0x03) | (fuel_level == 0x04) ) {
            tmpC = (tmpC & 0x80) | 0x70; //PC5, PC4 = 1, PC6, = 1
        }

        if ( (fuel_level == 0x05) | (fuel_level == 0x06 ) ) {
            tmpC = 0x38; //PC5, PC4, PC3 = 1
        }

        if ( (fuel_level == 0x07 ) | (fuel_level == 0x08 ) | (fuel_level == 0x09) ) {
            tmpC =  0x3C; //PC5,4,3,2 = 1
        }

        if ( (fuel_level == 0x0A) | (fuel_level == 0x0B) | (fuel_level == 0x0C) ) {
            tmpC =  0x3E; //PC5...1
        }

        if ( (fuel_level == 0x0D) | (fuel_level == 0x0E) | (fuel_level == 0x0F) ) {
            tmpC =  0x3F; //PC5...0
        } 

        PORTC = tmpC;
        
    }
    return 1;
}