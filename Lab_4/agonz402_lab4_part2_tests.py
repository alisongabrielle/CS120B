/*	Author: Alison Gonzales
 *  Partner(s) Name: Alison Gonzales
 *	Lab Section: 023
 *	Assignment: Lab # 4  Exercise # 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum IncDec_STATES { IncDec_SMStart, IncDec_Choose, IncDec_Inc, IncDec_Dec, IncDec_Keep, IncDec_Reset} IncDec_STATE;
unsigned char tmpC = 0x00;
unsigned char prev = 0x00;



void TickFct_IncDec() {
    unsigned tmpA = PINA;

    switch (IncDec_STATE) {

        case (IncDec_SMStart):
            IncDec_STATE = IncDec_Choose;
            break;

        case (IncDec_Choose):
            if (tmpA == 0x00) {
                IncDec_STATE = IncDec_Choose;
            }
            else if (tmpA == 0x01 && prev == 0x01) {
                IncDec_STATE = IncDec_Inc;
            }
            else if (tmpA == 0x02 && prev == 0x02) {

                IncDec_STATE = IncDec_Dec;
            }
            else if (tmpA == 0x03) {
                IncDec_STATE = IncDec_Reset;
            }
            else {
                IncDec_STATE = IncDec_Choose;
            }

            break;

        case (IncDec_Inc):
            if (tmpA == 0x03) {
                IncDec_STATE = IncDec_Reset;
            }
            else {
                IncDec_STATE = IncDec_Keep;
            }
            break;

        case (IncDec_Keep):
            if (tmpA == 0x03){

                IncDec_STATE = IncDec_Reset;
            }
            else if (tmpA == 0x00) {
                IncDec_STATE = IncDec_Choose;
            }
            else {
                IncDec_STATE = IncDec_Keep;
            }
            break;

        case (IncDec_Dec) :
            if (tmpA == 0x02) {
                IncDec_STATE = IncDec_Dec;
            }
            else {
                IncDec_STATE = IncDec_Keep;
            }
            break;
        
        default:
            IncDec_STATE = IncDec_SMStart;
            break;
    }

    switch (IncDec_STATE) {
        
        case (IncDec_SMStart):
            break;

        case (IncDec_Choose):
            tmpC = 0x07;
            prev = 0x01;
            break;
        
        case (IncDec_Inc):
            if (tmpC < 9) {
                tmpC = tmpC + 1;
            }
            prev = 0x02;
            break;
        
        case (IncDec_Keep):
            break;

        case (IncDec_Dec):
            if (tmpC > 0) {
                tmpC = tmpC  - 1;
            }
            prev = 0x03;
            break;

        case (IncDec_Reset):
            tmpC = 0x00;
            prev = 0x04;
            break;
    }
    PORTC = tmpC;
}

int main (void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    DDRB = 0xFF; PORTB = 0x00;

    PORTC = 0x00;
    

    IncDec_STATE = IncDec_SMStart;

    while (1) {
        TickFct_IncDec();
        PORTB = tmpC;
    }
    return 1;
}