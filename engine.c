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


typedef struct{
	char changedSinceLast;
	char whatIsThis;//player           - 0x01
					//ball             - 0x02
					//breakable        - 0x03
					//solid            - 0x04
                    //broken breakable - 0x05
					//nothing          - 0x00
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
		printf("%c", 220);
	}
}


//draw and kill functions take pointers. If you already have a pointer to the Map pass 'map' pass ?? (possibly just
// increment map and pass that, possible pass map[i], let me check)
// If you just have the map, pass &map[i]
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

void killBreakable(entity* object){
	int i, j;
	for(i = 0; i <= (object->sizeX); i++){
		for(j = 0; j <= (object->sizeY); j++){
			gotoxy(object->x1 + i,object->y1 + j);
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
			gotoxy(1,1);
            break;
		case 0x02: //PF6
			break;
		case 0x04: //PD3
			gotoxy(object->x1 + object->sizeX,object->y1);
            printf("%c", 0x20);
			if((object->x1 - 1) > 1){
				object->x1--;
			}
			gotoxy(object->x1,object->y1);
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
char collisionCheck(int x1, int y1, entity map[]) { // an array of breakables, solids and a ball
	char flag = 0;
	int i = 2; // i is equal to number of players + number of balls
	if(x1 >= (MAPSIZE * 2)  || x1 <= 1){ // returns true if hit wall
		flag = 0x03;// hit wall
	}
	if(y1 <= 2){ // returns true if wall hit
		flag = 0x07;// wall is hit
	}
	if (y1 > MAPSIZE){ // returns true if ball falls through floor
		flag = 0x09;// dead ball
	}
	while(map[i].whatIsThis){
			if((map[i].whatIsThis == 0x03) //hit top
			 && (x1 >= map[i].x1) 
			 && (x1 <= map[i].x1 + map[i].sizeX)){
				if(y1 == map[i].y1){
					if (x1 == map[i].x1){
						flag = 0x08;//top left corner
					}else if (x1 == map[i].x1 + map[i].sizeX){
						flag = 0x02;//top right corner
					}else{
						flag = 0x01; // hit top
					}
				} else if(y1 == map[i].y1 + map[i].sizeY){
					if (x1 == map[i].x1){
						flag = 0x06;//bottom left corner
					}else if (x1 == map[i].x1 + map[i].sizeX){
						flag = 0x04;//bottom right corner
					}else{
						flag = 0x05; // hit bottom
						}
					}
				}
			}
			if((map[i].whatIsThis == 0x03) //hit top
			 && (y1 > map[i].y1) 
			 && (y1 < map[i].y1 + map[i].sizeY)){
				if(x1 == map[i].x1){
					flag = 0x07; // hit left
				} else if(x1 == map[i].x1 + map[i].sizeX){
					flag = 0x03; // hit right
				}
			}
		return flag;
	}
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
	 */

//Ball movement ver 2
void ballMovement(entity *map) {
	char flag;
	//Variables

	//int where;
	//Find the ball entity (uncomment if not at 1)
	//for(where=1; map[where].whatIsThis != 0x02; where++){}

	//Remove the old ball
	gotoxy(map[1].x1, map[1].y1);
	printf("H");
	//Change position and print the new
    flag = collisionCheck( map[1].x1 + ((map[1].direction.x) + 0x2000) >> 14,map[1].y1 + ((map[1].direction.y) + 0x2000) >> 14, map);
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
        	//TODO come up with something
        default: 
        	break;
    }
    map[1].x1 += ((map[1].direction.x) + 0x2000) >> 14;
    map[1].y1 += ((map[1].direction.y) + 0x2000) >> 14;
	gotoxy(map[1].x1, map[1].y1);
	printf("%c", BALLTEXTURE);
	gotoxy(1, 1);
}