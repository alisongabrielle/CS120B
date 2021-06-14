volatile unsigned char TimerFlag = 0;

void TimerISR() {
   TimerFlag = 1;
}

enum HoldAdd_States {HoldAdd_SMStart, HoldAdd_Init, HoldAdd_Add, HoldAdd_Depress } HoldAdd_State;

void TickFct_HoldAdd() {
   switch(HoldAdd_State) { // Transitions
      case HoldAdd_SMStart:
         HoldAdd_State = HoldAdd_Init;
         break;
         
         case HoldAdd_Init: 
         if (A0) {
            HoldAdd_State = HoldAdd_Init;
         }
         else if (!A0) {
            HoldAdd_State = HoldAdd_Add;
         }
         break;
      case HoldAdd_Add: 
         if (A0) {
            HoldAdd_State = HoldAdd_Depress;
         }
         else if (!A0) {
            HoldAdd_State = HoldAdd_Add;
         }
         break;
      case HoldAdd_Depress: 
         if (A0) {
            HoldAdd_State = HoldAdd_Init;
         }
         break;
      default:
         HoldAdd_State = HoldAdd_Init;
         break;
   } 

   switch(HoldAdd_State) { 
      case HoldAdd_Init:
         B = 0;
         break;
      case HoldAdd_Add:
         B = B + 1;
         break;
      case HoldAdd_Depress:
         break;
      default:
      break;
   } 

}

int main() {

   const unsigned int periodHoldAdd = 1000; // 1000 ms default
   TimerSet(periodHoldAdd);
   TimerOn();
   
   HoldAdd_State = HoldAdd_SMStart; // Initial state
   B = 0; // Init outputs

   while(1) {
      TickFct_HoldAdd();
      while(!TimerFlag);
      TimerFlag = 0;
   }
} 