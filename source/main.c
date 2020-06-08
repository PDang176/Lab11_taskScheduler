/*	Author: Patrick Dang
 *  	Partner(s) Name: 
 *	Lab Section: 028
 *	Assignment: Lab #11  Exercise #5
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
unsigned char output = '\0';
unsigned char cursor = 1;
//End Shared Variables

enum movement_States {};

int movementSMTick(int state){

}

enum randomSpawn_States {};

int randomSpawnSMTick(int state){

}

enum display_States {};

int displaySMTick(int state){

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

	//Task 1 (movementSM)
	task1.state = start;
	task1.period = 10;
	task1.elapsedTime = task1.period;
	task1.TickFct = &movementSMTick;

	//Task 2 (randomSpawnSM)
	task2.state = start;
	task2.period = 10;
	task2.elapsedTime = task2.period;
	task2.TickFct = &randomSpawnSMTick;

	//Task 3 (displaySM)
	task3.state = start;
	task3.period = 100;
	task3.elapsedTime = task3.period;
	task3.TickFct = &displaySMTick;

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
