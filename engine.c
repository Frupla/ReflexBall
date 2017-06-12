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
	Tvector direction; // Speed and direction (only relevant for the ball)
	//zones? - so far no zones
	char sizeX; // represent the horizontal size factor
    char sizeY;	// represent the vertical size factor
	char color;
} entity;

void initiate(int heightOfMap, int widthOfMap){
	int i;
	for(i = 1; i <= heightOfMap; i++){
		gotoxy(1, i);
		printf("%c", 72);
		gotoxy(widthOfMap, i);
		printf("%c", 72);
	}
	for(i = 1; i <= widthOfMap; i++){
		gotoxy(i, 1);
		printf("%c", 72);
	}
}

void drawPlayer(entity* object){
	int i;
	for(i = 0; i <= (object->sizeX); i++){
		gotoxy(object->x1 + i,object->y1);
		printf("%c", 219);	
	}
	object->changedSinceLast = 0;
}

void drawBall(entity* object){
	int i, j;
	gotoxy(object->x1,object->y1);
	printf("%c", 184);	
	object->changedSinceLast = 0;
}

void drawBreakable(entity* object){
	int i, j;
	for(i = 0; i <= (object->sizeX); i++){
		for(j = 0; j <= (object->sizeY); j++){
			gotoxy(object->x1 + i,object->y1);
			printf("%c", 177);	
		}
	}
	object->changedSinceLast = 0;
}

void drawSolid(entity* object){
	int i, j;
	for(i = 0; i <= (object->sizeX); i++){
		for(j = 0; j <= (object->sizeY); j++){
			gotoxy(object->x1 + i,object->y1);
			printf("%c", 72);	
		}
	}
	object->changedSinceLast = 0;
}

void drawMap(entity *object) {
    while(object->whatIsThis) {
        switch (object->whatIsThis) {
            case 0x01:
                drawPlayer(object);
                break;
            case 0x02:
                drawBall(object);
                break;
            case 0x03:
                drawBreakable(object);
                break;
            case 0x04:
                drawSolid(object);
                break;
            default:
                break;
        }
        object++;
    }
}
/*
void playerMovement(char buttonPress, entity* object){
	int i;	
	switch(buttonPress) {
		case 0x01: //PF7
			object->x1++;
			object->changedSinceLast = 1;
            for(i = 0; i <= (object->sizeX); i++){	// Deletes the old drawing of the paddle
            	gotoxy(object->x1 + i,object->y1);
            	printf("%c", 20);
			}
            break;
		case 0x02: //PF6
			break;
		case 0x04: //PD3
			object->x1--;
			object->changedSinceLast = 1;
			break;
		case 0x05:	
			for(i = 0; i <= (object->sizeX); i++){	// Deletes the old drawing of the paddle
				gotoxy(object->x1 + i,object->y1);
				printf("%c", 219);	
			} // when PD3 and PF7 is pressed at the same time, nothing happens
            for(i = 0; i <= (object->sizeX); i++){	// Deletes the old drawing of the paddle
            	gotoxy(object->x1 + i,object->y1);
            	printf("%c", 20);
			}
            break;
		case 0x05: // when PD3 and PF7 is pressed at the same time, nothing happens
			break;
		default:
			break;
	}
}

void ballMovement(entity* ball){
	char flag = ballCollision();
	switch(flag){
		case 0x00:
            break;
		case 0x01:
            ball->direction.y = - ball->direction.y;
            break;
		case 0x02:
            ball->direction.x = - ball->direction.x;
            ball->direction.y = - ball->direction.y;
            break;
		case 0x03:
            ball->direction.x = - ball->direction.x;
            break;
		case 0x04:
            ball->direction.x = - ball->direction.x;
            ball->direction.y = - ball->direction.y;
            break;
		case 0x05:
            ball->direction.y = - ball->direction.y;
            break;
        case 0x06:
            ball->direction.x = - ball->direction.x;
            ball->direction.y = - ball->direction.y;
            break;
        case 0x07:
            ball->direction.x = - ball->direction.x;
            break;
        case 0x08:
            ball->direction.x = - ball->direction.x;
            ball->direction.y = - ball->direction.y;
            break;
        case 0x09:
        //TODO: raise like a dead flag or something
		default: break;
	}
    ball->x1 += (int) (ball->direction.x>>8); //might need a -> here instead of .
    ball->y1 += (int) (ball->direction.y>>8); //might need a -> here instead of .
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
	 * 0x01 = object hit top side
	 * 0x02 = object hit top right  corner
	 * 0x03 = object hit right side
	 * 0x04 = object hit bottom right corner
	 * 0x05 = object hit bottom side
	 * 0x06 = object hit bottom left corner
	 * 0x07 = object hit left side
	 * 0x08 = object hit top left corner
	 * 0x09 = object passed through floor??? - maybe do this in out of bounds check???
	 * 
}*/
