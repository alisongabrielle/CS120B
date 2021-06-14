/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 5/7/2021 12:9:43 PST
*/

#include "rims.h"

/*Define user variables and functions for this state machine here.*/
unsigned char tmpA = ~PINA & 0x07;
unsigned char SM1_Clk;
void TimerISR() {
   SM1_Clk = 1;
}

enum SM1_States { SM1_Wait, SM1_C4, SM1_D4, SM1_E4 } SM1_State;

TickFct_State_machine_1() {
   switch(SM1_State) { // Transitions
      case -1:
         SM1_State = SM1_Wait;
         break;
         case SM1_Wait: 
         if ((tmpA == 0x00)) {
            SM1_State = SM1_Wait;
         }
         else if ((tmpA == 0x01)) {
            SM1_State = SM1_C4;
         }
         else if ((tmpA == 0x02)) {
            SM1_State = SM1_D4;
         }
         else if ((tmpA == 0x04)) {
            SM1_State = SM1_E4;
         }
         break;
      case SM1_C4: 
         if (1) {
            SM1_State = SM1_C4;
            (tmpA == 0x01)
         }
         else if ((tmpA == 0x00)) {
            SM1_State = SM1_Wait;
         }
         break;
      case SM1_D4: 
         if ((tmpA == 0x02)) {
            SM1_State = SM1_D4;
         }
         else if ((tmpA == 0x00)) {
            SM1_State = SM1_Wait;
         }
         break;
      case SM1_E4: 
         if ((tmpA == 0x04)) {
            SM1_State = SM1_E4;
         }
         else if ((tmpA == 0x00)) {
            SM1_State = SM1_Wait;
         }
         break;
      default:
         SM1_State = SM1_Wait;
   } // Transitions

   switch(SM1_State) { // State actions
      case SM1_Wait:
         break;
      case SM1_C4:
         set_PWM(261.63);
         break;
      case SM1_D4:
         set_PWM(293.66);
         break;
      case SM1_E4:
         set_PWM(329.23);
         break;
      default: // ADD default behaviour below
      break;
   } // State actions

}

int main() {

   const unsigned int periodState_machine_1 = 1000; // 1000 ms default
   TimerSet(periodState_machine_1);
   TimerOn();
   
   SM1_State = -1; // Initial state
   B = 0; // Init outputs

   while(1) {
      TickFct_State_machine_1();
      while(!SM1_Clk);
      SM1_Clk = 0;
   } // while (1)
} // Main