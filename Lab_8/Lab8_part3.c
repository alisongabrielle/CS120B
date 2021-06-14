/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 5/7/2021 22:59:17 PST
*/

#include "rims.h"

double melody[5] = { 0x04, 0x80, 0x40, 0x02, 0x08};
short int i = 0;
unsigned char Melody_Clk;
void TimerISR() {
   Melody_Clk = 1;
}

enum Melody_States { Melody_Init, Melody_tone1, Melody_Wait } Melody_State;

TickFct_State_machine_1() {
   switch(Melody_State) { // Transitions
      case -1:
         Melody_State = Melody_Init;
         break;
         case Melody_Init: 
         if (!A0) {
            Melody_State = Melody_Init;
         }
         else if (A0) {
            Melody_State = Melody_tone1;
         }
         break;
      case Melody_tone1: 
         if (i  == 5) {
            Melody_State = Melody_Wait;
         }
         else if (i < 5) {
            Melody_State = Melody_tone1;
         }
         break;
      case Melody_Wait: 
         if (!A0) {
            Melody_State = Melody_Wait;
         }
         else if (A0) {
            Melody_State = Melody_tone1;
            i = 0;
         }
         break;
      default:
         Melody_State = Melody_Init;
   } // Transitions

   switch(Melody_State) { // State actions
      case Melody_Init:
         i = 0;
         B = 0x00;
         break;
      case Melody_tone1:
         B = melody[i];
         i++;
         break;
      case Melody_Wait:
         break;
      default: // ADD default behaviour below
      break;
   } // State actions

}

int main() {

   const unsigned int periodState_machine_1 = 1000; // 1000 ms default
   TimerSet(periodState_machine_1);
   TimerOn();
   
   Melody_State = -1; // Initial state
   B = 0; // Init outputs

   while(1) {
      TickFct_State_machine_1();
      while(!Melody_Clk);
      Melody_Clk = 0;
   } // while (1)
} // Main