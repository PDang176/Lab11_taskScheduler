/*	Author: Patrick Dang
 *  	Partner(s) Name: 
 *	Lab Section: 028
 *	Assignment: Lab #11  Exercise #1
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 *
 *	Video Link: https://drive.google.com/open?id=1AKq4VL2i6e4xOfZNx2SmFI2akfk4UmGa
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
unsigned char LED = 0x00;
//End Shared Variables

enum keypad_States { keypad_getKey };

int keypadSMTick(int state){
	//Local Variables
	unsigned char key;
	
	//Transitions
	switch(state){
		case keypad_getKey:
			state = keypad_getKey;
			break;
		default:
			state = keypad_getKey;
			break;
	}

	//State Actions
	switch(state){
		case keypad_getKey:
			key = GetKeypadKey();
			switch(key){
				case '\0': LED = 0x1F; break;
				case '1': LED = 0x01; break;
				case '2': LED = 0x02; break;
				case '3': LED = 0x03; break;
                                case '4': LED = 0x04; break;
				case '5': LED = 0x05; break;
                                case '6': LED = 0x06; break;
                                case '7': LED = 0x07; break;
                                case '8': LED = 0x08; break;
				case '9': LED = 0x09; break;
                                case 'A': LED = 0x0A; break;
                                case 'B': LED = 0x0B; break;
                                case 'C': LED = 0x0C; break;
                                case 'D': LED = 0x0D; break;
                                case '*': LED = 0x0E; break;
                                case '0': LED = 0x00; break;
                                case '#': LED = 0x0F; break;
				default: LED = 0x1B; break;
			}
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
			PORTB = LED;
			break;
		default:
			break;
	}
	return state;
}

int main(void) {
    	/* Insert DDR and PORT initializations */
	//DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xF0; PORTC = 0x0F;
	//DDRD = 0xFF; PORTD = 0x00;

	//Declare an array of tasks
	static task task1, task2;
	task *tasks[] = { &task1, &task2 };
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
