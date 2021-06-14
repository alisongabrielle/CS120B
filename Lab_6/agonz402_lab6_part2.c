/*	Name & E-mail: Alison Gonzales agonz402@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab # 6 Exercise # 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Link: https://www.youtube.com/watch?v=5Z_97w47AE4
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

short int patLED = 0;
short int prevState = 0;
short int pauseLED = 0;
short int resumeLED = 0;

enum LEDStopGame_States { LEDStopGame_SMStart, LEDStopGame_Led1, LEDStopGame_Led2, LEDStopGame_Led3, 
LEDStopGame_StopLed1, LEDStopGame_StopLed2, 
LEDStopGame_StopLed3, LEDStopGame_Reset } LEDStopGame_State;

void TickFct_LEDStopGame() {
   
   unsigned char tmpA = ~PINA & 0x01;

   switch(LEDStopGame_State) { 
      case LEDStopGame_SMStart:
         LEDStopGame_State = LEDStopGame_Led1;
         break;
         
      case LEDStopGame_Led1: 
         prevState = 1;
         
         if (tmpA == 0x00) {
            patLED = 1;
            LEDStopGame_State = LEDStopGame_Led2;
         }
         else if (tmpA == 0x01 ) {
            LEDStopGame_State = LEDStopGame_StopLed1;
         }
         break;
         
      case LEDStopGame_Led2: 
         
         if (tmpA == 0x00 && patLED == 1) {
            LEDStopGame_State = LEDStopGame_Led3;
         }
         else if (tmpA == 0x00 && patLED == 3) {
            LEDStopGame_State = LEDStopGame_Led1;
         }
         else if (tmpA == 0x01) {
            prevState = 2;
            LEDStopGame_State = LEDStopGame_StopLed2;
         }
         break;
         
       case LEDStopGame_Led3: 
         prevState = 3;
         
         if (tmpA == 0x01) {
            LEDStopGame_State = LEDStopGame_StopLed3;
         }
         else {
            patLED = 3;
            LEDStopGame_State = LEDStopGame_Led2;
         }
         break;
         
      case LEDStopGame_StopLed1: 
         
         pauseLED = 1;
         
         if (tmpA == 0x00 && resumeLED == 1) {
            LEDStopGame_State = LEDStopGame_Led1;
         }
         else if (tmpA == 0x00 && prevState == 1) {
            LEDStopGame_State = LEDStopGame_Reset;
         }
         else if (tmpA == 0x01 && prevState == 1) {
            LEDStopGame_State = LEDStopGame_StopLed1;
         }
         break;
         
         
      case LEDStopGame_StopLed2: 
         pauseLED = 2;
         
         if (tmpA == 0x00 && resumeLED == 2) {
            LEDStopGame_State = LEDStopGame_Led2;
         }
         else if (tmpA == 0x00  && prevState == 2) {
            LEDStopGame_State = LEDStopGame_Reset;
         }
         else if (tmpA == 0x01) {
            LEDStopGame_State = LEDStopGame_StopLed2;
         }
         break;
      
      case LEDStopGame_StopLed3: 
         pauseLED = 3;
         
         if (tmpA == 0x00 && resumeLED == 3) {
            LEDStopGame_State = LEDStopGame_Led3;
         }
         else if (tmpA == 0x00  && prevState == 3) {
            LEDStopGame_State = LEDStopGame_Reset;
         }
         else if (tmpA == 0x01) {
            LEDStopGame_State = LEDStopGame_StopLed3;
         }
         break;
      
      case LEDStopGame_Reset:
         if (tmpA == 0x01 && pauseLED == 1) {
            resumeLED = 1;
            LEDStopGame_State = LEDStopGame_StopLed1;
         }
         else if (tmpA == 0x01 && pauseLED == 2) {
            resumeLED = 2;
            LEDStopGame_State = LEDStopGame_StopLed2;
         }
         else if (tmpA == 0x01 && pauseLED == 3) {
            resumeLED = 3;
            LEDStopGame_State = LEDStopGame_StopLed3;
         }
         else {
            LEDStopGame_State = LEDStopGame_Reset;
         }
         break;
     
      default:
         LEDStopGame_State = LEDStopGame_Led1;
         break;
   } 

   switch(LEDStopGame_State) { 
      case LEDStopGame_SMStart:
         break;
      
      case LEDStopGame_Led1:
         PORTB = 0x01;
         break;
      case LEDStopGame_Led2:
         PORTB = 0x02;
         break;
      case LEDStopGame_Led3:
         PORTB = 0x04;
         break;

      case LEDStopGame_StopLed1:
         break;
      case LEDStopGame_StopLed2:
         break;
      case LEDStopGame_StopLed3:
         break;
         
      case LEDStopGame_Reset:
         break;

      default:
      break;
   } 
}

void main() {
   
   DDRA = 0x00; PORTA = 0xFF;
   DDRB = 0xFF; PORTB = 0x00;

   TimerSet(300);
   TimerOn();

   LEDStopGame_State = LEDStopGame_SMStart;
   PORTB = 0x00;

   while(1) {
      TickFct_LEDStopGame();
      while(!TimerFlag);
      TimerFlag = 0;
   } 
}