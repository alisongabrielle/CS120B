#ifndef PLAYER_INFO_H
#define PLAYER_INFO_H

#include <avr/interrupt.h>

typedef struct _playerinfo {

    unsigned char column; //for PORTC
    unsigned char row; //for PORTD
    unsigned char currPos; // current position of player
    unsigned char button;
    unsigned char reset;
    unsigned char resetY;
    unsigned char bPosRow;
    unsigned char bPosCol; 
    unsigned char ballHitY;
    unsigned char ballHitX;
    
} playerinfo;



#endif //PLAYER_INFO_H
