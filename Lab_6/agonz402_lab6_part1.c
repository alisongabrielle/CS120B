/*	Name & E-mail: Alison Gonzales agonz402@ucr.edu
 *	Lab Section: 021
 *	Assignment: Lab # 6 Exercise # 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * Demo Link: https://youtu.be/kpQV7gmMXc0
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

enum ThreeLEDs_States { ThreeLEDs_SMStart, ThreeLEDs_Led1 , ThreeLEDs_Led2, ThreeLEDs_Led3 } ThreeLEDs_State;

void TickFct_ThreeLEDs() {
  
   switch(ThreeLEDs_State) {
      
      case ThreeLEDs_SMStart:
         ThreeLEDs_State = ThreeLEDs_Led1;
         break;

      case ThreeLEDs_Led1 : 
         ThreeLEDs_State = ThreeLEDs_Led2;
         break;

      case ThreeLEDs_Led2: 
         ThreeLEDs_State = ThreeLEDs_Led3;
         break;

      case ThreeLEDs_Led3: 
         ThreeLEDs_State = ThreeLEDs_Led1;
         break;

      default:
         ThreeLEDs_State = ThreeLEDs_Led1;
         break;
   }

   switch(ThreeLEDs_State) {
      
      case ThreeLEDs_Led1 :
         PORTB = 0x01;
         break;

      case ThreeLEDs_Led2:
         PORTB = 0x02;
         break;

      case ThreeLEDs_Led3:
         PORTB = 0x04;
         break;
      
      default:
         break;
   }
}

void main() {
   DDRB = 0xFF; PORTB = 0x00;

   TimerSet(1000);
   TimerOn();

   while(1) {
      TickFct_ThreeLEDs();
      while(!TimerFlag);
      TimerFlag = 0;
   } 
}