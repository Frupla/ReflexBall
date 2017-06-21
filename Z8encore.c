#include <eZ8.h>  						  // special encore constants, macros and flash functions
#include <sio.h>	                      // special encore serial i/o functions
#include "ansi.h"
#include "charset.h"

#define LED_MAX_STR_LEN 20

int Msec = 0; 			// counts the time in 0.1ms, resets at 1000
int Timer = 0;            // Ensures that the LEDupdate() functions only works once every 0.1ms
char LED_Collumn = 0;    // Chooses the column being loaded into a display
char LED_Display = 0;    // Chooses the display to update next
char Displacement = 0;  // Keeps track of how many columns the LED's are offset. Increments when Msec is reset.
char Scrollon = 0;      // Indicates whether the text scroll is enabled or not.
char Letter = 0;        // Keeps track of how many letters the LED's are offset.
char Buffer[5][6] = {{    ' ', ' ', ' ', ' ', ' ', ' '}, //The buffer is used to store part of the string being read.
						 {' ',' ',' ',' ',' ',' '},
						 {' ',' ',' ',' ',' ',' '},
						 {' ',' ',' ',' ',' ',' '},
						 {' ',' ',' ',' ',' ',' '}};

char *LEDtext_p;        // Keeps track of the pointer pointing to the string being displayed.

char readKey(){
	char a,b,b1,b2,c=0x00;
	a = PDIN;
	b = PFIN;
	a =  (a & 0x08); // button PD3 on the board
	b1 = (b & 0x40); // button PF6 on the board
	b2 = (b & 0x80); // button PF7 on the board
	if(!b2){
		c |= 0x01; 
	}
	if(!b1){
		c |= 0x02; 
	}
	if(!a){
		c |= 0x04; 
	}
	return c;
}

#pragma interrupt
void timer0int(){
	Msec += 1;				 
}

/*int romstrLength(rom char* s){
	int i=0;
	while (s[i] != '\0' ){
 	    i++;
    }
	return i;
}*/

int strLength(char *s) {
	int i = 0;
	while (s[i] != '\0') {
		i++;
	}
	return i;
}

void timerSetup(){
 	T0CTL = 0x01;
	T0H = 0x1;
    T0L = 0x0;
	T0RH = 0x07;
    T0RL = 0x33;
	SET_VECTOR(TIMER0, timer0int);
	IRQ0ENH &= ~0x20;
	IRQ0ENL |= 0x20;
	T0CTL |= 0x80;			
	EI();
}

void LEDInit(){
	timerSetup();
	PEDD = 0x00;
	PGDD = 0x00;
	PEOUT = 0x1F;
	PGOUT = 0x00;
}

void updateBuffer(){
    int i, j, len = strLength(LEDtext_p);
    for (i = Letter; (i < len) && (i < 5 + Letter); i++) {
		for(j = 0; j < 5; j++){
			Buffer[i - Letter][j] = character_data[LEDtext_p[i] - 0x20][j];
		}
		Buffer[i - Letter][j] = 0x00;
	}
	for(i = i; i < 5 + Letter; i++){
		for(j = 0; j < 5; j++){
			Buffer[i - Letter][j] = character_data[0x00][j];
		}
		Buffer[i - Letter][j] = 0x00;
	}
}

void LEDSetString(char *src) {
	LEDtext_p = src;
	updateBuffer();
    if (Scrollon) {
        Letter++;
    }
}

void LEDUpdate(){
	if(Msec >= Timer){
		PEOUT &= ~(0x10>>LED_Collumn);
		PGOUT |= Buffer[LED_Display][LED_Collumn + Displacement];
		switch(LED_Display){
			case 0: PEOUT |= 0x80; break;
			case 1: PGOUT |= 0x80; break;
			case 2: PEOUT |= 0x20; break;
			case 3: PEOUT |= 0x40; break;
			default: PEOUT = 0x1F; PGOUT = 0x00; break;
		}
		LED_Display++;
		if(LED_Display > 3){
			LED_Collumn++;
			LED_Display=0;
		}
	  	if(LED_Collumn > 4){
			LED_Collumn = 0;
		}
		PEOUT = 0x1F; 
		PGOUT = 0x00;
		Timer++;
	}
	if(Msec >= 1000){
		Msec = 0;
		Timer = 0;
	   	if(Scrollon){
			Displacement++;
			if(Displacement > 6){
				updateBuffer();
				Letter++;
				Displacement = 1;
			}
		}
		if (Letter > strLength(LEDtext_p) - 1) {
			Letter = 0;
		}
	}
}

void LEDScrollon(){
	Scrollon = 1;
	Displacement = 0;
	Letter = 0;
}

void LEDScrolloff(){
	Scrollon = 0;
	Displacement = 0;
	Letter = 0;
}

int readMsec() {
    return Msec;
}
