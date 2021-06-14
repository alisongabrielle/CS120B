/*	Author: Alison Gonzales
 *  Partner(s) Name: Alison Gonzales
 *	Lab Section: 023
 *	Assignment: Lab # 11  Exercise # 1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *  Ball Link: 
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

#include "keypad.h"
#include "scheduler.h"
#include "timer.h"
#include "player_info.h"

//-----------------global_variables------------------------------
const int numOutputs = 4;
const int numMove = 4;
const int numRow = 8;
const int numCol = 5;

//indexes
short int j, k = 0; //j = columns, k = rows

//arrays
int column[] = { 0x00, 0x00, 0x00 }; //use for Display_Tick //[0] = player 1, [4] = player2 
int row[] = { 0x00, 0x00, 0x00}; //change in ball movement

unsigned char moveRow[] = {0x1C, 0x19, 0x13, 0x07}; //positions for players
unsigned char row_ball[] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0xFF}; //X AXIS
unsigned char col_ball[] = {0x1E, 0x1D, 0x1B, 0x17, 0x0F, 0x00}; //Y AXIS

short int holdPress1 = 0;
short int holdPress2 = 0;

unsigned char start = 0;
unsigned char start_button = 0;
unsigned char notHitStart = 0;
unsigned char player_mode = 0;

playerinfo player1;
playerinfo AI_player;
playerinfo ball;
playerinfo player2;

//period
short int startPeriod = 0;
unsigned char debug = 0;
unsigned char debug2 = 0;
unsigned char forward = 0;
//-----------------end_of_variables-------------------------------  

enum BallHit2_States { BallHit2_Check};

int BallHit2_SM( int state) {
    switch (state) { 
        case (BallHit2_Check): break;
        default: state = BallHit2_Check; break;
    }

    switch (state) {
        case (BallHit2_Check):
            if ( row_ball[ball.bPosRow] == 0x02 ) {           
                if ( moveRow[AI_player.currPos] == 0x1C ) {
                    if ( (col_ball[ball.bPosCol] == 0x1E ) || (col_ball[ball.bPosCol] == 0x1D) || (col_ball[ball.bPosCol] == 0x1B)  ) {
                        AI_player.ballHit = 1; //BALL WAS HIT
                    }
                    else {
                        AI_player.ballHit = 2;
                        
                    }
                }
                else if ( moveRow[AI_player.currPos] == 0x19 ) {
                    if ( (col_ball[ball.bPosCol] == 0x1E ) || (col_ball[ball.bPosCol] == 0x1D ) || (col_ball[ball.bPosCol] == 0x1B) || (col_ball[ball.bPosCol] == 0x17) ) {
                        AI_player.ballHit = 1;
                    }
                    else {
                        AI_player.ballHit = 2;
                        
                    }
                }
                else if ( moveRow[AI_player.currPos] == 0x13 ) {
                    if ( (col_ball[ball.bPosCol] == 0x1B ) || (col_ball[ball.bPosCol] == 0x17) || (col_ball[ball.bPosCol] == 0x0F) || (col_ball[ball.bPosCol] == 0x1D ) ) {
                        AI_player.ballHit = 1;
                    }
                    else {
                        AI_player.ballHit = 2;
                        
                    }
                }
                else if ( moveRow[AI_player.currPos] == 0x07 ) {
                    if ( (col_ball[ball.bPosCol] == 0x17 ) || (col_ball[ball.bPosCol] == 0x0F) || (col_ball[ball.bPosCol] == 0x1B) ) {
                        AI_player.ballHit = 1;
                    }
                    else {
                        AI_player.ballHit = 2;
                        
                    }
                }
            }
            else {
                AI_player.ballHit = 0; //NOT HIT
                
            }
            break;

        default: break; 
    }
    return state;    
} 

enum BallHit1_States { BallHit1_Check};

int BallHit1_SM( int state) {

    switch (state) { 
        
        case (BallHit1_Check): break;

        default: state = BallHit1_Check; break;
    }

    switch (state) {

        case (BallHit1_Check):
            if ( row_ball[ball.bPosRow] == 0x40 ) {           
                if ( moveRow[player1.currPos] == 0x1C ) {
                    if ( (col_ball[ball.bPosCol] == 0x1E ) || (col_ball[ball.bPosCol] == 0x1D) || (col_ball[ball.bPosCol] == 0x1B) ) {
                        player1.ballHit = 1; //BALL WAS HIT
                    }
                    else {
                        player1.ballHit = 2;
                    }
                }
                else if ( moveRow[player1.currPos] == 0x19 ) {
                    if ( (col_ball[ball.bPosCol] == 0x1E ) || (col_ball[ball.bPosCol] == 0x1D ) || (col_ball[ball.bPosCol] == 0x1B) || (col_ball[ball.bPosCol] == 0x17) ) {
                        player1.ballHit = 1;
                    }
                    else {
                        player1.ballHit = 2;
                    }
                }
                else if ( moveRow[player1.currPos] == 0x13 ) {
                    if ( (col_ball[ball.bPosCol] == 0x1B ) || (col_ball[ball.bPosCol] == 0x17) || (col_ball[ball.bPosCol] == 0x0F) || (col_ball[ball.bPosCol] == 0x1D ) ) {
                        player1.ballHit = 1;
                    }
                    else {
                        player1.ballHit = 2;
                    }
                }
                else if ( moveRow[player1.currPos] == 0x07 ) {
                    if ( (col_ball[ball.bPosCol] == 0x17 ) || (col_ball[ball.bPosCol] == 0x0F) || (col_ball[ball.bPosCol] == 0x1B) ) {
                        player1.ballHit = 1;   
                    }
                    else {
                        player1.ballHit = 2;
                    }
                }
            }
            else {
                player1.ballHit = 0; //NOT HIT
            }
            break;

        default: break; 
    }

    return state;    
} 

enum Ball_Y_States {Ball_Y_Wait, Ball_Y_shift, Ball_Y_shiftBack, Ball_Y_NotHit};
int Ball_Tick_Y_Axis(int state) {
	
	switch (state) {
		case Ball_Y_Wait:
            state = (start == 1) ? Ball_Y_shift : Ball_Y_Wait;
            break;

        case Ball_Y_shift:
            if (AI_player.ballHit == 2 || player1.ballHit == 2) {
                debug = 0x01;
                state = Ball_Y_NotHit;
                
            }
            else if (ball.resetY == 1) {
                debug = 0x02;
                state = Ball_Y_shiftBack;
                
            }
            else {
                state = Ball_Y_shift;
                
            }
        break;

        case Ball_Y_shiftBack:
            if (AI_player.ballHit == 2 || player1.ballHit == 2) {
                debug = 0x04;
                state = Ball_Y_NotHit;
                
            }
            else if (ball.resetY == 2) {
                debug = 0x08;
                state = Ball_Y_shift;
                
            }
            else {
                state = Ball_Y_shiftBack;
                
            }
            break;
        
        case Ball_Y_NotHit: 
        
         state = Ball_Y_Wait; break;

		default: state = Ball_Y_Wait; break;
	}	
	switch (state) {
        
        case Ball_Y_Wait: 
            ball.bPosCol = 2; //initial ball position;
        break;

        case Ball_Y_shift:	
            //moving to AI_Player/ Player 2
            if (ball.bPosCol < numCol - 1 ) {
                ball.bPosCol++;
            }
            else {
                ball.resetY = 1;
                ball.bPosCol = 4;
            }
            
        break;

        case Ball_Y_shiftBack:	
            //moving to AI_Player/ Player 2
            if (ball.bPosCol > 0) {
                ball.bPosCol--;
            }
            else {
                ball.resetY = 2;
                ball.bPosCol = 0;
            }
            
        break;

        case Ball_Y_NotHit:             
            ball.bPosCol = 5;  break;

        default: break;
        }

	return state;
}



enum Ball_X_States {Ball_X_Wait, Ball_X_shift, Ball_X_HitAI, Ball_X_shiftBack, Ball_X_HitPl1, Ball_X_NotHit};
int Ball_Tick_X_Axis(int state) {

	switch (state) {
		case Ball_X_Wait:
            state = (start == 1 && notHitStart == 0) ? Ball_X_shift : Ball_X_Wait; //wait for starting button
            break;
        
        case Ball_X_shift: 
            if (start == 0) {
                state = Ball_X_Wait;
            }
            else if (AI_player.ballHit == 1) {
                debug2 = 0x10;
                ball.reset = 1; //AI paddle hits the ball, ball goes back to player1
                state = Ball_X_HitAI; 
            }
            else if ( AI_player.ballHit == 2 ) { //AI_player miss ball
                debug2 = 0x20;
                state = Ball_X_NotHit;
            }
            else {
                state = Ball_X_shift; //continue ball movement across the net
            }
        break;  

        case Ball_X_HitAI:
            state = (ball.reset == 1) ? Ball_X_shiftBack : Ball_X_HitAI; break;

        case Ball_X_shiftBack:
             if (start == 0) {
                state = Ball_X_Wait;
            }
            else if (player1.ballHit == 1) { 
                debug2 = 0x40;
                ball.reset = 2; //player 1 hits the ball, ball go to AI Player
                state = Ball_X_HitPl1; 
            }
            else if (player1.ballHit == 2 ) {
                debug2 = 0x80;
                state = Ball_X_NotHit;
            }
            else {
                state = Ball_X_shiftBack; //continue ball movement across the net
            }
            break;
        
        case Ball_X_HitPl1:
            state = (ball.reset == 2) ? Ball_X_shift : Ball_X_HitPl1; break;
        
        case Ball_X_NotHit:
            state = Ball_X_Wait; break;

		default: state = Ball_X_Wait; break;
	}	
	switch (state) {
        
        case Ball_X_Wait: 
            ball.bPosRow = 3; //initial ball position;
        break;

        case Ball_X_HitAI: break;

        case Ball_X_shift:	
            //moving to AI_Player/ Player 2
            if (ball.bPosRow < numRow - 1) {
                ball.bPosRow++;
            }
            else {
                ball.bPosRow = 7;
            }
        break;

        case Ball_X_HitPl1: break;

        case Ball_X_shiftBack:	
            //moving to AI_Player/ Player 2
            if (ball.bPosRow > 0) {
                ball.bPosRow--;
            }
            else {
                ball.bPosRow = 0;
            }
        break;

        case Ball_X_NotHit: 
            notHitStart = 1;
            ball.bPosRow = 8;  
            break;

        default: break;
        }

	return state;
}

//AI_Player: Move Up or Down Automatic 
enum AIplay_States {AIplay_Wait, AIplay_Up, AIplay_Down};

int AIPlayer_Move(int state) {

    switch (state) {
        case (AIplay_Wait):
            state = (start == 1 && player_mode == 1 ) ? AIplay_Up: AIplay_Wait; break;
        
        case (AIplay_Up): 
            if (start == 0) {
                state = AIplay_Wait;
            }
            else if (AI_player.reset == 1) {
                state = AIplay_Down;
            }
            else {
                state = AIplay_Up;
            }
            break;


        case (AIplay_Down):
            if (start == 0) {
                state = AIplay_Wait;
            }
            else if (AI_player.reset == 2) {
                state = AIplay_Up;
            }
            else {
                state = AIplay_Down;
            }
            break;

        default: state = AIplay_Wait; break;
    }

    switch (state) {
        case (AIplay_Wait): 
            AI_player.currPos = 2;
            break;

        case (AIplay_Up): 
            if (AI_player.currPos < numOutputs - 1) {
                AI_player.currPos++;
            }
            else {
                AI_player.reset = 1;
            }
        break;

        case (AIplay_Down): 
            if (AI_player.currPos > 0) {
                AI_player.currPos--;
            }
            else {
                AI_player.reset = 2;
            }
        break;

        default: break;
    }
    return state;
}


enum Player2_States { Player2_SMStart, Player2_Begin, Player2_Init, Player2_Inc, Player2_Dec, Player2_WaitInc, Player2_WaitDec, Player2_Reset };

int Player2_SM (int state) {
   
   player2.button = GetKeypadKey();
    
   switch (state) {
        case (Player2_SMStart): 
        state = (start == 1 && player_mode == 2) ? Player2_Begin : Player2_SMStart; break;
         
        case (Player2_Begin):
            if (player2.button == 'A') {
                state = Player2_Inc;
            }
            else if (player2.button == '1') {
                state = Player2_Dec;
            }
            else {
                state = Player2_Init;
            }
            break;

        case (Player2_Init):
            if (player2.button == '\0') {
                state = Player2_Init;
            }
            else if (player2.button == 'A') {
                state = Player2_Inc;
            }
            else if (player2.button == '1') {
                state = Player2_Dec;
            }
            break;

        case (Player2_Inc):
            if (player2.button == '#'){
                state = Player2_Reset;
            }
            else if (player2.button == '1') {
                state = Player2_Dec;
            }
            else if (player2.button == 'A') {
               state = Player2_WaitInc;  
            }
            else {
                state = Player2_Init;
            }
            break;

        case (Player2_WaitInc):
            if (player2.button == '1') {
                state = Player2_Dec;
            }
            else if (player2.button == '#') {
                state = Player2_Reset;
            }
            else if (player2.button == 'A' && holdPress1 == 1) {
               state = Player2_Inc;
               holdPress1 = 0;
            }
            else if (player2.button == 'A') {
                holdPress1 = 1;
                state = Player2_WaitInc;
            }
            else {
               state = Player2_Init;
            }
            break;
        
        case (Player2_Dec):
            if (player2.button == '#'){
                state = Player2_Reset;
            }
            else if (player2.button == 'A') {
                state = Player2_Inc;
            }
            else if (player2.button == '1') {
               state = Player2_WaitDec;  
            }
            else {
                state = Player2_Init;
            }
            break;

        case (Player2_WaitDec):
            if (player2.button == 'A') {
                state = Player2_Inc;
            }
            else if (player2.button == '#') {
                state = Player2_Reset;
            }
            else if (player2.button == '1' && holdPress2 == 2) {
               state = Player2_Dec;
               holdPress2 = 0;
            }
            else if (player2.button == '1') {
               state = Player2_WaitDec;
               holdPress2 = 2;
            }
            else {
                state = Player2_Init;
            }          
            break;

        case (Player2_Reset):
            if (player2.button == 'A') {
                state = Player2_Inc;
            }
            else if (player2.button == '1') {
                state = Player2_Dec;
            }
            else {
                state = Player2_Reset;
            }
            break;

        default:
            state = Player2_SMStart;
            break;
        
    }

    switch (state){

        case (Player2_SMStart): 
            AI_player.currPos = 4; 
            break;

        case (Player2_Begin): 
            AI_player.currPos = 1;
            break;

        case (Player2_Init): break;
        
        case (Player2_Inc):
         if (AI_player.currPos < numMove - 1) { 
            AI_player.currPos = AI_player.currPos + 1; }
            break;

        case (Player2_Dec):
         if (AI_player.currPos > 0) { 
            AI_player.currPos = AI_player.currPos - 1; }
            break;
        
        case (Player2_WaitInc): break;
        case (Player2_WaitDec): break;

        case (Player2_Reset): 
            AI_player.currPos = 1;
            break;
    }

    return state;
}

//User Control: Move Up or Down
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

//Start SM
enum Start_States {Start_Choose, Start_Player1, Start_Player2, Start_Reset};

int Start_SM (int state) {
    
    start_button = GetKeypadKey();
    
    switch (state) {
        
        case (Start_Choose):
            if (start_button == '4') {
                state = Start_Player1;
            }
            else if (start_button == '6') {
                state = Start_Player2;
            }
            else {
                state = Start_Choose;
            }
            break;

        case Start_Player1: 
            state = (start_button == '*') ? Start_Reset : Start_Player1; break;

        case Start_Player2: 
            state = (start_button == '*') ? Start_Reset : Start_Player2; break;
        
        case Start_Reset:
            if (start_button == '4') {
                state = Start_Player1;
            }
            else if (start_button == '6') {
                state = Start_Player2;
            }
            else {
                state = Start_Reset;
            }
            break;

        default: state = Start_Choose; break;
    }

    switch (state) {

        case Start_Choose:
	    player_mode = 0; 
            start = 0;
            break;
        
        case Start_Player1:
	    player_mode = 1;
            start = 1;
            break; 
        
        case Start_Player2:
            player_mode = 2; 
            start = 1;
            break;

        case Start_Reset: 
            player_mode = 0; 
            start = 0;
            notHitStart = 1;
            break;
    }
    return state;
}

enum Display_States { Display_Wait, Display_Play, Display_Reset};
int Display_Tick(int state) {
	
   switch(state) {
      case Display_Wait: 
        if ( ( player_mode == 1) || ( player_mode == 2 ) ) {
            state = Display_Play;
        }
        else {
            state = Display_Wait;
        }
        break;

    case Display_Play:
        state = (player_mode == 0) ? Display_Reset : Display_Play;
        break;

    case Display_Reset:
        if ( ( player_mode == 1) || ( player_mode == 2 ) ) {
            state = Display_Play;
        }
        else {
            state = Display_Wait;
        }
        break;     

      default: state = Display_Wait; break;
   }

   switch (state) {
        case Display_Wait:
            column[0] = 0x1D;
            column[1] = 0x00;
            column[2] = 0x13;
            row[0] = 0x20;
            row[1] = 0x00;
            row[2] = 0x04;
        break;

        case Display_Play:
            column[0] = moveRow[player1.currPos];
            column[1] = col_ball[ball.bPosCol];
            column[2] = moveRow[AI_player.currPos];
            row[0] = 0x80;
            row[1] = row_ball[ball.bPosRow];
            row[2] = 0x01;
        break;

        case Display_Reset:
        break;

      default: break;
   }

   return state;
}

//Just display the actual outputs
enum Print_States { Print_Output};
int Print_Tick(int state) {
    
   switch(state) {
      case Print_Output: break;     

      default: state = Print_Output; break;
   }

   switch (state) {
        case Print_Output:
        if (j < 3) {
            PORTD = column[j];  //Y AXIS         
	        PORTC = row[j];	//X AXIS	    
            j++;
        }
        else {
            j = 0;
        }
        break;

      default: break;
   }

   return state;
}


int main(void) {

   DDRC = 0xFF; PORTC = 0x00;
   DDRD = 0xFF; PORTD = 0x00;
   DDRB = 0x0F; PORTB = 0xF0;
   DDRA = 0xF0; PORTA = 0x0F;

    static task task1, task2, task3, task4, task5, task6, task7, task8, task9, task10;
   task *tasks[] = { &task1, &task2, &task3, &task4, &task5, &task6, &task7, &task8, &task9, &task10};

   const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

   const char start = -1;

   task1.state = start;
   task1.period = 100;
   task1.elapsedTime = task1.period;
   task1.TickFct = &Start_SM;

   task2.state = start;
   task2.period = 1; 
   task2.elapsedTime = task2.period;
   task2.TickFct = &Display_Tick;

   task3.state = start;
   task3.period = 1; 
   task3.elapsedTime = task3.period;
   task3.TickFct = &Print_Tick;

   task4.state = start;
   task4.period = 300; 
   task4.elapsedTime = task4.period;
   task4.TickFct = &Ball_Tick_X_Axis;

   task5.state = start;
   task5.period = 300; 
   task5.elapsedTime = task5.period;
   task5.TickFct = &Ball_Tick_Y_Axis;

   task6.state = start;
   task6.period = 300; 
   task6.elapsedTime = task6.period;
   task6.TickFct = &BallHit1_SM;

   task7.state = start;
   task7.period = 300; 
   task7.elapsedTime = task7.period;
   task7.TickFct = &BallHit2_SM;

   task8.state = start;
   task8.period = 100; 
   task8.elapsedTime = task8.period;
   task8.TickFct = &TickFct_IncDec;

   task9.state = start;
   task9.period = 100; 
   task9.elapsedTime = task9.period;
   task9.TickFct = &Player2_SM;

   task10.state = start;
   task10.period = 100; 
   task10.elapsedTime = task10.period;
   task10.TickFct = &AIPlayer_Move;

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











