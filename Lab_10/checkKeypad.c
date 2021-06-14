/*	Author: Alison Gonzales
 *  Partner(s) Name: Alison Gonzales
 *	Lab Section: 023
 *	Assignment: Lab # 10  Exercise # 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: 
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "keypad.h"
#include "scheduler.h"


int main(void) {

   unsigned char x;
   DDRB = 0xFF; keypad_output = 0x00; // keypad_output set to output, outputs init 0s
   DDRC = 0xF0; PORTC = 0x0F; // PC7..4 outputs init 0s, PC3..0 inputs init 1s 

   while (1) {
      x = GetKeypadKey();

      switch (x) {
         case '\0': keypad_output = 0x01F; break; //All 5 LEDs on
         case '1': keypad_output = 0x01; break; //hex equivalent
         case '2': keypad_output = 0x02; break;
         case '3': keypad_output = 0x03; break;
         case '4': keypad_output = 0x04; break;
         case '5': keypad_output = 0x05; break;
         case '6': keypad_output = 0x06; break;
         case '7': keypad_output = 0x07; break;
         case '8': keypad_output = 0x08; break;
         case '9': keypad_output = 0x09; break;
         case 'A': keypad_output = 0x0A; break;
         case 'B': keypad_output = 0x0B; break;
         case 'C': keypad_output = 0x0C; break;
         case 'D': keypad_output = 0x0D; break;
         case '*': keypad_output = 0x0E; break;
         case '0': keypad_output = 0x00; break;
         case '#': keypad_output = 0x0F; break;
         default: keypad_output = 0x1B; break; //Should never occur. Middle LED off.
      }
   }

}