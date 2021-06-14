#include "RIMS.h"
short int prevState = 0;
short int pauseLED = 0;
unsigned char LEDStopGame_Clk;
void TimerISR() {
   LEDStopGame_Clk = 1;
}

enum LEDStopGame_States { LEDStopGame_Led1, LEDStopGame_Led2, LEDStopGame_Led3, 
LEDStopGame_StopLed1, LEDStopGame_StopLed2, 
LEDStopGame_StopLed3, LEDStopGame_Reset } LEDStopGame_State;

TickFct_LEDStopGame() {
   switch(LEDStopGame_State) { // Transitions
      case -1:
         LEDStopGame_State = LEDStopGame_Led1;
         break;
         
         case LEDStopGame_Led1: 
         if (!A0 ) {
            prevState = 1;
            LEDStopGame_State = LEDStopGame_Led2;
         }
         else if (A0) {
            LEDStopGame_State = LEDStopGame_StopLed1;
         }
         break;
         
         case LEDStopGame_StopLed1: 
         if (!A0) {
            LEDStopGame_State = LEDStopGame_Reset;
         }
         else {
            pauseLED = 1;
            LEDStopGame_State = LEDStopGame_StopLed1;
         }
         break;
         
      case LEDStopGame_Led2: 
         if (!A0 && prevState == 1) {
            LEDStopGame_State = LEDStopGame_Led3;
         }
         else if (!A0 && prevState == 3) {
            LEDStopGame_State = LEDStopGame_Led1;
         }
         else if (A0) {
            LEDStopGame_State = LEDStopGame_StopLed2;
            prevState = 2;
         }
         break;
         
      case LEDStopGame_StopLed2: 
         if (!A0) {
            LEDStopGame_State = LEDStopGame_Reset;
         }
         else {
            pauseLED = 2;
            LEDStopGame_State = LEDStopGame_StopLed2;
         }
         break;
         
      case LEDStopGame_Led3: 
         if (A0) {
            LEDStopGame_State = LEDStopGame_StopLed3;
         }
         else {
            prevState = 3;
            LEDStopGame_State = LEDStopGame_Led2;
         }
         break;
      
      case LEDStopGame_StopLed3: 
         if (!A0) {
            LEDStopGame_State = LEDStopGame_Reset;
         }
         else {
            pauseLED = 3;
            LEDStopGame_State = LEDStopGame_StopLed3;
         }
         break;
      
      case LEDStopGame_Reset:
         if (A0 && pauseLED == 1) {
            LEDStopGame_State = LEDStopGame_Led1;
         }
         else if (A0 && pauseLED == 2) {
            LEDStopGame_State = LEDStopGame_Led2;
         }
         else if (A0 && pauseLED == 3) {
            LEDStopGame_State = LEDStopGame_Led3;
         }
         else {
            LEDStopGame_State = LEDStopGame_Reset;
         }
         break;
     
      default:
         LEDStopGame_State = LEDStopGame_Led1;
         break;
   } // Transitions

   switch(LEDStopGame_State) { // State actions
      case LEDStopGame_Led1:
         B = 0x01;
         
         break;
      case LEDStopGame_Led2:
         B = 0x02;
         
         break;
      case LEDStopGame_Led3:
         B = 0x04;
         
         break;
      case LEDStopGame_StopLed1:
         break;
      case LEDStopGame_StopLed2:
         break;
      case LEDStopGame_StopLed3:
         break;
         
      case LEDStopGame_Reset:
         break;
      default: // ADD default behaviour below
      break;
   } // State actions
}

void main() {

   const unsigned int periodLEDStopGame = 1000; // 1000 ms default
   TimerSet(periodLEDStopGame);
   TimerOn();
   
   LEDStopGame_State = -1; // Initial state
   B = 0; // Init outputs

   while(1) {
      TickFct_LEDStopGame();
      while(!LEDStopGame_Clk);
      LEDStopGame_Clk = 0;
   } // while (1)
} // Main