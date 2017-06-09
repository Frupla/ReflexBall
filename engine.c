#include <eZ8.h>  						  // special encore constants, macros and flash functions
#include <sio.h>	                      // special encore serial i/o functions
#include "ansi.h"
#include "math.h"
#include "Z8encore.h"

typedef struct{
	char changedSinceLast;
	char whatIsThis;//player     - 0x01 
					//ball       - 0x02
					//breakable  - 0x03
					//solid      - 0x04
					//nothing    - 0x00
	char x1; //1. coordinate, placement
	char y1; //2. coordiante, placement
	struct TVector direction; //ADD THIS STRUCT TO THE INCLUDED CODE PLS 
	//zones? - so far no zones
	char size; // first 4 bits represent the vertical size factor (y), last 4 represent the horizontal size factor (x)
	char color;
} entity;

void drawPlayer(entity object){
	int i;
	for(i = 0; i <= (object.size & 0x0F); i++){
		gotoxy(object.x1 + i,object.y1)
		printf(219);	
	}
	object.changedSinceLast = 0;
}

void drawBall(entity object){
	int i, j;
	gotoxy(object.x1,object.y1);
	printf(184);	
	object.changedSinceLast = 0;	
}

void drawBreakable(entity object){
	int i, j;
	for(i = 0; i <= (object.size & 0x0F); i++){
		for(j = 0; j <= (object.size & 0xF0); j++){
			gotoxy(object.x1 + i,object.y1)
			printf(177);	
		}
	object.changedSinceLast = 0;
	}
}

void drawSolid(entity object){
	int i, j;
	for(i = 0; i <= (object.size & 0x0F); i++){
		for(j = 0; j <= (object.size & 0xF0); j++){
			gotoxy(object.x1 + i,object.y1)
			printf(72);	
		}
	}
	object.changedSinceLast = 0;
}

void drawMap(entity[][] object){
	int i = 0, j = 0;
	while(object[i].whatIsThis){
		while(object[i][j].whatIsThis){
			if (object[i][j].changedSinceLast){
				switch(object[i][j].whatIsThis){
					case 0x01: drawPlayer(object[i][j]); break;
					case 0x02: drawBall(object[i][j]); break; 
					case 0x03: drawBreakable(object[i][j]); break; 
					case 0x04: drawSolid(object[i][j]); break;
					default: break;
				}
				j++;
			}
		}
		j = 0;
		i++;
	}
}

void playerMovement(char buttonPress, entity* object){
	switch(buttonPress){
		case 0x01: 
	}
}

void collisionCheck(entity[][] object){
	
}

