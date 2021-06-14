/*	Author: Alison Gonzales
 *  Partner(s) Name: Alison Gonzales
 *	Lab Section: 023
 *	Assignment: Lab # 10  Exercise # 2
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
#include "timer.h"

//----------Shared Variables---------------------
unsigned char lock = 0x00; // global variable that incdicates if house is lock or unlock
unsigned char pressKeyPad = 0; //to be use to check if there's any keypad buttons that are pressed
unsigned char pressPB7 = 0; //making sure that PB7 is not pressed
unsigned short int prevState = 0; //make sures that buttons are pressed properly
unsigned char keypad_output = 0x00;
//----------End Shared Variables-----------------

//Enumeration of states
enum UnlockSM_States { UnlockSM_Wait, UnlockSM_Release, UnlockSM_Sharp, UnlockSM_1, UnlockSM_2, UnlockSM_3, UnlockSM_4, UnlockSM_5, UnlockSM_UnLock};

int UnlockTickSM (int state) {
   // Local Variable
   unsigned char press = GetKeypadKey();
   pressKeyPad = press;
   
   switch (state) {
      case UnlockSM_Wait:
         prevState = 0;
         state = press == '#'? UnlockSM_Sharp : UnlockSM_Wait; break;
      
      case UnlockSM_Sharp:
         prevState = 9; //from #
         state = press == '\0'? UnlockSM_Release : UnlockSM_Sharp; break;
      
      case UnlockSM_1:
         prevState = 1; //from 1
         state = press == '\0'? UnlockSM_Release : UnlockSM_1; break;
      
      case UnlockSM_2:
         prevState = 2; //from 2
         state = press == '\0'? UnlockSM_Release : UnlockSM_2; break;
      
      case UnlockSM_3:
         prevState = 3; //from 3
         state = press == '\0'? UnlockSM_Release : UnlockSM_4; break;

      case UnlockSM_4:
         prevState = 4; //from 4
         state = press == '\0'? UnlockSM_Release : UnlockSM_5; break;
      
      case UnlockSM_5:
         prevState = 5; //from 4
         state = press == '\0'? UnlockSM_Release : UnlockSM_5; break;
      
      case UnlockSM_UnLock:
         state = UnlockSM_Release; break;

      case UnlockSM_Release:
         if (press == '1' && prevState == 9) { 
            state = UnlockSM_1;
         }
         else if (press == '2' && prevState == 1) {
            state = UnlockSM_2;
         }
         else if (press == '3' && prevState == 2) {
            state = UnlockSM_3;
         }
         else if (press == '4' && prevState == 3) {
            state = UnlockSM_4;
         }
         else if (press == '5' && prevState == 4) {
            state = UnlockSM_5;
         }
         else if (prevState == 5) {
            state = UnlockSM_UnLock;
         }
         else {
            state = UnlockSM_Release;
         }
         break;

      default: state = UnlockSM_Wait; break;
   }

   switch (state)
   {
   case UnlockSM_Wait: lock = 0x00; break; //PB0 = off

   case UnlockSM_Sharp: break;
   case UnlockSM_1: break;
   case UnlockSM_2: break;
   case UnlockSM_3: break;
   case UnlockSM_4: break;
   case UnlockSM_5: break;

   case UnlockSM_Release: break;

   case UnlockSM_UnLock: lock = 0x80; break; //PB0 = on;

   default: break;
   }

   //pressKeyPad = (press != '\0') ? 1 : 0 ; //checks if any keypads are press. 1 = keypads press, 0 not

   return state; 
}

enum keyPad_States { keyPad_wait, keyPad_pressKeyPad, keyPad_release };

int keyPadSMTick (int state) {
   
   switch (state) {
      case keyPad_wait:
         state = pressKeyPad != '\0'? keyPad_pressKeyPad: keyPad_wait; break;
      case keyPad_pressKeyPad:
         state = pressKeyPad != '\0'? keyPad_pressKeyPad: keyPad_release; break;
      case keyPad_release:
         state = pressKeyPad != '\0'? keyPad_pressKeyPad: keyPad_release; break;
      
      default: state = keyPad_wait; break;
   }

   switch (state) {
      case keyPad_wait: keypad_output = 0x01F; break;
      case keyPad_pressKeyPad: 
            switch (pressKeyPad) {
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
         break;
      case keyPad_release: keypad_output = 0x01F; break;
   }
   return state; 
}

enum display_States { display_display };

int displaySMTick(int state) {

   unsigned char output;

   switch (state) {
      case display_display: state = display_display; break;
      default: state = display_display; break;
   }
   switch (state)
   {
   case display_display:
      output = lock | keypad_output;
      break;
   }
   PORTB = output;
   return state;
}

int main(void) {

   DDRC = 0xF0; PORTC = 0x0F;
   DDRB = 0xFF; PORTB = 0x00;

   static task task1, task2, task3;
   task *tasks[] = { &task1, &task2, &task3};

   const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

   const char start = -1;

   task1.state = start;
   task1.period = 500;
   task1.elapsedTime = task1.period;
   task1.TickFct = &UnlockTickSM; 

   /*task2.state = start;
   task2.period = 500;
   task2.elapsedTime = task2.period;
   task2.TickFct = &B7LightSMTick;*/

   task2.state = start;
   task2.period = 500;
   task2.elapsedTime = task2.period;
   task2.TickFct = &keyPadSMTick; 

   task3.state = start;
   task3.period = 10;
   task3.elapsedTime = task3.period;
   task3.TickFct = &displaySMTick;

   unsigned short i;

   unsigned long GCD = tasks[0] -> period;
   for ( i = 1; i < numTasks; i++) {
      GCD = findGCD (GCD, tasks[i]->period); 
   }

   TimerSet(GCD);
   TimerOn();

   while (1) {
      for ( i = 0; i < numTasks; i++ ) {
         if (tasks[i]->elapsedTime == tasks[i]->period) {
            tasks[i]-> state = tasks[i] -> TickFct(tasks[i] -> state);
            tasks[i]-> elapsedTime = 0; 
         }
         tasks[i] -> elapsedTime += GCD;    
      }
      while (!TimerFlag);
      TimerFlag = 0;
   }
   return 0; 
}