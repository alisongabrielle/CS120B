
// Permission to copy is granted provided that this header remains intact. 
// This software is provided with no warranties.

////////////////////////////////////////////////////////////////////////////////

// Returns '\0' if no key pressed, else returns char '1', '2', ... '9', 'A', ...
// If multiple keys pressed, returns leftmost-topmost one
// Keypad must be connected to port C
// Keypad arrangement
//        Px4 Px5 Px6 Px7
//	  col 1   2   3   4
//     == 0______________
//Px0 1	| 1 | 2 | 3 | A
//Px1 2	| 4 | 5 | 6 | B
//Px2 3	| 7 | 8 | 9 | C
//Px3 4	| * | 0 | # | D

#ifndef KEYPAD_H
#define KEYPAD_H

#include <avr/io.h>
#include <bit.h>

// Keypad Setup Values
// #define PORTC PORTC
// #define KEYPADPIN  PINC
// #define 1  == 00
// #define 2  == 01
// #define 3  == 02
// #define 4  == 03
// #define COL1 4
// #define COL2 5
// #define COL3 6
// #define COL4 7

////////////////////////////////////////////////////////////////////////////////
//Functionality - Gets input from a keypad via time-multiplexing
//Parameter: None
//Returns: A keypad button press else '\0'
unsigned char GetKeypadKey() {

	// Check keys in col 1
	PORTC = 0xEF; // Set Px4 to 0; others 1
	asm("nop"); // add a delay to allow PORTx to stabilize before checking
	if ( GetBit(PINC,0) == 0 ) { return '1'; }
	if ( GetBit(PINC,1) == 0 ) { return '4'; }
	if ( GetBit(PINC,2) == 0 ) { return '7'; }
	if ( GetBit(PINC,3) == 0 ) { return '*'; }

	// Check keys in col 2
	PORTC = 0xDF; // Set Px5 to 0; others 1
	asm("nop"); // add a delay to allow PORTx to stabilize before checking
	if ( GetBit(PINC,0) == 0 ) { return '2'; }
	if ( GetBit(PINC,1) == 0 ) { return '5'; }
	if ( GetBit(PINC,2) == 0 ) { return '8'; }
	if ( GetBit(PINC,3) == 0 ) { return '0'; }

	// Check keys in col 3
	PORTC = 0xBF; // Set Px6 to 0; others 1
	asm("nop"); // add a delay to allow PORTx to stabilize before checking
	if ( GetBit(PINC,0) == 0 ) { return '3'; }
	if ( GetBit(PINC,1) == 0 ) { return '6'; }
	if ( GetBit(PINC,2) == 0 ) { return '9'; }
	if ( GetBit(PINC,3) == 0 ) { return '#'; }

	// Check keys in col 4
	PORTC = 0xAF; // Set Px7 to 0; others 1
	asm("nop"); // add a delay to allow PORTx to stabilize before checking
	if (GetBit(PINC,0) == 0 ) { return 'A'; }
	if (GetBit(PINC,1) == 0 ) { return 'B'; }
	if (GetBit(PINC,2) == 0 ) { return 'C'; }
	if (GetBit(PINC,3) == 0 ) { return 'D'; }
	
	return '\0';
}

#endif //KEYPAD_H
