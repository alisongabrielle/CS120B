/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.8 --- 4/28/2021 17:16:47 PST
*/

#include "rims.h"

unsigned char ThreeLEDs_Clk;
void TimerISR() {
   ThreeLEDs_Clk = 1;
}

enum ThreeLEDs_States { ThreeLEDs_SMStart, ThreeLEDs_Led1 , ThreeLEDs_Led2, ThreeLEDs_Led3 } ThreeLEDs_State;

TickFct_ThreeLEDs() {
   switch(ThreeLEDs_State) {
      
      case ThreeLEDs_SMStart:
         ThreeLEDs_State = ThreeLEDs_Led1 ;
         break;

      case ThreeLEDs_Led1 : 
         ThreeLEDs_State = ThreeLEDs_Led2;
         break;

      case ThreeLEDs_Led2: 
         ThreeLEDs_State = ThreeLEDs_Led3;
         break;

      case ThreeLEDs_Led3: 
         ThreeLEDs_State = ThreeLEDs_Led1 ;
         break;

      default:
         ThreeLEDs_State = ThreeLEDs_Led1;
         break;

   switch(ThreeLEDs_State) {
      
      case ThreeLEDs_Led1 :
         PORTB = 0x01;
         break;

      case ThreeLEDs_Led2:
         PORTB = 0x02;
         break;

      case ThreeLEDs_Led3:
         PORTB = 0x04;
         break;
      
      default:
         break;
   } 

}

int main() {

   DDRB = 0xFF; PORTB = 0x00;

   const unsigned int periodThreeLEDs = 1000; // 1000 ms = 1 second
   TimerSet(periodThreeLEDs);
   TimerOn();
   
   ThreeLEDs_State = ThreeLEDs_SMStart; 
   PORTB = 0; 

   while(1) {
      TickFct_ThreeLEDs();
      while(!ThreeLEDs_Clk);
      ThreeLEDs_Clk = 0;
   } 
} 