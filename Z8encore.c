#include <eZ8.h>  						  // special encore constants, macros and flash functions
#include <sio.h>	                      // special encore serial i/o functions
#include "ansi.h"
#include "charset.h"

#define LED_MAX_STR_LEN 100

int Msec = 0; 			// counts the time in 0.1ms, resets at 1000
int Timer = 0;			// ensures that the LEDupdate() functions only works once every 0.1ms
char LED_Collumn = 0; 	// choses the collumn being loaded into a display
char LED_Display = 0;	// choses the display
char Displacement = 0;  // increments when Msec is reset, 
char Scrollon = 0; 
char Letter = 0;
char Buffer[5][6] = 	{{' ',' ',' ',' ',' ',' '},
						 {' ',' ',' ',' ',' ',' '},
						 {' ',' ',' ',' ',' ',' '},
						 {' ',' ',' ',' ',' ',' '},
						 {' ',' ',' ',' ',' ',' '}};
char *LEDtext_p;

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

int romstrLength(rom char* s){
	int i=0;
	while(s[i]!='\0'){
 	i++;}
	return i;
}

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
	int i, j, romln = strLength(LEDtext_p);
	for(i = Letter; (i < romln) && (i < 5 + Letter); i++){
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
	Letter++;
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

void LEDScroll(){
	Scrollon = !Scrollon;
}

int readMsec() {
    return Msec;
}
