#include <eZ8.h>  						  // special encore constants, macros and flash functions
#include <sio.h>	                      // special encore serial i/o functions
#include "ansi.h"
#include "math.h"
#include "Z8encore.h"

#define MAPSIZE 400

typedef struct{
	char changedSinceLast;
	char whatIsThis;//player     - 0x01 
					//ball       - 0x02
					//breakable  - 0x03
					//solid      - 0x04
					//nothing    - 0x00
	int x1; //1. coordinate, placement
	int y1; //2. coordiante, placement
	TVector direction; // Speed and direction (only relevant for the ball)
	//zones? - so far no zones
	char sizeX; // represent the horizontal size factor
    char sizeY;	// represent the vertical size factor
	char color;
} entity;

void drawPlayer(entity object){
	int i;
	for(i = 0; i <= (object.sizeX); i++){
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
	for(i = 0; i <= (object.sizeX); i++){
		for(j = 0; j <= (object.sizeY); j++){
			gotoxy(object.x1 + i,object.y1)
			printf(177);	
		}
	}
	object.changedSinceLast = 0;
}

void drawSolid(entity object){
	int i, j;
	for(i = 0; i <= (object.sizeX); i++){
		for(j = 0; j <= (object.sizeY); j++){
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
	collisionCheck();
	for(i = 0; i <= (object->sizeX); i++){	// Deletes the old drawing of the paddle
		gotoxy(object->x1 + i,object->y1)
		printf(20);	
	}
	switch(buttonPress) {
		case 0x01: //PF7
			object->x1++;
			object->changedSinceLast = 1;
			break;
		case 0x02: //PF6
			break;
		case 0x04: //PD3
			object->x1--;
			object->changedSinceLast = 1;
			break;
		case 0x05: // when PD3 and PF7 is pressed at the same time, nothing happens
			break;
		default:
			break;
	}
}

void ballMovement(entity *object) {
	char flag = collisionCheck()
	switch(flag){
		case 0x00:	break;
		case 0x01:	
		case 0x02:
		case 0x04:
		case 0x08:
		case 0x10:
		default: break;
	}
}

char collisionCheck(entity[] breakables, entity[] solids, entity ball) { // an array of breakables, solids and a ball
	char flag = 0;
	char didithit = 0;
	if(((ball.x1 + (char)ball.direction.x) >= breakables.x1) && ((ball.x1 + (char)ball.direction.x) <= (breakables.x1 + breakables.sizeX))){// Tvectors are made with longs, maybe they shouldn't be
		if(((ball.y1 + (char)ball.direction.y) >= breakables.y1) && ((ball.y1 + (char)ball.direction.y) <= (breakables.y1 + breakables.sizeY))){
			didithit = 1;
		}
	}
	if(didithit){
		switch()
	}
	return flag;
	/* flag encoding
	 * 0x00 = no collision
	 * 0x01 = object hit left side
	 * 0x02 = object hit bottom side
	 * 0x04 = object hit right side
	 * 0x08 = object hit top side
	 * 0x10 = object passed through floor??? - maybe do this in out of bounds check???
	 * */
}

