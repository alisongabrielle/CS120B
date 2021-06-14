/*	Author: Alison Gonzales
 *  Partner(s) Name: Alison Gonzales
 *	Lab Section: 023
 *	Assignment: Lab # 4  Exercise # 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum Gate_STATES {Gate_SMStart, Gate_Init, Gate_Wait, Gate_Unlock, Gate_Lock} Gate_STATE;
unsigned char prev = 0x00;

void TickFct_Gate() {

    unsigned char tmpA2 = PINA & 0x04;
    unsigned char tmpA1 = PINA & 0x02;


    switch (Gate_STATE) {

        case (Gate_SMStart):
            Gate_STATE = Gate_Init;
            break;

        case (Gate_Init):
            if (PINA == 0x00) {
                Gate_STATE = Gate_Init;
            }
            else if ( PINA == 0x80 || PINA == 0x02 || PINA == 0x01 || PINA == 0x83 ) {
                Gate_STATE = Gate_Lock;
            }
            else if (tmpA2 == 0x04) {
                Gate_STATE = Gate_Wait;
            }
            else {
                Gate_STATE = Gate_Init;
            }
            break;

        case (Gate_Wait):
            if (tmpA2 == 0x04 && prev == 0x02) {
                Gate_STATE = Gate_Wait;
            }
            else if (tmpA1 == 0x01 && prev == 0x02) {
                Gate_STATE = Gate_Wait;
            }
            else if (tmpA2 == 0x00 && tmpA1 == 0x00 && prev == 0x02) {
                
                Gate_STATE = Gate_Unlock;
            }
            else if (PINA == 0x00) {
                Gate_STATE = Gate_Unlock;
            }
            else if (PINA == 0x80 || PINA == 0x02 || PINA == 0x01 || PINA == 0x83) {
                
                Gate_STATE = Gate_Lock;
            }
            break;

        case (Gate_Unlock):
            if (PINA == 0x80 || PINA == 0x04 || 0x01) {
                Gate_STATE = Gate_Lock;
            }
            else if (prev == 0x03 && (tmpA2 == 0x00 || tmpA1 == 0x00) ) {
                Gate_STATE = Gate_Unlock;
            }
            
            break;

        case (Gate_Lock):
            Gate_STATE = Gate_Init;    
            break;

        default:
            Gate_STATE = Gate_SMStart;
            break;

    }

    switch (Gate_STATE) {
        
        case (Gate_SMStart):
            break;

        case (Gate_Init):
            PORTB = 0x00;
            prev = 0x01;
            break;

        case (Gate_Wait):
            prev = 0x02;
            break;

        case (Gate_Unlock):
            PORTB = 0x01;
            prev = 0x03;
            break;

        case (Gate_Lock):
            PORTB = 0x00;
            prev = 0x04;
            break;

    }

}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRC = 0xFF; PORTB = 0x00;

    PORTB = 0x00;

    Gate_STATE = Gate_SMStart;

    while (1) {
        TickFct_Gate();
    }
    return 1;
}