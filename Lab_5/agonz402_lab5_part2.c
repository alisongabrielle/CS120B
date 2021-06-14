/*	Author: Alison Gonzales
 *  Partner(s) Name: Alison Gonzales
 *	Lab Section: 023
 *	Assignment: Lab # 5  Exercise # 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

enum IncDec_STATES { IncDec_SMStart, IncDec_Begin, IncDec_Init, IncDec_Inc, IncDec_Dec, IncDec_WaitInc, IncDec_WaitDec, IncDec_Reset } IncDec_STATE;
unsigned char tmpC = 0x00;
//unsigned char prevState;

void TickFct_IncDec() {
    
    unsigned tmpA = ~PINA & 0x03;

    switch (IncDec_STATE) {

        case (IncDec_SMStart):
            IncDec_STATE = IncDec_Begin;
            break;
         
       case (IncDec_Begin):
            if (tmpA == 0x01) {
                IncDec_STATE = IncDec_Inc;
            }
            else if (tmpA == 0x02) {
                IncDec_STATE = IncDec_Dec;
            }
            else {
                IncDec_STATE = IncDec_Init;
            }
            break;

        case (IncDec_Init):
            if (tmpA == 0x00) {
                IncDec_STATE = IncDec_Init;
            }
            else if (tmpA == 0x01) {
                IncDec_STATE = IncDec_Inc;
            }
            else if (tmpA == 0x02) {
                IncDec_STATE = IncDec_Dec;
            }
            break;

        case (IncDec_Inc):
            if (tmpA == 0x03){
                IncDec_STATE = IncDec_Reset;
            }
            else if (tmpA == 0x02) {
                IncDec_STATE = IncDec_Dec;
            }
            else if (tmpA == 0x01) {
               IncDec_STATE = IncDec_WaitInc;  
            }
            else {
                IncDec_STATE = IncDec_Init;
            }
            break;

        case (IncDec_WaitInc):
            if (tmpA == 0x02) {
                IncDec_STATE = IncDec_Dec;
            }
            else if (tmpA == 0x03) {
                IncDec_STATE = IncDec_Reset;
            }
            else if (tmpA == 0x01) {
                IncDec_STATE = IncDec_WaitInc;
            }      
            else {
               IncDec_STATE = IncDec_Init;
            }
            break;
        
        case (IncDec_Dec):
            if (tmpA == 0x03){
                IncDec_STATE = IncDec_Reset;
            }
            else if (tmpA == 0x01) {
                IncDec_STATE = IncDec_Inc;
            }
            else if (tmpA == 0x02) {
               IncDec_STATE = IncDec_WaitDec;  
            }
            else {
                IncDec_STATE = IncDec_Init;
            }
            break;

        case (IncDec_WaitDec):
            if (tmpA == 0x01) {
                IncDec_STATE = IncDec_Inc;
            }
            else if (tmpA == 0x03) {
                IncDec_STATE = IncDec_Reset;
            }
            else if (tmpA == 0x02) {
               IncDec_STATE = IncDec_WaitDec;  
            }
            else {
                IncDec_STATE = IncDec_Init;
            }          
            break;

        case (IncDec_Reset):
            if (tmpA == 0x01) {
                IncDec_STATE = IncDec_Inc;
            }
            else if (tmpA == 0x02) {
                IncDec_STATE = IncDec_Dec;
            }
            else {
                IncDec_STATE = IncDec_Reset;
            }
            break;

        default:
            IncDec_STATE = IncDec_SMStart;
            break;
        
    }

    switch (IncDec_STATE){

        case (IncDec_SMStart):
            tmpC = 0x07;
            break;
        case (IncDec_Begin) :
            tmpC = 0x07;
            break;        
        case (IncDec_Init):
            break;

        case (IncDec_Inc):
            if (tmpC < 9) {
                tmpC = tmpC + 1;
            }
//            prevState = 0x01;
            break;

        case (IncDec_Dec):
            if (tmpC > 0) {
                tmpC = tmpC - 1;
            }
//            prevState = 0x02;
            break;
        
        case (IncDec_WaitInc):
//            prevState = 0x03;
            break;
        
        case (IncDec_WaitDec):
//            prevState = 0x04;
            break;

        case (IncDec_Reset):
            tmpC = 0x00;
//            prevState = 0x05;
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
    }
    return 1;
}