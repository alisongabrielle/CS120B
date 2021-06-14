/*	Author: Alison Gonzales
 *  Partner(s) Name: Alison Gonzales
 *	Lab Section: 023
 *	Assignment: Lab # 6  Exercise # 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://www.youtube.com/watch?v=6QWEaT3lz5s
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn() {
   TCCR1B = 0x0B;
   OCR1A = 125;
   TIMSK1 = 0x02;
   TCNT1 = 0;
   _avr_timer_cntcurr = _avr_timer_M;
   SREG |= 0x80;
}


void TimerOff() {
   TCCR1B = 0x00;
}

void TimerISR() {
   TimerFlag = 1;
}

ISR (TIMER1_COMPA_vect) {
      _avr_timer_cntcurr--;
      if (_avr_timer_cntcurr == 0) {
         TimerISR();
         _avr_timer_cntcurr = _avr_timer_M;
      }
}

void TimerSet(unsigned long M) {
   _avr_timer_M = M;
   _avr_timer_cntcurr = _avr_timer_M;
}

enum IncDec_STATES { IncDec_SMStart, IncDec_Begin, IncDec_Init, IncDec_Inc, IncDec_Dec, IncDec_WaitInc, IncDec_WaitDec, IncDec_Reset } IncDec_STATE;
unsigned char tmpC = 0x00;
short int holdPress1 = 0;
short int holdPress2 = 0;

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
            else if (tmpA == 0x01 && holdPress1 == 1) {
               IncDec_STATE = IncDec_Inc;
               holdPress1 = 0;
            }
            else if (tmpA == 0x01) {
                holdPress1 = 1;
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
            else if (tmpA == 0x02 && holdPress2 == 2) {
               IncDec_STATE = IncDec_Dec;
               holdPress2 = 0;
            }
            else if (tmpA == 0x02) {
               IncDec_STATE = IncDec_WaitDec;
               holdPress2 = 2;
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
            break;

        case (IncDec_Dec):
            if (tmpC > 0) {
                tmpC = tmpC - 1;
            }
            break;
        
        case (IncDec_WaitInc):
            break;
        
        case (IncDec_WaitDec):
            break;

        case (IncDec_Reset):
            tmpC = 0x00;
            break;
        
    }

   PORTB = tmpC;
}


void main () {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;

    TimerSet(100);
    TimerOn();
    PORTB = 0x00;
    
    IncDec_STATE = IncDec_SMStart;

    while (1) {
      TickFct_IncDec();
      while(!TimerFlag);
      TimerFlag = 0;
    }
}