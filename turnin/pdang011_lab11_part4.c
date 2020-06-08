/*	Author: Patrick Dang
 *  	Partner(s) Name: 
 *	Lab Section: 028
 *	Assignment: Lab #11  Exercise #4
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * 	Video Link: https://drive.google.com/open?id=1AnKCUTvif8rt886aho80fZy0G1yKB_Ap 
 */
#include <avr/io.h>
#include "io.h"
#include "keypad.h"
#include "scheduler.h"
#include "timer.h"
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

//Shared Variables
unsigned char output = '\0';
unsigned char cursor = 1;
//End Shared Variables

enum keypad_States { keypad_WaitPress, keypad_WaitRelease };

int keypadSMTick(int state){
	//Local Variables
	unsigned char key;
	static unsigned char previousKey;

	//Transitions
	switch(state){
		case keypad_WaitPress:
			key = GetKeypadKey();
			if(key == '\0'){
				state = keypad_WaitPress;
			}
			else{
				state = keypad_WaitRelease;
				previousKey = key;
			}
			break;
		case keypad_WaitRelease:
			key = GetKeypadKey();
			if(key == '\0'){
				state = keypad_WaitPress;
				output = previousKey;
			}
			else{
				state = keypad_WaitRelease;
			}
			break;
		default:
			state = keypad_WaitPress;
			break;
	}

	//State Actions
	switch(state){
		case keypad_WaitPress:
			break;
		case keypad_WaitRelease:
			break;
		default:
			break;
	}
	return state;
}

enum display_States { display_display };

int displaySMTick(int state){
	//Transitions
	switch(state){
		case display_display:
			state = display_display;
			break;
		default:
			state = display_display;
			break;
	}
	
	//State Actions
	switch(state){
		case display_display:
			if(output != '\0'){
				LCD_WriteData(output);
				output = '\0';
				cursor = (cursor < 16) ? cursor + 1 : 1;
				LCD_Cursor(cursor);
			}
			break;
		default:
			break;
	}
}

int main(void) {
    	/* Insert DDR and PORT initializations */
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	DDRD = 0xFF; PORTD = 0x00;

	LCD_init();
	
	LCD_DisplayString(1, "Congratulations!");
	LCD_Cursor(1);

	//Declare an array of tasks
	static task task1, task2;
	task *tasks[] = { &task1, &task2};
	const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

	const char start = -1;

	//Task 1 (keypadSM)
	task1.state = start;
	task1.period = 10;
	task1.elapsedTime = task1.period;
	task1.TickFct = &keypadSMTick;

	//Task 2 (displaySM)
	task2.state = start;
	task2.period = 10;
	task2.elapsedTime = task2.period;
	task2.TickFct = &displaySMTick;

	unsigned short i; //for-loop iterator

	//Find GCD
	unsigned long GCD = tasks[0]->period;
	for(i = 0; i < numTasks; i++){
		GCD = findGCD(GCD, tasks[i]->period);
	}

	//Set the timer and turn it on
	TimerSet(GCD);
	TimerOn();

    	/* Insert your solution below */
      	while (1) {
		for(i = 0; i < numTasks; i++){
			if(tasks[i]->elapsedTime == tasks[i]->period){
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += GCD;
		}
		while(!TimerFlag);
		TimerFlag = 0;
    	}
    	return 0;
}
