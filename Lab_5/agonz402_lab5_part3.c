/*	Author: Alison Gonzales
 *  Partner(s) Name: Alison Gonzales
 *	Lab Section: 023
 *	Assignment: Lab # 5  Exercise # 3
 *	Exercise Description: [optional - include for your own benefit]
 * 
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://www.youtube.com/watch?v=QhKbSv7lWH4
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum Lights_STATES { Start, Init, WaitForward, Forward, Switch, WaitBackward, Backward} Lights_STATE;
unsigned char tmpB = 0x00;
//unsigned char prevState = 0x00;
short i = 0;
short numPat = 8;
char pattern[8] = {0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F};
                //   0,   1,     2,   3,    4,    5,    6,    7
void TickFct_Lights() {
    
    unsigned tmpA = ~PINA & 0x01;

    switch (Lights_STATE){
        
        case (Start):
            Lights_STATE = Init;
            i = 0;
            break;

        case (Init):
            if (tmpA == 0x01 ) {
                    Lights_STATE = WaitForward;
            }
            else {
                Lights_STATE = Init; 
            }
            break;

        case (WaitForward):
            Lights_STATE = Forward;
            break;

        case(Forward):
             if (i == numPat) { //PRIORITY! since i == 8, overflow! Immediately go backwards
                Lights_STATE = Switch;
            }
            else if ( tmpA == 0x01) {
                Lights_STATE = Forward;
            }
            else {
                Lights_STATE = Init; //We always go back to init when A0 == 0; (Button Release) moving forward
            }
            break;

        case (Switch):
            if (i == 0) {
                Lights_STATE = Init;
            }
            else if (tmpA == 0x01 ) {
                    Lights_STATE = WaitBackward;
            }
            else {
                Lights_STATE = Switch; 
            }
            break;

        case (WaitBackward): 
            Lights_STATE = Backward;
            break;
        
        case (Backward):
            if (tmpA == 0x01) {
                Lights_STATE = Backward;
            }
            else {
                Lights_STATE = Switch;
            }
            break;

        default:
            Lights_STATE = Start; 
    }

    switch (Lights_STATE){

        case (Start):
            break;
        
        case (Init):
            PORTB = 0x00;
            break;
        
        case (WaitForward):
            i++;
            PORTB = pattern[i];
            break;

        case (Forward):
            //prevState = 0x01;
            break;

         case (WaitBackward):
            i--;
            PORTB = pattern[i];
            break;

        case (Backward):
            //prevState = 0x02;
            break;

        default:
            break;
    }

}

int main (void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    PORTB = 0x00;

    Lights_STATE = Start;

    while (1) {
        TickFct_Lights();
    }
    return 1;
}