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
unsigned char start_button = 0x00;
unsigned char notHitStart = 0;

playerinfo player1;
playerinfo AI_player;
playerinfo ball;

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
            state = (start == 1) ? AIplay_Up: AIplay_Wait; break;
        
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


//User Control: Move Up or Down
enum IncDec_States { IncDec_SMStart, IncDec_Begin, IncDec_Init, IncDec_Inc, IncDec_Dec, IncDec_WaitInc, IncDec_WaitDec, IncDec_Reset };

int TickFct_IncDec(int state) {
   
    player1.button = ~PINB & 0x03; //since we're playing with an AI
   
   switch (state) {
        case (IncDec_SMStart): state = IncDec_Begin; break;
         
       case (IncDec_Begin):
            if (player1.button == 0x01) {
                state = IncDec_Inc;
            }
            else if (player1.button == 0x02) {
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
            else if (player1.button == 0x01) {
                state = IncDec_Inc;
            }
            else if (player1.button == 0x02) {
                state = IncDec_Dec;
            }
            break;

        case (IncDec_Inc):
            if (player1.button == 0x03){
                state = IncDec_Reset;
            }
            else if (player1.button == 0x02) {
                state = IncDec_Dec;
            }
            else if (player1.button == 0x01) {
               state = IncDec_WaitInc;  
            }
            else {
                state = IncDec_Init;
            }
            break;

        case (IncDec_WaitInc):
            if (player1.button == 0x02) {
                state = IncDec_Dec;
            }
            else if (player1.button == 0x03) {
                state = IncDec_Reset;
            }
            else if (player1.button == 0x01 && holdPress1 == 1) {
               state = IncDec_Inc;
               holdPress1 = 0;
            }
            else if (player1.button == 0x01) {
                holdPress1 = 1;
                state = IncDec_WaitInc;
            }
            else {
               state = IncDec_Init;
            }
            break;
        
        case (IncDec_Dec):
            if (player1.button == 0x03){
                state = IncDec_Reset;
            }
            else if (player1.button == 0x01) {
                state = IncDec_Inc;
            }
            else if (player1.button == 0x02) {
               state = IncDec_WaitDec;  
            }
            else {
                state = IncDec_Init;
            }
            break;

        case (IncDec_WaitDec):
            if (player1.button == 0x01) {
                state = IncDec_Inc;
            }
            else if (player1.button == 0x03) {
                state = IncDec_Reset;
            }
            else if (player1.button == 0x02 && holdPress2 == 2) {
               state = IncDec_Dec;
               holdPress2 = 0;
            }
            else if (player1.button == 0x02) {
               state = IncDec_WaitDec;
               holdPress2 = 2;
            }
            else {
                state = IncDec_Init;
            }          
            break;

        case (IncDec_Reset):
            if (player1.button == 0x01) {
                state = IncDec_Inc;
            }
            else if (player1.button == 0x02) {
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

enum Start_States {Start_Wait, Start_On, Start_Off};

int Start_SM (int state) {
    
    start_button = ~PINB & 0x04;
    
    switch (state) {
        
        case (Start_Wait):
            state = (start_button == 0x04) ? Start_On : Start_Wait; break;

        case (Start_On):
            state = (start_button == 0x00) ? Start_On : Start_Off; break;

        case (Start_Off):
            state = (start_button == 0x04) ? Start_On : Start_Off; break;

        default: state = Start_Wait; break;
    }

    switch (state) {

        case (Start_Wait): start = 0; break;
        case (Start_On): start = 1; break;
        case (Start_Off): start = 0; notHitStart = 0; break;
        default: break;
    }

    return state;

}

//Display Output
enum Display_States { Display_Output};
int Display_Tick(int state) {
	
   column[0] = moveRow[player1.currPos];
   column[1] = col_ball[ball.bPosCol];
   column[2] = moveRow[AI_player.currPos];
   row[0] = 0x80;
   row[1] = row_ball[ball.bPosRow];
   row[2] = 0x01;

   switch(state) {
      case Display_Output: break;     

      default: state = Display_Output; break;
   }

   switch (state) {
        case Display_Output:
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

   PORTA = debug | debug2;
   return state;
}

int main(void) {

   DDRC = 0xFF; PORTC = 0x00;
   DDRD = 0xFF; PORTD = 0x00;
   DDRB = 0x00; PORTB = 0xFF;
   DDRA = 0xFF; PORTA = 0x00;

   static task task1, task2, task3, task4, task5, task6, task7, task8;
   task *tasks[] = { &task1, &task2, &task3, &task4, &task5, &task6, &task7, &task8};

   const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

   const char start = -1;

   task1.state = start;
   task1.period = 1;
   task1.elapsedTime = task1.period;
   task1.TickFct = &Display_Tick;

   task2.state = start;
   task2.period = 100; //100ms for player1.button input
   task2.elapsedTime = task2.period;
   task2.TickFct = &TickFct_IncDec;

   task3.state = start;
   task3.period = 200; //100ms for player1.button input
   task3.elapsedTime = task3.period;
   task3.TickFct = &AIPlayer_Move;

   task4.state = start;
   task4.period = 300; //100ms for player1.button input
   task4.elapsedTime = task4.period;
   task4.TickFct = &Ball_Tick_X_Axis;

   task5.state = start;
   task5.period = 300; //100ms for player1.button input
   task5.elapsedTime = task5.period;
   task5.TickFct = &Ball_Tick_Y_Axis;

   task6.state = start;
   task6.period = 300; //100ms for player1.button input
   task6.elapsedTime = task6.period;
   task6.TickFct = &BallHit1_SM;

   task7.state = start;
   task7.period = 300; //100ms for player1.button input
   task7.elapsedTime = task7.period;
   task7.TickFct = &BallHit2_SM;

   task8.state = start;
   task8.period = 100; //100ms for player1.button input
   task8.elapsedTime = task8.period;
   task8.TickFct = &Start_SM;

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











