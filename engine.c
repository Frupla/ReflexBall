#include <eZ8.h>  						  // special encore constants, macros and flash functions
#include <sio.h>	                      // special encore serial i/o functions
#include "ansi.h"
#include "math.h"
#include "Z8encore.h"

#define MAPSIZE 100
#define SOLIDTEXTURE 72
#define PLAYERTEXTURE 219
#define BALLTEXTURE 184
#define BREAKABLETEXTURE 177


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

void initiate(){
	int i;
	for(i = 1; i <= MAPSIZE; i++){
		gotoxy(1, i);
		printf("%c", SOLIDTEXTURE);
		gotoxy((2 * MAPSIZE), i);
		printf("%c", SOLIDTEXTURE);
	}
	for(i = 1; i <= (2 * MAPSIZE); i++){
		gotoxy(i, 1);
		printf("%c", SOLIDTEXTURE);
	}
}

void drawPlayer(entity* object){
	int i;
	for(i = 0; i <= (object->sizeX); i++){
		gotoxy(object->x1 + i,object->y1);
		printf("%c", PLAYERTEXTURE);
	}
	object->changedSinceLast = 0;
}

void drawBall(entity* object){
	int i, j;
	gotoxy(object->x1,object->y1);
	printf("%c", BALLTEXTURE);
	object->changedSinceLast = 0;
}

void drawBreakable(entity* object){
	int i, j;
	for(i = 0; i <= (object->sizeX); i++){
		for(j = 0; j <= (object->sizeY); j++){
			gotoxy(object->x1 + i,object->y1 + j);
			printf("%c", BREAKABLETEXTURE);
		}
	}
	object->changedSinceLast = 0;
}

void drawSolid(entity* object){
	int i, j;
	for(i = 0; i <= (object->sizeX); i++){
		for(j = 0; j <= (object->sizeY); j++){
			gotoxy(object->x1 + i,object->y1 + j);
			printf("%c", SOLIDTEXTURE);
		}
	}
	object->changedSinceLast = 0;
}

//pre: map must be nothing-terminated
//     map is a 1D array containing all objects on the map
void drawMap(entity* map) {
    while(map->whatIsThis) {
        switch (map->whatIsThis) {
            case 0x01:
                if (map->changedSinceLast) {
                    drawPlayer(map);
                }
                break;
            case 0x02:
                if (map->changedSinceLast) {
                    drawBall(map);
                }
                break;
            case 0x03:
                if (map->changedSinceLast) {
                    drawBreakable(map);
                }
                break;
            case 0x04:
                if (map->changedSinceLast) {
                    drawSolid(map);
                }
                break;
            default:
                break;
        }
        map++;
    }
}

void playerMovement(char buttonPress, entity* object){
	int i;	
	switch(buttonPress) {
		case 0x01: //PF7
            gotoxy(object->x1,object->y1);
            printf("%c", 0x20);
			if ((object->x1 + 1 + object->sizeX) < 2 * MAPSIZE){
				object->x1++;
			}
			gotoxy(object->x1 + object->sizeX,object->y1);
            printf("%c", PLAYERTEXTURE);
            break;
		case 0x02: //PF6
			break;
		case 0x04: //PD3
			gotoxy(object->x1 + object->sizeX,object->y1);
            printf("%c", 0x20);
			if ((object->x1 + 1) < 1){
				object->x1--;
			}
			gotoxy(object->x1,object->y1);
            printf("%c", PLAYERTEXTURE);
			break;
		case 0x05:
			// when PD3 and PF7 is pressed at the same time, nothing happens
			break;
		default:
			break;
		}
}
/*
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
