/*	Author: Alison Gonzales
 *  Partner(s) Name: Alison Gonzales
 *	Lab Section: 023
 *	Assignment: Lab # 8  Exercise # 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://youtu.be/IH4O12BFXXE
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

// 0.954 hz is lowest frequency possible with this function
// based on settings in PWM_on()
// Passing in 0 as the frequency will stop the speaker from generating sound

void set_PWM(double frequency) {
  static double current_frequency; //Keeps track of the currently set frequency
  // Will only update the registers when the frequency changes, otherwise allows
  //music to play uninterrupted

  if (frequency != current_frequency) {
    if (!frequency) { TCCR3B &= 0x08; } //stops timer/counter
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
    current_frequency = frequency; //Updates the current frequency
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

int main () {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0x40; 

    unsigned char button = 0x00;
    PWM_on();

    while (1) {

      button = ~PINA & 0x07;

      if (button == 0x01) {
        set_PWM(261.63); //C4
      }
      else if (button == 0x02) {
        set_PWM(293.66); //D4
      }
      else if (button == 0x04) {
        set_PWM(329.23); //E4
      }
      else {
        set_PWM(0);
      }
    }

    return 0;

}