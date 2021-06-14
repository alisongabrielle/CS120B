/*	Author: Alison Gonzales
 *  Partner(s) Name: Alison Gonzales
 *	Lab Section: 023
 *	Assignment: Lab # 4  Exercise # 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum LED_STATES { LED_SMStart, LED_Init, LED_On1, LED_On2, LED_Release} LED_STATE;
unsigned char prev = 0x00;

void TickFct_LED() {
    
    unsigned char tmpA = PINA & 0x01;

    switch(LED_STATE) {

        case (LED_SMStart):
            LED_STATE = LED_Init;
            PORTB = 0x01;
            break;
        
        case (LED_Init):
            if (tmpA == 0x00) {
                LED_STATE = LED_Init;
            }
            else if (tmpA == 0x01){
                LED_STATE = LED_On1;
            }
            else {
                LED_STATE = LED_Init;
            }
            break;


        case(LED_On1):
            
            if (tmpA == 0x01) {
                LED_STATE = LED_On1;
            }
            else if (tmpA == 0x00) {
                LED_STATE = LED_Release;
            }
            else {
                LED_STATE = LED_Init;
            }
            break;

        case (LED_Release):
            if (tmpA == 0x00) {
                LED_STATE = LED_Release;
            }
            else if (tmpA == 0x01) {
                LED_STATE = LED_On2;
            }
            else {
                LED_STATE = LED_Init;
            }
            break;

         case (LED_On2):
            if ( tmpA == 0x01 || (tmpA == 0x00)) {
                LED_STATE = LED_On2;
            }
            if (tmpA == 0x00 && prev == 0x03) {
                LED_STATE = LED_Init;
            }
            break;

        default:
            LED_STATE = LED_Init;
            break;
        
    }
    
    switch (LED_STATE) {
        case(LED_SMStart):
            break;

        case (LED_Init):
            PORTB = 0x01;
            prev = 0x01;
            break;
        
        case (LED_On1):
            PORTB = 0x02;
            prev = 0x02;
            break;
        
        case (LED_On2):
            PORTB = 0x01;
            prev = 0x03;
            break;
        
        case (LED_Release):
            PORTB = 0x02;
            prev = 0x04;
            break;
    }

}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    DDRC = 0xFF; PORTC = 0x00;

    PORTB = 0x00;

    LED_STATE = LED_SMStart;

    while (1) {
        TickFct_LED();
    }
    return 1;
}

