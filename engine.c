#include <eZ8.h>  						  // special encore constants, macros and flash functions
#include <sio.h>	                      // special encore serial i/o functions
#include "ansi.h"
#include "LEDdisplay.h"

typedef struct{
	char changedSinceLast;
	char whatIsThis;//player     - 0x01 
					//ball       - 0x02
					//breakable  - 0x03
					//solid      - 0.04
					//nothing    - 0x00
	char x1; //1. coordinate, placement
	char y1; //2. coordiante, placement
	struct vector direction; //ADD THIS STRUCT TO THE INCLUDED CODE PLS 
	//zones? - so far no zones
	char size;
	char color;
}entity;

void drawPlayer(entity object){

}

void drawBall(entity object){
	
}

void drawBreakable(entity object){
	
}

void drawSolid(entity object){
	
}

void drawMap(entity[] object){

}

