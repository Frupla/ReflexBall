#include <eZ8.h>  						  // special encore constants, macros and flash functions
#include <sio.h>	                      // special encore serial i/o functions
#include "ansi.h"
#include "math.h"
#include "Z8encore.h"

#define MAPSIZE 100
#define SOLIDTEXTURE 219
#define PLAYERTEXTURE 223
#define BALLTEXTURE 184
#define BREAKABLETEXTURE 177
#define BACKGROUNDTEXTURE 32

#define EIGHTEEN_FOURTEEN_TO_INT(a) ((int)((a + 0x2000) >> 14))  //this is kinda shitty, cuts 2 MSB when recast as int, and it will be
#define LONG_TO_EIGHTEEN_FOURTEEN(a) (a << 14)


typedef struct{
	char changedSinceLast;
	char whatIsThis;//player           - 0x01
					//ball             - 0x02
					//breakable        - 0x03
					//solid            - 0x04
                    //broken breakable - 0x05
					//nothing          - 0x00
	long x1; //1. coordinate, placement, 18.14
	long y1; //2. coordiante, placement, 18.14
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
		printf("%c", 220);
	}
}


//draw and kill functions take pointers. If you already have a pointer to the Map pass 'map' pass ?? (possibly just
// increment map and pass that, possible pass map[i], let me check)
// If you just have the map, pass &map[i]
void drawPlayer(entity* object){
	int i;
	for(i = 0; i <= ((object->sizeX)*5); i++){
		gotoxy(EIGHTEEN_FOURTEEN_TO_INT(object->x1) + i,EIGHTEEN_FOURTEEN_TO_INT(object->y1));
		printf("%c", PLAYERTEXTURE);
	}
	object->changedSinceLast = 0;
}

void drawBall(entity* object){
	int i, j;
	gotoxy(EIGHTEEN_FOURTEEN_TO_INT(object->x1),EIGHTEEN_FOURTEEN_TO_INT(object->y1));
	printf("%c", BALLTEXTURE);
	object->changedSinceLast = 0;
}

void drawBreakable(entity* object){
	int i, j;
	for(i = 0; i <= (object->sizeX); i++){
		for(j = 0; j <= (object->sizeY); j++){
			gotoxy(EIGHTEEN_FOURTEEN_TO_INT(object->x1) + i,EIGHTEEN_FOURTEEN_TO_INT(object->y1) + j);
			printf("%c", BREAKABLETEXTURE);
		}
	}
	object->changedSinceLast = 0;
}

void killBreakable(entity* object){
	int i, j;
	for(i = 0; i <= (object->sizeX); i++){
		for(j = 0; j <= (object->sizeY); j++){
			gotoxy(EIGHTEEN_FOURTEEN_TO_INT(object->x1) + i,EIGHTEEN_FOURTEEN_TO_INT(object->y1) + j);
			printf("%c", BACKGROUNDTEXTURE);
		}
	}
	object->changedSinceLast = 0;
    object->whatIsThis = 0x05;
}

