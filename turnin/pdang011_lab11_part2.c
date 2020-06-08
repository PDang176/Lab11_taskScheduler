/*	Author: Patrick Dang
 *  	Partner(s) Name: 
 *	Lab Section: 028
 *	Assignment: Lab #11  Exercise #2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Video Link: https://drive.google.com/open?id=1AYCo93SWSaH1hIY9beqdjBULtzK5ThvJ
 */
#include <avr/io.h>
#include "io.h"
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0xFF; PORTA = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	TimerSet(300);
	TimerOn();

	LCD_init();

	const unsigned char* string = (unsigned char*)"CS120B is Legend... wait for it DARY!";
	
	const unsigned char* start = string;

    /* Insert your solution below */
    while (1) {
	LCD_ClearScreen();
	for(unsigned char c = 1, i = 0; c < 17; c++, i++){
		LCD_Cursor(c);
		LCD_WriteData(string[i]);
	}
	string = (*string == 'a') ? start : string + 1;
	while(!TimerFlag){}
	TimerFlag = 0;
    }
    return 1;
}
