/*	Author: Alison Gonzales
 *  Partner(s) Name: Alison Gonzales
 *	Lab Section: 023
 *	Assignment: Lab # 10  Exercise # 4
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
#include "pwm.h"

//----------Shared Variables---------------------
unsigned char lock = 0x00; // global variable that incdicates if house is lock or unlock
unsigned short int prevState = 0; //make sures that buttons are pressed properly
unsigned char keypad_output = 0x00;

//----------End Shared Variables-----------------

//Enumeration of states
enum UnlockSM_States { UnlockSM_Wait, UnlockSM_Release, UnlockSM_1, UnlockSM_2, UnlockSM_3, UnlockSM_4, UnlockSM_5, UnlockSM_UnLock};

int UnlockTickSM (int state) {
   // Local Variable
   unsigned char press = GetKeypadKey();
	//pressKeyPad = press;
   
   switch (state) {
      case UnlockSM_Wait:
         prevState = 0;
         state = press == button1 ? UnlockSM_Sharp : UnlockSM_Wait; break;
      
      case UnlockSM_1:
         prevState = 1; //from #
         state = press == '\0'? UnlockSM_Release : UnlockSM_Sharp; break;
      
      case UnlockSM_2:
         prevState = 2; //from 1
         state = press == '\0'? UnlockSM_Release : UnlockSM_1; break;
      
      case UnlockSM_3:
         prevState = 3; //from 2
         state = press == '\0'? UnlockSM_Release : UnlockSM_2; break;
      
      case UnlockSM_4:
         prevState = 4; //from 3
         state = press == '\0'? UnlockSM_Release : UnlockSM_4; break;

      case UnlockSM_UnLock:
         state = UnlockSM_Release; break;

      case UnlockSM_Release:
         if (press == button2 && prevState == 1) { 
            state = UnlockSM_2;
         }
         else if (press == button3 && prevState == 2) {
            state = UnlockSM_3;
         }
         else if (press == button4 && prevState == 3) {
            state = UnlockSM_4;
         }
         else if (prevState == 4) {
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

   case UnlockSM_1: break;
   case UnlockSM_2: break;
   case UnlockSM_3: break;
   case UnlockSM_4: break;

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

unsigned char doublePress2 = 0; //check if it's both press2 or not.

enum DoublePress2_States { DoublePress2_Wait, DoublePress2_Hold, DoublePress2_Release } DoublePress2_State;

void DoublePress2SM() {
   
   unsigned char bothPress = (~PINB & 0x80) + '#';


   switch (DoublePress2_State) { 
      
      case DoublePress2_Wait:
         DoublePress2_State = (bothPress) ? DoublePress2_Hold : DoublePress2_Wait; break;

      case DoublePress2_Hold:
         DoublePress2_State = (!bothPress) ? DoublePress2_Release : DoublePress2_Hold; break;
      
      case DoublePress2_Release:
         DoublePress2_State = (bothPress) ? DoublePress2_Hold : DoublePress2_Release; break;

   }

   switch (DoublePress2_State) {
      
      case DoublePress2_Wait:
         doublePress2 = 0; break;

      case DoublePress2_Hold:
         doublePress2 = 1; break;

      case DoublePress2_Release:
         doublePress2 = 0; break;
   }

}

enum ChangeComb_States { ChangeComb_Wait, ChangeComb_WaitStore, ChangeComb_1, ChangeComb_2, ChangeComb_3, ChangeComb_4, ChangeComb_Release }; 
unsigned char button1, button2, button3, button4;
unsigned char prevState2 = 0;

int ChangeCombSM( int ChangeComb_State ) {

   unsigned char press2 = GetKeypadKey();

   switch (ChangeComb_State) {

      case ChangeComb_Wait:
         ChangeComb_State = doublePress2 ? ChangeComb_WaitStore : ChangeComb_Wait; break; 
      
      case ChangeComb_WaitStore:
         prevState2 = 9;
         ChangeComb_State = (press2 != '\0') ? ChangeComb_1 : ChangeComb_WaitStore; break;
      
      case ChangeComb_1:
         prevState2 = 1;
         ChangeComb_State = (press2 == '\0') ? ChangeComb_Release : ChangeComb_1; break;
      
      case ChangeComb_2:
         prevState2 = 2;
         ChangeComb_State = (press2 == '\0') ? ChangeComb_Release : ChangeComb_2; break;

      case ChangeComb_3:
         prevState2 = 3;
         ChangeComb_State = (press2 == '\0') ? ChangeComb_Release : ChangeComb_3; break;
      
      case ChangeComb_4:
         prevState2 = 4;
         ChangeComb_State = (press2 == '\0') ? ChangeComb_Release : ChangeComb_4; break;

      case ChangeComb_Release:
         
         if (press2 != '\0' && prevState2 == 9) { 
            ChangeComb_State = ChangeComb_1;
         }
         else if (press2 != '\0' && prevState2 == 1) {
            ChangeComb_State = ChangeComb_2;
         }
         else if (press2 != '\0' && prevState2 == 2) {
            ChangeComb_State = ChangeComb_3;
         }
         else if (press2 != '\0' && prevState2 == 3) {
            ChangeComb_State = ChangeComb_4;
         }
         else {
            ChangeComb_State = ChangeComb_Release;
         }
         break;

      default: ChangeComb_State = ChangeComb_Wait; break;
   }

   switch (ChangeComb_State) {

      case ChangeComb_Wait: break;
      case ChangeComb_WaitStore: break; 

      case ChangeComb_1: button1 = press2; break;      
      case ChangeComb_2: button2 = press2; break;
      case ChangeComb_3: button3 = press2; break;
      case ChangeComb_4: button4 = press2; break;

      case ChangeComb_Release: break;

      default: break;
   
   }

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

   task5.state = start;
   task5.period = 200;
   task5.elapsedTime = task5.period;
   task5.TickFct = &ChangeCombSM;

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