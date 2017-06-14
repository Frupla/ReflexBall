#include <eZ8.h>  						  // special encore constants, macros and flash functions
#include <sio.h>	                      // special encore serial i/o functions
#include "ansi.h"
#include "math.h"
#include "Z8encore.h"

#define MAPSIZE 60
#define SOLIDTEXTURE 219
#define PLAYERTEXTURE 223
#define BALLTEXTURE 184
#define BREAKABLETEXTURE 219
#define BACKGROUNDTEXTURE 32
#define STDTEXTCOLOR 15

#define EIGHTEEN_FOURTEEN_TO_INT(a) ((int)((a + 0x2000) >> 14))  //this is kinda shitty, cuts 2 MSB when recast as int, and it will be
#define LONG_TO_EIGHTEEN_FOURTEEN(a) (a << 14)

/*
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
	char color; // n.o. lives for breakables  (color breakables after this). Is set to 3 in drawBreakables
                // 0x00 - dead, no lives
} entity;
*/

typedef struct{
    char changedSinceLast;
    //not sure if I need this
    char whatIsThis;//player           - 0x01
    //ball             - 0x02
    //breakable        - 0x03
    //solid            - 0x04
    //broken breakable - 0x05
    //nothing          - 0x00
    long x1; //1. coordinate, placement, 18.14
    long y1; //2. coordiante, placement, 18.14
    Tvector direction; // Speed and direction (only relevant for the ball)
    char size; // represent the size factor, balls must be symmetric
    char color; // n.o. lives for breakables  (color breakables after this). Is set to 3 in drawBreakables
} ball_t;

typedef struct{
    char changedSinceLast;
    char whatIsThis;//player           - 0x01
    //ball             - 0x02
    //breakable        - 0x03
    //solid            - 0x04
    //broken breakable - 0x05
    //nothing          - 0x00
    int x1; //1. coordinate, placement, 18.14
    int y1; //2. coordiante, placement, 18.14
    //Tvector direction; // Speed and direction (only relevant for the ball)
    //zones? - so far no zones
    char sizeX; // represent the horizontal size factor
    char color; // n.o. lives for breakables  (color breakables after this). Is set to 3 in drawBreakables
    // 0x00 - dead, no lives
} player_t;

typedef struct{
    char changedSinceLast;
    char whatIsThis;
    int x1; //1. coordinate, placement, 18.14
    int y1; //2. coordiante, placement, 18.14
    //Tvector direction; // Speed and direction (only relevant for the ball)
    //zones? - so far no zones
    char sizeX; // represent the horizontal size factor
    char sizeY;	// represent the vertical size factor
    char lives; // n.o. lives for breakables  (color breakables after this).
    // DO NOT FIVE IT MORE THAN 7 HEALTH.
    // 0x00 - dead, no lives
} breakable_t;


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
// If you just have te map, pass &map[i]
void drawPlayer(player_t * object){
	int i;
	for(i = 0; i <= ((object->sizeX)*5); i++){
		gotoxy(object->x1 + i,object->y1);
        fgcolor(object->color);
		printf("%c", PLAYERTEXTURE);
	}
    fgcolor(STDTEXTCOLOR);
	object->changedSinceLast = 0;
}

void drawBall(ball_t* object){
	int i, j;
	gotoxy(EIGHTEEN_FOURTEEN_TO_INT(object->x1),EIGHTEEN_FOURTEEN_TO_INT(object->y1));
    fgcolor(object[0].color);
	printf("%c", BALLTEXTURE);
	object->changedSinceLast = 0;
    fgcolor(STDTEXTCOLOR);
}

void drawBreakable(breakable_t* object){
	int i, j;
    //object->lives = 3; //change if you want to change n.o. lives
	for(i = 0; i <= (object->sizeX); i++){
		for(j = 0; j <= (object->sizeY); j++){
            fgcolor(7 - object->lives);
			gotoxy(object->x1 + i,object->y1 + j);
			printf("%c", BREAKABLETEXTURE);
		}
	}
    fgcolor(STDTEXTCOLOR);
	object->changedSinceLast = 0;
}

void killBreakable(breakable_t* object){
	int i, j;
    object->lives--;
    if (!object->lives) {
        for (i = 0; i <= (object->sizeX); i++) {
            for (j = 0; j <= (object->sizeY); j++) {
                gotoxy(object->x1 + i, object->y1 + j);
                printf("%c", BACKGROUNDTEXTURE);
            }
        }
        object->changedSinceLast = 0;
        object->whatIsThis = 0x05;
    } else {
        fgcolor(7 - object->lives);
        for (i = 0; i <= (object->sizeX); i++) {
            for (j = 0; j <= (object->sizeY); j++) {
                gotoxy(object->x1 + i, object->y1 + j);
                printf("%c", BREAKABLETEXTURE);
            }
        }
        fgcolor(STDTEXTCOLOR);
        object->changedSinceLast = 0;
    }
}
 /* //Actually a struct for this as well as all the remainig code for handling
  * DON'T DELETE
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
  */

