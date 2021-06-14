/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 5/7/2021 14:49:0 PST
*/

#include "rims.h"

/*Define user variables and functions for this state machine here.*/
unsigned char tmpA = ~PINA & 0x01; //PA0 -> on/off button
unsigned char Power_Clk;
void TimerISR() {
   Power_Clk = 1;
}

enum Power_States { Power_Off, Power_On, Power_OffWait } Power_State;

TickFct_Power() {
   switch(Power_State) { // Transitions
      case -1:
         Power_State = Power_Off;
         break;
         case Power_Off: 
         if (tmpA == 0x01;) {
            Power_State = Power_On;
         }
         else if (tmpA == 0x00;) {
            Power_State = Power_Off;
         }
         break;
      case Power_On: 
         if (tmpA == 0x00;) {
            Power_State = Power_OffWait;
         }
         else if (tmpA == 0x01;) {
            Power_State = Power_On;
         }
         break;
      case Power_OffWait: 
         if (tmp == 0x01;) {
            Power_State = Power_Off;
         }
         else if (tmpA == 0x00;) {
            Power_State = Power_OffWait;
         }
         break;
      default:
         Power_State = Power_Off;
   } // Transitions

   switch(Power_State) { // State actions
      case Power_Off:
         break;
      case Power_On:
         PWN_on()
         break;
      case Power_OffWait:
         break;
      default: // ADD default behaviour below
      break;
   } // State actions

}

int main() {

   const unsigned int periodPower = 1000; // 1000 ms default
   TimerSet(periodPower);
   TimerOn();
   
   Power_State = -1; // Initial state
   B = 0; // Init outputs

   while(1) {
      TickFct_Power();
      while(!Power_Clk);
      Power_Clk = 0;
   } // while (1)
} // Main