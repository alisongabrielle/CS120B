/*	Author: Alison Gonzales
 *  Partner(s) Name: Alison Gonzales
 *	Lab Section: 023
 *	Assignment: Lab # 8  Exercise # 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://youtu.be/ryS0h-teryw
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char tmpA = 0x00;

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

// 0.954 hz is lowest frequency possible with this function
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound

void set_PWM(double frequency) {
  static double current_frequency; //Keeps track of the currently set frequency
  // Will only update the registers when the frequency changes, otherwise allows
  //music to play uninterrupted

  if (frequency != current_frequency) {
    if (!frequency) { TCCR3B &= 0X08; } //stops timer/counter
    else { TCCR3B |= 0x03; } //resumes/continues timer/counter

    //prevents OCR3A from overflowing, using prescaler 64
    //0.954 is smallest frequency that will not result in overflow
    if (frequency < 0.954) { OCR3A = 0xFFFF; }

    //prevents OCRA from underflowing, using prescaler 64
    // 31250 is largest frequency that will not result in underflow
    else if (frequency > 31250) { OCR3A = 0x0000; }

    //set OCRA based on desired frequency
    else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }

    TCNT3 = 0; //resets counter
    //current_frequency = frequency; //Updates the current frequency
  } 
}

void PWM_on() {
  TCCR3A = (1 << COM3A0);
    //COM3A0: Toggle PB3 on compare match between counter and OCR3A
  TCCR3B = (1 << WGM32) | ( 1 << CS31) | (1 << CS30);
    // WGM32: When counter (TCNT3) matches OCR3A, reset counter
  set_PWM(0);
}

void PWM_off() {
  TCCR3A = 0x00;
  TCCR3B = 0x00;
}


enum Melody_States { Melody_SMStart, Melody_Init, Melody_tone1, Melody_Wait } Melody_State;
double melody[6] = { 440.00, 329.63, 493.88, 261.63, 392.00, 0};
short int i = 0;

void TickFct_Melody() {

   switch(Melody_State) { 
    
    case Melody_SMStart:
         Melody_State = Melody_Init;
         break;

    case Melody_Init: 
         if (tmpA == 0x00) {
            Melody_State = Melody_Init;
         }
         else if (tmpA == 0x01) {
            Melody_State = Melody_tone1;
         }
         break;
    case Melody_tone1: 
         if (i  == 6) {
            Melody_State = Melody_Wait;
         }
         else if (i < 6) {
            Melody_State = Melody_tone1;
         }
         break;
    case Melody_Wait: 
         if (tmpA == 0x00) {
            Melody_State = Melody_Wait;
         }
         else if (tmpA == 0x01) {
            Melody_State = Melody_tone1;
         }
         break;
      
      default:
         Melody_State = Melody_Init;
         break;
   } 

   switch (Melody_State) { 
      case Melody_SMStart:
         break;

      case Melody_Init:
         i = 0;
         set_PWM(0);
         break;

      case Melody_tone1:
         set_PWM(melody[i]);
         i++;
         break;

      case Melody_Wait:
         set_PWM(0);
         i = 0;
         break;
      default: 
      break;
   } 

}


void main () {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0x40; PORTB = 0x00;

    TimerSet(1000);
    TimerOn();
    PWM_on();

    Melody_State = Melody_SMStart;

    while (1) {
	
      tmpA = ~PINA & 0x01;    

      TickFct_Melody();

      while(!TimerFlag);
      TimerFlag = 0;

    }
}