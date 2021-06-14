/*	Author: Alison Gonzales
 *  Partner(s) Name: Alison Gonzales
 *	Lab Section: 023
 *	Assignment: Lab # 8  Exercise # 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://youtu.be/O0CKH-Ub1HU
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

enum Power_States { Power_SMStart, Power_Off, Power_On, Power_OffWait } Power_State;

void TickFct_Power() {

  switch(Power_State) {

      case  Power_SMStart:
          Power_State = Power_Off;
         break;

      case Power_Off:
        if (tmpA == 0x04) {
          Power_State = Power_On;
        }
        else if (tmpA == 0x00) {
          Power_State = Power_Off;
        }
        
        break;

      case Power_On: 
         if ( tmpA == 0x00 ) {
            Power_State = Power_OffWait;
         }
         else if (tmpA == 0x04) {
            Power_State = Power_On;
         }
         break;

      case Power_OffWait: 
         if (tmpA == 0x04) {
            Power_State = Power_Off;
         }
         else if (tmpA == 0x00) {
            Power_State = Power_OffWait;
         }
         break;

      default:
         Power_State = Power_Off;
         break;
   }

   switch(Power_State) { 
      case Power_Off:
         PWM_off();
         break;
      
      case Power_On:
         PWM_on();
	      set_PWM(261.63);
         break;
      
      case Power_OffWait:
         break;
      
      default: 
      break;
   } 

}


enum Scale_STATES { Scale_SMStart, Scale_Begin, Scale_Init, Scale_Inc, 
Scale_Dec, Scale_WaitInc, Scale_WaitDec, Scale_Reset } Scale_STATE;

double tone[8] = { 261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25 };
short int i;
short int curr;

void TickFct_Scale() {
    
    switch (Scale_STATE) {

        case (Scale_SMStart):
            Scale_STATE = Scale_Begin;
            break;
         
       case (Scale_Begin):
            i = 0;

            if (tmpA == 0x01) {
                Scale_STATE = Scale_Inc;
            }
            else if (tmpA == 0x02) {
                Scale_STATE = Scale_Dec;
            }
            else {
                Scale_STATE = Scale_Init;
            }
            break;

        case (Scale_Init):

            if (tmpA == 0x00) {
                Scale_STATE = Scale_Init;
            }
            else if (tmpA == 0x01) {
                Scale_STATE = Scale_Inc;
            }
            else if (tmpA == 0x02) {
                Scale_STATE = Scale_Dec;
            }
            break;

        case (Scale_Inc):
            if (tmpA == 0x03){
                Scale_STATE = Scale_Reset;
            }
            else if (tmpA == 0x02) {
                Scale_STATE = Scale_Dec;
            }
            else if (tmpA == 0x01) {
               Scale_STATE = Scale_WaitInc;  
            }
            else {
                Scale_STATE = Scale_Init;
            }
            break;

        case (Scale_WaitInc):
            if (tmpA == 0x02) {
                Scale_STATE = Scale_Dec;
            }
            else if (tmpA == 0x03) {
                Scale_STATE = Scale_Reset;
            }
            else if (tmpA == 0x01) {
                Scale_STATE = Scale_WaitInc;
            }      
            else {
               Scale_STATE = Scale_Init;
            }
            break;
        
        case (Scale_Dec):
            if (tmpA == 0x03){
                Scale_STATE = Scale_Reset;
            }
            else if (tmpA == 0x01) {
                Scale_STATE = Scale_Inc;
            }
            else if (tmpA == 0x02) {
               Scale_STATE = Scale_WaitDec;  
            }
            else {
                Scale_STATE = Scale_Init;
            }
            break;

        case (Scale_WaitDec):
            if (tmpA == 0x01) {
                Scale_STATE = Scale_Inc;
            }
            else if (tmpA == 0x03) {
                Scale_STATE = Scale_Reset;
            }
            else if (tmpA == 0x02) {
               Scale_STATE = Scale_WaitDec;  
            }
            else {
                Scale_STATE = Scale_Init;
            }          
            break;

        case (Scale_Reset):
            if (tmpA == 0x01) {
                Scale_STATE = Scale_Inc;
            }
            else if (tmpA == 0x02) {
                Scale_STATE = Scale_Dec;
            }
            else {
                Scale_STATE = Scale_Reset;
            }
            break;

        default:
            Scale_STATE = Scale_SMStart;
            break;
        
    }

    switch (Scale_STATE){

        case (Scale_SMStart):
            set_PWM(261.63);
            break;

        case (Scale_Begin) :
            set_PWM(261.63);
            break;        
        case (Scale_Init):
            break;

        case (Scale_Inc):
            curr = i;
            if (i < 7) {
               i++;
            }
            else {
               i = 7;
               curr = 7;
            }
            set_PWM(tone[curr]);
            break;

        case (Scale_Dec):
            curr = i;
            if (i > 0) {
                i--;
            }
            set_PWM(tone[curr]);
            break;
        
        case (Scale_WaitInc):
            set_PWM(tone[curr]);
            break;
        
        case (Scale_WaitDec):
            set_PWM(tone[curr]);
            break;

        case (Scale_Reset):
            set_PWM(261.63);
            break;
        
    }
}


void main () {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0x40; PORTB = 0x00;

    TimerSet(100);
    TimerOn();
    PWM_on();

    Power_State = Power_SMStart;
    Scale_STATE = Scale_SMStart; 

    while (1) {
	
      tmpA = ~PINA & 0x07;    

      TickFct_Power();
      TickFct_Scale();

      while(!TimerFlag);
      TimerFlag = 0;

    }
}