#include <eZ8.h>  						  // special encore constants, macros and flash functions
#include <sio.h>	                      // special encore serial i/o functions
#include "ansi.h"
#include "math.h"
#include "Z8encore.h"

#define MAPSIZE 59
#define SOLIDTEXTURE 219
#define PLAYERTEXTURE 223
#define BALLTEXTURE 184
#define BREAKABLETEXTURE 219
#define MORELIFETEXTURE 176
#define MOREBALLSTEXTURE 177
#define WIDERPADDLETEXTURE 178
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

typedef struct stuffTemp{
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
    char lives; //n.o. lives of a player
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
    char powerUp;
    //0x00 0 no powerUp
    //0x01 1 more life <- implementated
    //0x02 1 more ball
    //0x03 wider paddle
} breakable_t;

//Just access PowerUp on

void initiate(){
	int i;
	for(i = 1; i <= MAPSIZE; i++){
		gotoxy(1, i);
		printf("%c", SOLIDTEXTURE);
		gotoxy((4 * MAPSIZE), i);
		printf("%c", SOLIDTEXTURE);
	}
	for(i = 1; i <= (4 * MAPSIZE); i++){
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
    fgcolor(7 - object->lives);
    switch (object->powerUp){
        case 0x00:
            for(i = 0; i <= (object->sizeX); i++){
                for(j = 0; j <= (object->sizeY); j++){
                    gotoxy(object->x1 + i,object->y1 + j);
                    printf("%c", BREAKABLETEXTURE);
                }
            }
            break;
        case 0x01: //more life
            for(i = 0; i <= (object->sizeX); i++){
                for(j = 0; j <= (object->sizeY); j++){
                    gotoxy(object->x1 + i,object->y1 + j);
                    printf("%c", MORELIFETEXTURE);
                }
            }
            break;
        case 0x02: //more balls
            for(i = 0; i <= (object->sizeX); i++){
                for(j = 0; j <= (object->sizeY); j++){
                    gotoxy(object->x1 + i,object->y1 + j);
                    printf("%c", MOREBALLSTEXTURE);
                }
            }
            break;
        case 0x03: //wider paddle
            for(i = 0; i <= (object->sizeX); i++){
                for(j = 0; j <= (object->sizeY); j++){
                    gotoxy(object->x1 + i,object->y1 + j);
                    printf("%c", WIDERPADDLETEXTURE);
                }
            }
            break;
        default:
            for(i = 0; i <= (object->sizeX); i++){
                for(j = 0; j <= (object->sizeY); j++){
                    gotoxy(object->x1 + i,object->y1 + j);
                    printf("%c", BREAKABLETEXTURE);
                }
            }
            break;
    }
    fgcolor(STDTEXTCOLOR);
	object->changedSinceLast = 0;
}

void killBreakable(breakable_t* object, player_t* player){
	int i, j, texture;
    object->lives--;
    switch(object->powerUp){
    	case 0x00: texture = BREAKABLETEXTURE;
    	break;
    	case 0x01: texture = MORELIFETEXTURE;
    	break;
    	case 0x02: texture = MOREBALLSTEXTURE;
    	break;
    	case 0x03: texture = WIDERPADDLETEXTURE;
    	default: 
    	texture = BREAKABLETEXTURE;
    	break;
    }
    if (!object->lives) {
        for (i = 0; i <= (object->sizeX); i++) {
            for (j = 0; j <= (object->sizeY); j++) {
                gotoxy(object->x1 + i, object->y1 + j);
                printf("%c", BACKGROUNDTEXTURE);
            }
        }
        object->changedSinceLast = 0;
        object->whatIsThis = 0x05;
        if (object->powerUp){
            switch (object->powerUp){
                case 0x00:
                    break;
                case 0x01:
                    if (player->lives < 9){
                        player->lives++;
                    }
                default:
                    break;
            }

        }
    //it's dead
    } else {
        fgcolor(7 - object->lives);
        for (i = 0; i <= (object->sizeX); i++) {
            for (j = 0; j <= (object->sizeY); j++) {
                gotoxy(object->x1 + i, object->y1 + j);
                printf("%c", texture);
            }
        }
        fgcolor(STDTEXTCOLOR);
        object->changedSinceLast = 0;
     //it isn't dead
    }
}

char didYouWin(breakable_t* breakables) {
    int i = 0;
    while (breakables[i].whatIsThis){
        if (breakables[i].whatIsThis != 0x05){
            return 0x00; //no you didn't
        }
        i++;
    }
    return 0x01; //yes you did
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
	int i = 15, j = 4, n = 0;
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
    while (breakables[n].whatIsThis){
        if (breakables[n].whatIsThis == 0x03 && breakables[n].changedSinceLast){
            drawBreakable(&breakables[n]);
        }
		n++;
    }
    //Also add a whileloop here for solids if you want to implement them
}

void playerMovement(char buttonPress, player_t* object){
	int i;
	switch(buttonPress) {
		case 0x01: //PF7
			if ((object->x1 + 1 + ((object->sizeX)*5)) < 4 * MAPSIZE){
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
char collisionCheck(int x1, int y1, player_t* player, breakable_t* breakables) { // an array of breakables, an array of players
	char flag = 0;
	int i = 0;
	if(x1 >= MAPSIZE * 4){ // returns true if hit wall
		flag = 0x10;// hit right wall
        return flag;
	}
	if(x1 <= 1){ 
		flag = 0x20; // hit left wall
		return flag;
	}
	if(y1 <= 1){ // returns true if ceiling
		flag = 0x30;// wall is hit
        return flag;
	}
    if (y1 >= MAPSIZE) { // returns true if ball falls through floor
		flag = 0x09;// dead ball
        return flag;
	}
    if (y1 == player->y1) {
        if ((x1 >= player->x1) &&
            (x1 < (player->x1 + player->sizeX))) {
                flag = 0x0A;
                return flag; // left side of paddle
            }
            if ((x1 >= player->x1 + (player->sizeX)) &&
                (x1 < (player->x1 + (2 * player->sizeX)))) {
                flag = 0x0B;
                return flag; // middle left side of paddle
            }
            if ((x1 >= (player->x1 + (player->sizeX) * 2)) &&
                (x1 < (player->x1) + (3 * (player->sizeX)))) {
                flag = 0x0C;
                return flag; // middle of paddle
            }
            if ((x1 >= (player->x1 + (player->sizeX * 3))) &&
                (x1 < (player->x1 + (4 * player->sizeX)))) {
                flag = 0x0D;
                return flag; // right middle side of paddle
            }
            if ((x1 >= (player->x1 + (player->sizeX) * 4)) &&
                (x1 <= (player->x1 + (5 * (player->sizeX))))) {
                flag = 0x0E;
                return flag; // right side of paddle
            }
     }
    while (breakables[i].whatIsThis != 0x00){
        if(breakables[i].whatIsThis == 0x03) {    //checks & breaks breakables
            if ((x1 >= (breakables[i].x1)) &&
                (x1 <= (breakables[i].x1 + breakables[i].sizeX))) {
                if (y1 == (breakables[i].y1)) {
                    if (x1 == (breakables[i].x1)) {
                        flag = 0x08;//top left corner
                        killBreakable(&breakables[i], player);
                        return flag;
                    } else if (x1 == (breakables[i].x1 + breakables[i].sizeX)) {
                        flag = 0x02;//top right corner
                        killBreakable(&breakables[i], player);
                        return flag;
                    } else {
                        flag = 0x01; // hit top
                        killBreakable(&breakables[i], player);
                        return flag;
                    }
                } else if (y1 == (breakables[i].y1 + breakables[i].sizeY)) {
                    if (x1 == (breakables[i].x1)) {
                        flag = 0x06;//bottom left corner
                        killBreakable(&breakables[i], player);
                        return flag;
                    } else if (x1 == (breakables[i].x1 + breakables[i].sizeX)) {
                        flag = 0x04;//bottom right corner
                        killBreakable(&breakables[i], player);
                        return flag;
                    } else {
                        flag = 0x05; // hit bottom
                        killBreakable(&breakables[i], player);
                        return flag;
                    }
                }
            }
            if ((y1 > (breakables[i].y1))
                && (y1 < (breakables[i].y1 + breakables[i].sizeY))) {
                if (x1 == (breakables[i].x1)) {
                    flag = 0x07; // hit left
                    killBreakable(&breakables[i], player);
                    return flag;
                } else if (x1 == (breakables[i].x1 + breakables[i].sizeX)) {
                    flag = 0x03; // hit right
                    killBreakable(&breakables[i], player);
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
char ballMovement(ball_t *ball, player_t* player, breakable_t *breakables) { //1 ball, 1 player and list of breakables
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
    flag = collisionCheck(tempX, tempY, player, breakables);
    switch(flag){
    	case 0x00:
            break;
		case 0x01:
            ball->direction.y = - ball->direction.y;
            flag = 0x01; //Ball hit breakable
            break;
        case 0x02: //Top right corner
            if (ball[0].direction.x > 0) {
                ball[0].direction.y = -ball[0].direction.y;
            } else {
                ball[0].direction.x = -ball[0].direction.x;
            }
            flag = 0x01; //Ball hit breakable
            break;
		case 0x03:
            ball->direction.x = - ball->direction.x;
            flag = 0x01; //Ball hit breakable
            break;
        case 0x04: //Bottom right corner
            if (ball[0].direction.y > 0) {
                ball[0].direction.x = -ball[0].direction.x;
            } else {
                ball->direction.y = -ball->direction.y;
            }
            flag = 0x01; //Ball hit breakable
            break;
		case 0x05:
            ball->direction.y = - ball->direction.y;
            flag = 0x01; //Ball hit breakable
            break;
        case 0x06: // Bottom left cornor
            if (ball[0].direction.y > 0) {
                ball[0].direction.x = -ball[0].direction.x;
            } else {
                ball->direction.y = -ball->direction.y;
            }
            flag = 0x01; //Ball hit breakable
            break;
        case 0x07:
            ball->direction.x = - ball->direction.x;
            flag = 0x01; //Ball hit wall
            break;
        case 0x08: //Top left corner
            if (ball[0].direction.x < 0) {
                ball[0].direction.y = -ball[0].direction.y;
            } else {
                ball[0].direction.x = -ball[0].direction.x;
            }
            flag = 0x01; //Ball hit wall
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
        case 0x10: //Right wall
            ball->direction.x = -ball->direction.x;
            flag = 0x04; //Ball hit wall
            break;
        case 0x20: //Left wall
            ball->direction.x = -ball->direction.x;
            break;
        case 0x30: //Ceiling
            ball->direction.y = -ball->direction.y;
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