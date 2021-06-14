/*	Author: Alison Gonzales
 *  Partner(s) Name: Alison Gonzales
 *	Lab Section: 023
 *	Assignment: Lab # 10  Exercise # 3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Demo Link: https://youtu.be/EHCUMG0SQJo
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "keypad.h"
#include "scheduler.h"
#include "timer.h"
#include "pwm.h"

//----------Shared Variables---------------------
unsigned char lock = 0x00; // global variable that incdicates if house is lock or unlock
//unsigned char pressKeyPad = 0; //to be use to check if there's any keypad buttons that are pressed
unsigned short int prevState = 0; //make sures that buttons are pressed properly
unsigned char keypad_output = 0x00;
unsigned char wasLock = 0;
//----------End Shared Variables-----------------

//Enumeration of states
enum UnlockSM_States { UnlockSM_Wait, UnlockSM_Release, UnlockSM_Sharp, UnlockSM_1, UnlockSM_2, UnlockSM_3, UnlockSM_4, UnlockSM_5, UnlockSM_UnLock};

int UnlockTickSM (int state) {
   // Local Variable
   unsigned char press = GetKeypadKey();
	//pressKeyPad = press;
   
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
         else if (wasLock == 1) { //only happens when PB7 was pressed. 
            state = UnlockSM_Wait; 
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

   case UnlockSM_UnLock: lock = 0x01; break; //PB0 = on;

   default: break;
   }

   return state; 
}

enum pauseButtonSM_States { pauseButton_wait, pauseButton_press, pauseButton_release };

int pauseButtonSMTick (int state) {
   // Local Variable
   unsigned char press = ~PINB & 0x80;
   
   switch (state) {
      case pauseButton_wait:
         state = press == 0x80? pauseButton_press: pauseButton_wait; break;
      case pauseButton_press:
	state = pauseButton_release; break;
      case pauseButton_release: 
	state = press == 0x00? pauseButton_wait: pauseButton_press; break;
         default: state = pauseButton_wait; break;
   }

   switch (state) {
      case pauseButton_wait: break;
      case pauseButton_press: 
	wasLock = 1;         
	lock = 0x00;  //PB0 = 0.
         break;
      case pauseButton_release: break;
   }
   return state; 
}

enum Doorbell_states { Doorbell_SMStart, Doorbell_init, Doorbell_tone1, Doorbell_wait };
double Doorbell[6] = { 440.00, 329.63, 493.88, 261.63, 392.00, 0};
short int j = 0;

int DoorbellSM( int state) {

    unsigned tmpA = ~PINA & 0x80;

   switch(state) { 
    
    case Doorbell_SMStart: state = Doorbell_init; break;

    case Doorbell_init: 
         if (tmpA == 0x00) {
            state = Doorbell_init;
         }
         else if (tmpA == 0x80) {
            state = Doorbell_tone1;
         }
         break;

    case Doorbell_tone1: 
         if (j  == 6) {
            state = Doorbell_wait;
         }
         else if (j < 6) {
            state = Doorbell_tone1;
         }
         break;

    case Doorbell_wait: 
         if (tmpA == 0x00) {
            state = Doorbell_wait;
         }
         else if (tmpA == 0x80) {
            PWM_on();
	    state = Doorbell_tone1;
         }
         break;
      
      default: state = Doorbell_init; break;
   } 

   switch (state) { 
      case Doorbell_SMStart: break;

      case Doorbell_init: j = 0; PWM_on(); break;

      case Doorbell_tone1: set_PWM(Doorbell[j]); j++; break;

      case Doorbell_wait: PWM_off(); j = 0; break;
      
      default: break;
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
      output = lock;
      break;
   }
   PORTB = output;
   return state;
}

int main(void) {

   DDRA = 0x00; PORTA = 0xFF;
   DDRC = 0xF0; PORTC = 0x0F;
   DDRB = 0x4F; PORTB = 0x00;

   static task task1, task2, task3, task4;
   task *tasks[] = { &task1, &task2, &task3, &task4};

   const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

   const char start = -1;

   task1.state = start;
   task1.period = 500;
   task1.elapsedTime = task1.period;
   task1.TickFct = &UnlockTickSM;

   task2.state = start;
   task2.period = 50;
   task2.elapsedTime = task2.period;
   task2.TickFct = &pauseButtonSMTick; 

   task3.state = start;
   task3.period = 200;
   task3.elapsedTime = task3.period;
   task3.TickFct = &DoorbellSM;

   task4.state = start;
   task4.period = 10;
   task4.elapsedTime = task4.period;
   task4.TickFct = &displaySMTick;

   unsigned short i;

   unsigned long GCD = tasks[0] -> period;
   for ( i = 1; i < numTasks; i++) {
      GCD = findGCD (GCD, tasks[i]->period); 
   }

   TimerSet(GCD);
   TimerOn();
   PWM_on();
   
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