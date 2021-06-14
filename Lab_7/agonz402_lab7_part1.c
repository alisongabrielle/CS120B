/*	Author: Alison Gonzales
 *  Partner(s) Name: Alison Gonzales
 *	Lab Section: 023
 *	Assignment: Lab # 7  Exercise # 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://youtu.be/TN8VK7zorjE
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
    DDRD = 0xFF; PORTD = 0x00;

    PORTB = 0x00;
    PORTD = 0x00;

    ADC_init();

    while (1) {

      unsigned short potInput = ADC; //10-bit input from potentiometer

      PORTB = (char) potInput; // lower 8 bits
      PORTD = (char) potInput >> 8; //upper 2 bits  

    }

    return 0;
}