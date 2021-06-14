enum BallHitPlayer2_States { BallHitPlayer2_Check};

int BallHitPlayer2_SM( int state) {
    switch (state) { 
        case (BallHitPlayer2_Check): break;
        default: state = BallHitPlayer2_Check; break;
    }

    switch (state) {
        case (BallHitPlayer2_Check):
            if ( row_ball[ball.bPosRow] == 0x02 ) {           
                if ( moveRow[player2.currPos] == 0x1C && player2.ballHit == 0 ) {
                    if ( (col_ball[ball.bPosCol] == 0x1E ) || (col_ball[ball.bPosCol] == 0x1D) || (col_ball[ball.bPosCol] == 0x1B)  ) {
                        player2.ballHit = 1; //BALL WAS HIT
                    }
                    else {
                        player2.ballHit = 2;
                        
                    }
                }
                else if ( moveRow[player2.currPos] == 0x19 && player2.ballHit == 0 ) {
                    if ( (col_ball[ball.bPosCol] == 0x1E ) || (col_ball[ball.bPosCol] == 0x1D ) || (col_ball[ball.bPosCol] == 0x1B) || (col_ball[ball.bPosCol] == 0x17) ) {
                        player2.ballHit = 1;
                    }
                    else {
                        player2.ballHit = 2;
                        
                    }
                }
                else if ( moveRow[player2.currPos] == 0x13 && player2.ballHit == 0 ) {
                    if ( (col_ball[ball.bPosCol] == 0x1B ) || (col_ball[ball.bPosCol] == 0x17) || (col_ball[ball.bPosCol] == 0x0F) || (col_ball[ball.bPosCol] == 0x1D ) ) {
                        player2.ballHit = 1;
                    }
                    else {
                        player2.ballHit = 2;
                        
                    }
                }
                else if ( moveRow[player2.currPos] == 0x07 && player2.ballHit == 0 ) {
                    if ( (col_ball[ball.bPosCol] == 0x17 ) || (col_ball[ball.bPosCol] == 0x0F) || (col_ball[ball.bPosCol] == 0x1B) ) {
                        player2.ballHit = 1;
                    }
                    else {
                        player2.ballHit = 2;
                        
                    }
                }
            }
            else {
                player2.ballHit = 0; //NOT HIT
            }
            break;

        default: break; 
    }
    return state;    
} 

enum BallHit2_States { BallHit2_Check};

int BallHit2_SM( int state) {
    switch (state) { 
        case (BallHit2_Check): break;
        default: state = BallHit2_Check; break;
    }

    switch (state) {
        case (BallHit2_Check):
            if ( row_ball[ball.bPosRow] == 0x02 ) {           
                if ( moveRow[AI_player.currPos] == 0x1C && AI_player.ballHit == 0 ) {
                    if ( (col_ball[ball.bPosCol] == 0x1E ) || (col_ball[ball.bPosCol] == 0x1D) || (col_ball[ball.bPosCol] == 0x1B)  ) {
                        AI_player.ballHit = 1; //BALL WAS HIT
                    }
                    else {
                        AI_player.ballHit = 2;
                        
                    }
                }
                else if ( moveRow[AI_player.currPos] == 0x19 && AI_player.ballHit == 0 ) {
                    if ( (col_ball[ball.bPosCol] == 0x1E ) || (col_ball[ball.bPosCol] == 0x1D ) || (col_ball[ball.bPosCol] == 0x1B) || (col_ball[ball.bPosCol] == 0x17) ) {
                        AI_player.ballHit = 1;
                    }
                    else {
                        AI_player.ballHit = 2;
                        
                    }
                }
                else if ( moveRow[AI_player.currPos] == 0x13 && AI_player.ballHit == 0 ) {
                    if ( (col_ball[ball.bPosCol] == 0x1B ) || (col_ball[ball.bPosCol] == 0x17) || (col_ball[ball.bPosCol] == 0x0F) || (col_ball[ball.bPosCol] == 0x1D ) ) {
                        AI_player.ballHit = 1;
                    }
                    else {
                        AI_player.ballHit = 2;
                        
                    }
                }
                else if ( moveRow[AI_player.currPos] == 0x07 && AI_player.ballHit == 0 ) {
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
                        player1.ballHitX = 1; //BALL WAS HIT
                    }
                    else {
                        player1.ballHitX = 2;
                    }
                }
                else if ( moveRow[player1.currPos] == 0x19 && player1.ballHitX == 0 ) {
                    if ( (col_ball[ball.bPosCol] == 0x1E ) || (col_ball[ball.bPosCol] == 0x1D ) || (col_ball[ball.bPosCol] == 0x1B) || (col_ball[ball.bPosCol] == 0x17) ) {
                        player1.ballHitX = 1;
                    }
                    else {
                        player1.ballHitX = 2;
                    }
                }
                else if ( moveRow[player1.currPos] == 0x13 && player1.ballHitX == 0 ) {
                    if ( (col_ball[ball.bPosCol] == 0x1B ) || (col_ball[ball.bPosCol] == 0x17) || (col_ball[ball.bPosCol] == 0x0F) || (col_ball[ball.bPosCol] == 0x1D ) ) {
                        player1.ballHitX = 1;
                    }
                    else {
                        player1.ballHitX = 2;
                    }
                }
                else if ( moveRow[player1.currPos] == 0x07 && player1.ballHitX == 0 ) {
                    if ( (col_ball[ball.bPosCol] == 0x17 ) || (col_ball[ball.bPosCol] == 0x0F) || (col_ball[ball.bPosCol] == 0x1B) ) {
                        player1.ballHitX = 1;   
                    }
                    else {
                        player1.ballHitX = 2;
                    }
                }
            }
            else {
                player1.ballHitX = 0; //NOT HIT
            }
            break;

        default: break; 
    }

    return state;    
} 
