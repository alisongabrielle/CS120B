/*	Author: Alison Gonzales
 *  Partner(s) Name: Alison Gonzales
 *	Lab Section: 023
 *	Assignment: Lab # 7  Exercise # 4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://youtu.be/iFTy3-hZipg
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

void ADC_init() {
    ADCSRA |= ( (1<< ADEN )| (1 << ADSC) | (1 << ADATE) );
    // ADEN: setting this bit enables analog-to-digital conversion.
    //ADSC: setting this bit starts the first concersion.
    //ADATE: setting this bit enables auto-triggering. Since we are 
    //      in Free Running Mode, a new conversion will trigger whenever
    //      the previous conversion completes.
}

int main () {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    //DDRD = 0xFF; PORTD = 0x00;

    PORTB = 0x00;
    //PORTD = 0x00;

    //after the demo, values found as shown below
    //unsigned short MAX = 255; //8-bit LEDs => 255

    ADC_init();

    while (1) {

      unsigned short photoResistor = ADC; //10-bit input from tmpB
      unsigned char tmpB = (char)photoResistor;

      if (tmpB >= 255 ) {
        PORTB = 0xFF;
      }
      else if (tmpB >= 187 && tmpB <= 217 ) {
        PORTB = 0x7F;
      }
      else if (tmpB >= 156 && tmpB <= 186 ) {
        PORTB = 0x3F;
      }
      else if (tmpB >= 125 && tmpB <= 155 ) {
        PORTB = 0x1F;
      }
      else if (tmpB >= 94 && tmpB <= 124 ) {
        PORTB = 0x0F;
      }

      else if (tmpB >= 63 && tmpB <= 93 ) {
        PORTB = 0x07;
      }

      else if (tmpB >= 32 && tmpB >= 62 ) {
        PORTB = 0x03;
      }
      
      else if (tmpB <= 31) {
        PORTB = 0x01;
      }


    }

    return 0;
}