//pre: all arrays must be nothing-terminated, ei have the last object have whatIsThis= 0x00
//     arrays must contain all objects on map
void drawMap(player_t* players, ball_t* balls, breakable_t* breakables) {
    while (players->whatIsThis){
        if (players->whatIsThis == 0x01 && players->changedSinceLast){
            drawPlayer(players);
        }
        players++;
    }
    while (balls->whatIsThis){
        if (players->whatIsThis == 0x02 && balls->changedSinceLast){
            drawBall(balls);
        }
        balls++;
    }
    while (breakables->whatIsThis){
        if (breakables->whatIsThis == 0x03 && breakables->changedSinceLast){
            drawBreakable(breakables);
        }
        breakables++;
    }
    //Also add a whileloop here for solids if you want to implement them
}

void playerMovement(char buttonPress, player_t* object){
	int i;
	switch(buttonPress) {
		case 0x01: //PF7
			if ((object->x1 + 1 + ((object->sizeX)*5)) < 2 * MAPSIZE){
                gotoxy(object->x1, object->y1);
                printf("%c", 0x20);
				object->x1++;
			}
			gotoxy(object->x1 + ((object->sizeX)*5),object->y1);
            fgcolor(object[0].color);
            printf("%c", PLAYERTEXTURE);
            fgcolor(15);
			gotoxy(1,1);
            break;
		case 0x02: //PF6
			break;
		case 0x04: //PD3
			if((object->x1 - 1) > 1){
                gotoxy(object->x1 + ((object->sizeX) * 5), object->y1);
                printf("%c", 0x20);
				object->x1--;
			}
			gotoxy(object->x1,object->y1);
            fgcolor(object[0].color);
            printf("%c", PLAYERTEXTURE);
            fgcolor(15);
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
char collisionCheck(int x1, int y1, player_t* players, breakable_t* breakables) { // an array of breakables, an array of players
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
    if (y1 >= MAPSIZE) { // returns true if ball falls through floor
		flag = 0x09;// dead ball
        return flag;
	}
    while (players[i].whatIsThis != 0x00){
        if (y1 == players[i].y1) {
            if ((x1 >= players[i].x1) &&
                (x1 < (players[i].x1 + players[i].sizeX))) {
                flag = 0x0A;
                return flag; // left side of paddle
            }
            if ((x1 >= players[i].x1 + (players[i].sizeX)) &&
                (x1 < (players[i].x1 + (2 * players[i].sizeX)))) {
                flag = 0x0B;
                return flag; // middle left side of paddle
            }
            if ((x1 >= (players[i].x1 + (players[i].sizeX) * 2)) &&
                (x1 < (players[i].x1) + (3 * (players[i].sizeX)))) {
                flag = 0x0C;
                return flag; // middle of paddle
            }
            if ((x1 >= (players[i].x1 + (players[i].sizeX * 3))) &&
                (x1 < (players[i].x1 + (4 * players[i].sizeX)))) {
                flag = 0x0D;
                return flag; // right middle side of paddle
            }
            if ((x1 >= (players[i].x1 + (players[i].sizeX) * 4)) &&
                (x1 <= (players[i].x1 + (5 * (players[i].sizeX))))) {
                flag = 0x0E;
                return flag; // right side of paddle
            }
        }
        i++;
    }
    i = 0;
    while (breakables[i].whatIsThis != 0x00){
        if(breakables[i].whatIsThis == 0x03) {    //checks & breaks breakables
            if ((x1 >= (breakables[i].x1)) &&
                (x1 <= (breakables[i].x1 + breakables[i].sizeX))) {
                if (y1 == (breakables[i].y1)) {
                    if (x1 == (breakables[i].x1)) {
                        flag = 0x08;//top left corner
                        killBreakable(&breakables[i]);
                        return flag;
                    } else if (x1 == (breakables[i].x1 + breakables[i].sizeX)) {
                        flag = 0x02;//top right corner
                        killBreakable(&breakables[i]);
                        return flag;
                    } else {
                        flag = 0x01; // hit top
                        killBreakable(&breakables[i]);
                        return flag;
                    }
                } else if (y1 == (breakables[i].y1 + breakables[i].sizeY)) {
                    if (x1 == (breakables[i].x1)) {
                        flag = 0x06;//bottom left corner
                        killBreakable(&breakables[i]);
                        return flag;
                    } else if (x1 == (breakables[i].x1 + breakables[i].sizeX)) {
                        flag = 0x04;//bottom right corner
                        killBreakable(&breakables[i]);
                        return flag;
                    } else {
                        flag = 0x05; // hit bottom
                        killBreakable(&breakables[i]);
                        return flag;
                    }
                }
            }
            if ((y1 > (breakables[i].y1))
                && (y1 < (breakables[i].y1 + breakables[i].sizeY))) {
                if (x1 == (breakables[i].x1)) {
                    flag = 0x07; // hit left
                    killBreakable(&breakables[i]);
                    return flag;
                } else if (x1 == (breakables[i].x1 + breakables[i].sizeX)) {
                    flag = 0x03; // hit right
                    killBreakable(&breakables[i]);
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
char ballMovement(ball_t *ball, player_t *players, breakable_t *breakables) { //1 ball, all the players and breakables
	char flag;
	int tempX;
	int tempY;
	//Variables

	//int where;
    // --- Find the ball entity (uncomment if not at 1) ----
	//for(where=1; map[where].whatIsThis != 0x02; where++){}

	//Remove the old ball
	gotoxy(EIGHTEEN_FOURTEEN_TO_INT(ball->x1), EIGHTEEN_FOURTEEN_TO_INT(ball->y1));
	printf(" ");

    //Calculate the next position, and pass this to collisionCheck.
    tempX = EIGHTEEN_FOURTEEN_TO_INT(ball->x1 + ball->direction.x);
	tempY = EIGHTEEN_FOURTEEN_TO_INT(ball->y1 + ball->direction.y);
    flag = collisionCheck(tempX, tempY, players, breakables);
    switch(flag){
    	case 0x00:
            break;
		case 0x01:
            ball->direction.y = - ball->direction.y;
            flag = 0x01; //Ball hit breakable
            break;
		case 0x02:
            ball->direction.x = - ball->direction.x;
            ball->direction.y = - ball->direction.y;
            flag = 0x01; //Ball hit breakable
            break;
		case 0x03:
            ball->direction.x = - ball->direction.x;
            flag = 0x01; //Ball hit breakable
            break;
		case 0x04:
			ball->direction.x = - ball->direction.x;
            ball->direction.y = - ball->direction.y;
            flag = 0x01; //Ball hit breakable
            break;
		case 0x05:
            ball->direction.y = - ball->direction.y;
            flag = 0x01; //Ball hit breakable
            break;
        case 0x06:
            ball->direction.x = - ball->direction.x;
            ball->direction.y = - ball->direction.y;
            flag = 0x01; //Ball hit breakable
            break;
        case 0x07:
            ball->direction.x = - ball->direction.x;
            flag = 0x01; //Ball hit breakable
            break;
        case 0x08:
            ball->direction.x = - ball->direction.x;
            ball->direction.y = - ball->direction.y;
            flag = 0x01; //Ball hit breakable
            break;
        case 0x09 :
            flag = 0x02; // Ball out of bounds
            ball->direction.x = 0;
            ball->direction.y = 0;
            break;
        case 0x0a :
            //TODO : Change it from a fixed reflect angle,
            // to something that varies with the incoming angle
            ball->direction.y = -ball->direction.y;
            rotate(&ball->direction, -48);
            if (ball->direction.y > -(0x1000)) {
                rotate(&ball->direction, 12);
            }
            flag = 0x03; // Ball hit paddle
            break;
        case 0x0b :
            ball->direction.y = -ball->direction.y;
            rotate(&ball->direction, -24);
            if (ball->direction.y > -(0x1000)) {
                rotate(&ball->direction, 12);
            }
            flag = 0x03; // Ball hit paddle
            break;
        case 0x0c :
            ball->direction.y = -ball->direction.y;
            flag = 0x03; // Ball hit paddle
            break;
        case 0x0d :
            ball->direction.y = -ball->direction.y;
            rotate(&ball->direction, 24);
            if (ball->direction.y > -(0x1000)) {
                rotate(&ball->direction, -12);
            }
            flag = 0x03; // Ball hit paddle
            break;
        case 0x0e :
            ball->direction.y = -ball->direction.y;
            rotate(&ball->direction, 48);
            if (ball->direction.y > -(0x1000)) {
                rotate(&ball->direction, -12);
            }
            flag = 0x03; // Ball hit paddle
            break;
        default:
            flag = 0x00; //eh
        	break;
    }
    //Change position and print the new
    ball->x1 += ball->direction.x;
    ball->y1 += ball->direction.y;
    fgcolor(ball[0].color);
	gotoxy(EIGHTEEN_FOURTEEN_TO_INT(ball->x1), EIGHTEEN_FOURTEEN_TO_INT(ball->y1));
	printf("%c", BALLTEXTURE);
    fgcolor(15);

    return flag; // Give away information!
}