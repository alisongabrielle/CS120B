enum IncDec_States { IncDec_SMStart, IncDec_Begin, IncDec_Init, IncDec_Inc, IncDec_Dec, IncDec_WaitInc, IncDec_WaitDec, IncDec_Reset };

int TickFct_IncDec(int state) {
   
    player1.button = ~PINB & 0x60; //since we're playing with an AI
   
   switch (state) {
        case (IncDec_SMStart): 
            state = (start == 1 && player_mode == 1) ? IncDec_Begin : IncDec_SMStart; break;
         
       case (IncDec_Begin):
            if (player1.button == 0x40) {
                state = IncDec_Inc;
            }
            else if (player1.button == 0x20) {
                state = IncDec_Dec;
            }
            else {
                state = IncDec_Init;
            }
            break;

        case (IncDec_Init):
            if (player1.button == 0x00) {
                state = IncDec_Init;
            }
            else if (player1.button == 0x40) {
                state = IncDec_Inc;
            }
            else if (player1.button == 0x20) {
                state = IncDec_Dec;
            }
            break;

        case (IncDec_Inc):
            if (player1.button == 0x60){
                state = IncDec_Reset;
            }
            else if (player1.button == 0x20) {
                state = IncDec_Dec;
            }
            else if (player1.button == 0x40) {
               state = IncDec_WaitInc;  
            }
            else {
                state = IncDec_Init;
            }
            break;

        case (IncDec_WaitInc):
            if (player1.button == 0x20) {
                state = IncDec_Dec;
            }
            else if (player1.button == 0x60) {
                state = IncDec_Reset;
            }
            else if (player1.button == 0x40 && holdPress1 == 1) {
               state = IncDec_Inc;
               holdPress1 = 0;
            }
            else if (player1.button == 0x40) {
                holdPress1 = 1;
                state = IncDec_WaitInc;
            }
            else {
               state = IncDec_Init;
            }
            break;
        
        case (IncDec_Dec):
            if (player1.button == 0x60){
                state = IncDec_Reset;
            }
            else if (player1.button == 0x40) {
                state = IncDec_Inc;
            }
            else if (player1.button == 0x20) {
               state = IncDec_WaitDec;  
            }
            else {
                state = IncDec_Init;
            }
            break;

        case (IncDec_WaitDec):
            if (player1.button == 0x40) {
                state = IncDec_Inc;
            }
            else if (player1.button == 0x60) {
                state = IncDec_Reset;
            }
            else if (player1.button == 0x20 && holdPress2 == 2) {
               state = IncDec_Dec;
               holdPress2 = 0;
            }
            else if (player1.button == 0x20) {
               state = IncDec_WaitDec;
               holdPress2 = 2;
            }
            else {
                state = IncDec_Init;
            }          
            break;

        case (IncDec_Reset):
            if (player1.button == 0x40) {
                state = IncDec_Inc;
            }
            else if (player1.button == 0x20) {
                state = IncDec_Dec;
            }
            else {
                state = IncDec_Reset;
            }
            break;

        default:
            state = IncDec_SMStart;
            break;
        
    }

    switch (state){

        case (IncDec_SMStart): 
            player1.currPos = 1; 
            break;

        case (IncDec_Begin): 
            player1.currPos = 1;
            break;

        case (IncDec_Init): break;
        
        case (IncDec_Inc):
         if (player1.currPos < numMove - 1) { 
            player1.currPos = player1.currPos + 1; }
            break;

        case (IncDec_Dec):
         if (player1.currPos > 0) { 
            player1.currPos = player1.currPos - 1; }
            break;
        
        case (IncDec_WaitInc): break;
        case (IncDec_WaitDec): break;

        case (IncDec_Reset): 
            player1.currPos = 1;
            break;
    }

    return state;
}