void drawSolid(entity* object){
	int i, j;
	for(i = 0; i <= (object->sizeX); i++){
		for(j = 0; j <= (object->sizeY); j++){
			gotoxy(EIGHTEEN_FOURTEEN_TO_INT(object->x1 + i),EIGHTEEN_FOURTEEN_TO_INT(object->y1) + j);
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
    long l = 1;
	switch(buttonPress) {
		case 0x01: //PF7
            gotoxy(EIGHTEEN_FOURTEEN_TO_INT(object->x1),EIGHTEEN_FOURTEEN_TO_INT(object->y1));
            printf("%c", 0x20);
			if ((EIGHTEEN_FOURTEEN_TO_INT(object->x1) + 1 + ((object->sizeX)*5)) < 2 * MAPSIZE){
				object->x1 = object->x1 + LONG_TO_EIGHTEEN_FOURTEEN(l);
			}
			gotoxy(EIGHTEEN_FOURTEEN_TO_INT(object->x1) + ((object->sizeX)*5),EIGHTEEN_FOURTEEN_TO_INT(object->y1));
            printf("%c", PLAYERTEXTURE);
			gotoxy(1,1);
            break;
		case 0x02: //PF6
			break;
		case 0x04: //PD3
			gotoxy(EIGHTEEN_FOURTEEN_TO_INT(object->x1) + ((object->sizeX)*5),EIGHTEEN_FOURTEEN_TO_INT(object->y1));
            printf("%c", 0x20);
			if((EIGHTEEN_FOURTEEN_TO_INT(object->x1) - 1) > 1){
				object->x1= object->x1 - LONG_TO_EIGHTEEN_FOURTEEN(l);
			}
			gotoxy(EIGHTEEN_FOURTEEN_TO_INT(object->x1),EIGHTEEN_FOURTEEN_TO_INT(object->y1));
            printf("%c", PLAYERTEXTURE);
			gotoxy(1,1);
			break;
		case 0x05:
			// when PD3 and PF7 is pressed at the same time, nothing happens
			break;
		default:
			break;
		}
}

// pre: takes position that the ball would have (current position + direction), and checks it for overlap
char collisionCheck(int x1, int y1, entity* map[]) { // an array of breakables, solids and a ball
	char flag = 0;
	int i = 0;
	if(x1 >= MAPSIZE * 2){ // returns true if hit wall
		flag = 0x07;// hit right wall
        return flag;
	}
	if(x1 <= 1){ 
		flag = 0x03; // hit left wall
		return flag;
	}
	if(y1 <= 1){ // returns true if ceiling
		flag = 0x05;// wall is hit
        return flag;
	}
	if (y1 > MAPSIZE){ // returns true if ball falls through floor
		flag = 0x09;// dead ball
        return flag;
	}
	while(map[i]->whatIsThis){
		if(map[i]->whatIsThis == 0x01 && y1 == EIGHTEEN_FOURTEEN_TO_INT(map[i]->y1)) {
			if ((x1 >= EIGHTEEN_FOURTEEN_TO_INT(map[i]->x1)) && (x1 < (EIGHTEEN_FOURTEEN_TO_INT(map[i]->x1) + ((map[i]->sizeX))))){
				flag = 0x0A; 
				return flag; // left side of paddle
			}
			if ((x1 >= (EIGHTEEN_FOURTEEN_TO_INT(map[i]->x1) + ((map[i]->sizeX)))) && (x1 < (EIGHTEEN_FOURTEEN_TO_INT(map[i]->x1) + (2*(map[i]->sizeX))))){
				flag = 0x0B;
				return flag; // middle left side of paddle
			}
			if ((x1 >= (EIGHTEEN_FOURTEEN_TO_INT(map[i]->x1) + ((map[i]->sizeX)*2))) && (x1 < (EIGHTEEN_FOURTEEN_TO_INT(map[i]->x1) + (3*(map[i]->sizeX))))){
				flag = 0x0C;
				return flag; // middle of paddle
			}
			if ((x1 >= (EIGHTEEN_FOURTEEN_TO_INT(map[i]->x1) + ((map[i]->sizeX)*3))) && (x1 < (EIGHTEEN_FOURTEEN_TO_INT(map[i]->x1) + (4*(map[i]->sizeX))))){
				flag = 0x0D;
				return flag; // right middle side of paddle
			}
			if ((x1 >= (EIGHTEEN_FOURTEEN_TO_INT(map[i]->x1) + ((map[i]->sizeX)*4))) && (x1 < (EIGHTEEN_FOURTEEN_TO_INT(map[i]->x1) + (5*(map[i]->sizeX))))){
				flag = 0x0E;
				return flag; // right side of paddle
			}			
		}
        if(map[i]->whatIsThis == 0x03) {
            if ((x1 >= EIGHTEEN_FOURTEEN_TO_INT(map[i]->x1)) && (x1 <= EIGHTEEN_FOURTEEN_TO_INT(map[i]->x1) + map[i]->sizeX)) {
                if (y1 == EIGHTEEN_FOURTEEN_TO_INT(map[i]->y1)) {
                    if (x1 == EIGHTEEN_FOURTEEN_TO_INT(map[i]->x1)) {
                        flag = 0x08;//top left corner
                        killBreakable(map[i]);
                        return flag;
                    } else if (x1 == EIGHTEEN_FOURTEEN_TO_INT(map[i]->x1) + map[i]->sizeX) {
                        flag = 0x02;//top right corner
                        return flag;
                    } else {
                        flag = 0x01; // hit top
                        return flag;
                    }
                } else if (y1 == EIGHTEEN_FOURTEEN_TO_INT(map[i]->y1) + map[i]->sizeY) {
                    if (x1 == EIGHTEEN_FOURTEEN_TO_INT(map[i]->x1)) {
                        flag = 0x06;//bottom left corner
                        return flag;
                    } else if (x1 == EIGHTEEN_FOURTEEN_TO_INT(map[i]->x1) + map[i]->sizeX) {
                        flag = 0x04;//bottom right corner
                        return flag;
                    } else {
                        flag = 0x05; // hit bottom
                        return flag;
                    }
                }
            }
            if ((y1 > EIGHTEEN_FOURTEEN_TO_INT(map[i]->y1))
                && (y1 < EIGHTEEN_FOURTEEN_TO_INT(map[i]->y1) + map[i]->sizeY)) {
                if (x1 == EIGHTEEN_FOURTEEN_TO_INT(map[i]->x1)) {
                    flag = 0x07; // hit left
                    return flag;
                } else if (x1 == EIGHTEEN_FOURTEEN_TO_INT(map[i]->x1) + map[i]->sizeX) {
                    flag = 0x03; // hit right
                    return flag;
                }
            }
        }
	i++;
    }
	return flag;
}
	/* flag encoding
	 * 0x00 = no collision
	 * 0x01 = object hit top side
	 * 0x02 = object hit top right  corner
	 * 0x03 = object hit right side or left wall
	 * 0x04 = object hit bottom right corner
	 * 0x05 = object hit bottom side or ceiling
	 * 0x06 = object hit bottom left corner
	 * 0x07 = object hit left side or right wall
	 * 0x08 = object hit top left corner
	 * 0x09 = object passed through floor??? - maybe do this in out of bounds check???
	 * 0x0A = object hit left
	 * 0x0B = object hit left middle
	 * 0x0C	= object hit middle
	 * 0x0D	= object hit right middle
	 * 0x0E = object hit right
	 */

//Ball movement ver 2
void ballMovement(entity *map) {
	char flag;
	int tempX;
	int tempY,ty,tx;
	long dx,dy;
	//Variables

	//int where;
	//Find the ball entity (uncomment if not at 1)
	//for(where=1; map[where].whatIsThis != 0x02; where++){}
	ty = EIGHTEEN_FOURTEEN_TO_INT(map[1].y1);
	tx = EIGHTEEN_FOURTEEN_TO_INT(map[1].x1);
	dx = (((map[1].direction.x) + 0x2000) >> 14);
	dy = (((map[1].direction.y) + 0x2000) >> 14);
	//Remove the old ball
	gotoxy(EIGHTEEN_FOURTEEN_TO_INT(map[1].x1), EIGHTEEN_FOURTEEN_TO_INT(map[1].y1));
	printf(" ");
    tempX = EIGHTEEN_FOURTEEN_TO_INT(map[1].x1 + map[1].direction.x);
	tempY = EIGHTEEN_FOURTEEN_TO_INT(map[1].y1 + map[1].direction.y);
    //flag = collisionCheck(map[1].x1 + ((map[1].direction.x) + 0x2000) >> 14,map[1].y1 + ((map[1].direction.y) + 0x2000) >> 14, map);
    flag = collisionCheck(tempX, tempY, map);
    switch(flag){
    	case 0x00:
            break;
		case 0x01:
            map[1].direction.y = - map[1].direction.y;
            break;
		case 0x02:
            map[1].direction.x = - map[1].direction.x;
            map[1].direction.y = - map[1].direction.y;
            break;
		case 0x03:
            map[1].direction.x = - map[1].direction.x;
            break;
		case 0x04:
			map[1].direction.x = - map[1].direction.x;
            map[1].direction.y = - map[1].direction.y;
            break;
		case 0x05:
            map[1].direction.y = - map[1].direction.y;
            break;
        case 0x06:
            map[1].direction.x = - map[1].direction.x;
            map[1].direction.y = - map[1].direction.y;
            break;
        case 0x07:
            map[1].direction.x = - map[1].direction.x;
            break;
        case 0x08:
            map[1].direction.x = - map[1].direction.x;
            map[1].direction.y = - map[1].direction.y;
            break;
        case 0x09:
        	//TODO come up with something  - Or not
        default: 
        	break;
    }
    //Change position and print the new
    map[1].x1 += map[1].direction.x;
    map[1].y1 += map[1].direction.y;
	gotoxy(EIGHTEEN_FOURTEEN_TO_INT(map[1].x1), EIGHTEEN_FOURTEEN_TO_INT(map[1].y1));
	printf("%c", BALLTEXTURE);
	gotoxy(1, 1);
}