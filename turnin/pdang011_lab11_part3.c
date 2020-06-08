/*	Author: Patrick Dang
 *  	Partner(s) Name: 
 *	Lab Section: 028
 *	Assignment: Lab #11  Exercise #3
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 * 	Video Link: 
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
unsigned char output = ' ';
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

int main(void) {
    	/* Insert DDR and PORT initializations */
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	DDRD = 0xFF; PORTD = 0x00;

	LCD_init();

	//Declare an array of tasks
	static task task1;
	task *tasks[] = { &task1};
	const unsigned short numTasks = sizeof(tasks) / sizeof(task*);

	const char start = -1;

	//Task 1 (keypadSM)
	task1.state = start;
	task1.period = 10;
	task1.elapsedTime = task1.period;
	task1.TickFct = &keypadSMTick;

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
		LCD_ClearScreen();
		LCD_WriteData(output);
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
