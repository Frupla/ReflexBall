#include <eZ8.h>  						  // special encore constants, macros and flash functions
#include <sio.h>	                      // special encore serial i/o functions
#include "engine.h"
#include "ansi.h"

void level1(breakable_t *breakable, ball_t *ball, player_t *player) {
	int i, j, n=0, flag = 2;
	Tvector tempVec;

    // player setup
    player[0].whatIsThis = 0x01;
    player[0].changedSinceLast = 1;
    player[0].x1 = 50;
    player[0].y1 = 57;
    player[0].sizeX = 0x0A;
    player[0].color = 0x0f;
    player[0].lives = 0x03;
    // Terminate array
    player[1].whatIsThis = 0x00;

    // Ball setup
    tempVec.x = LONG_TO_EIGHTEEN_FOURTEEN(1);
    tempVec.y = LONG_TO_EIGHTEEN_FOURTEEN(0);
    rotate(&tempVec, 64);
    //Ball 1
    ball[0].whatIsThis = 0x02;
    ball[0].changedSinceLast = 1;
    ball[0].xs = LONG_TO_EIGHTEEN_FOURTEEN(40);
    ball[0].ys = LONG_TO_EIGHTEEN_FOURTEEN(25);
    ball[0].x1 = ball[0].xs;
    ball[0].y1 = ball[0].ys;
    ball[0].direction = tempVec;
    ball[0].color = 0x0A;
	rotate(&tempVec,64);
	ball[1].whatIsThis = 0x02;
    ball[1].changedSinceLast = 1;
    ball[1].xs = LONG_TO_EIGHTEEN_FOURTEEN(100);
    ball[1].ys = LONG_TO_EIGHTEEN_FOURTEEN(25);
    ball[1].x1 = ball[0].xs;
    ball[1].y1 = ball[0].ys;
    ball[1].direction = tempVec;
    ball[1].color = 0x0A;
	ball[2].whatIsThis = 0x0;

 	//Breakable setup
    for (i = 2; i <= 132 ; i += 13) {
        for (j = 8; j <= 12; j += 2) {
            breakable[n].whatIsThis = 0x03;
            breakable[n].changedSinceLast = 1;
            breakable[n].x1 = i;
            breakable[n].y1 = j;
            breakable[n].sizeX = 0x0C;
            breakable[n].sizeY = 0x01;
			if(flag >= 2){
            	breakable[n].lives = 0x01;
				flag = 0;
			} else{
				breakable[n].lives = 0x02;
			}
            if(j == 8 && i != 132){
                breakable[n].powerUp = 0x03;
            } else{
                breakable[n].powerUp = 0x00;
            }
			flag++;
            n++;
       }
    }
	for (i = 106; i <= 225 ; i += 13) {
        for (j = 38; j <= 	42; j += 2) {
            breakable[n].whatIsThis = 0x03;
            breakable[n].changedSinceLast = 1;
            breakable[n].x1 = i;
            breakable[n].y1 = j;
            breakable[n].sizeX = 0x0C;
            breakable[n].sizeY = 0x01;
			if(flag >= 2){
            	breakable[n].lives = 0x02;
				flag = 0;
			} else{
				breakable[n].lives = 0x01;
            }
			flag++;
            if(j == 38 && i != 106){
                breakable[n].powerUp = 0x01;
            } else{
                breakable[n].powerUp = 0x00;
            }
            n++;
       }
    }
    breakable[n].whatIsThis = 0x00;
	breakable[0].powerUp = 0x02;
	breakable[33].powerUp = 0x02;
	breakable[32].powerUp = 0x02;
}

void level2(breakable_t * breakable, ball_t * ball, player_t * player){
    int i, j, n=0;
	Tvector tempVec;
    // player setup
    player[0].whatIsThis = 0x01;
    player[0].changedSinceLast = 1;
    player[0].x1 = 50;
    player[0].y1 = 57;
    player[0].sizeX = 0x04;
    player[0].color = 0x0f;
    player[0].lives = 0x03;
    // Paddle 2
    player[1].whatIsThis = 0x01;
    player[1].changedSinceLast = 1;
    player[1].x1 = 50;
    player[1].y1 = 3;
    player[1].sizeX = 0x04;
    player[1].color = 0x0c;
    //Terminate array
    player[2].whatIsThis = 0x00;

    // Ball setup
    tempVec.x = LONG_TO_EIGHTEEN_FOURTEEN(-1);
    tempVec.y = LONG_TO_EIGHTEEN_FOURTEEN(0);
    rotate(&tempVec, -20);
    ball[0].whatIsThis = 0x02;
    ball[0].changedSinceLast = 1;
    ball[0].xs = LONG_TO_EIGHTEEN_FOURTEEN(192);
    ball[0].ys = LONG_TO_EIGHTEEN_FOURTEEN(7);
    ball[0].x1 = ball[0].xs;
    ball[0].y1 = ball[0].ys;
    ball[0].direction = tempVec;

    ball[0].color = 0x0A;
    ball[1].whatIsThis = 0x00;
    //Breakable setup
    for (i = 1; i <= 9 ; i++) {
        for (j = 1; j <= 9 - i; j++) {
            breakable[n].whatIsThis = 0x03;
            breakable[n].changedSinceLast = 1;
            breakable[n].x1 = 20 * i;
            breakable[n].y1 = 5 * j + 5;
            breakable[n].sizeX = 0x0A;
            breakable[n].sizeY = 0x01;
            breakable[n].lives = 5 - j / 2;
            breakable[n].powerUp = 0x0;
            n++;
        }
    }
    for (i = 9; i >= 1 ; i--) {
        for (j = 9; j > 9 - i; j--) {
            breakable[n].whatIsThis = 0x03;
            breakable[n].changedSinceLast = 1;
            breakable[n].x1 = 20 * i + 20;
            breakable[n].y1 = 5 * j + 5;
            breakable[n].sizeX = 0x0A;
            breakable[n].sizeY = 0x01;
            breakable[n].lives = 5 - j / 2;
            breakable[n].powerUp = 0x0;
            n++;
        }
    }
    breakable[4].powerUp = 0x01;
    breakable[60].powerUp = 0x01;
    breakable[10].powerUp = 0x01;
	breakable[40].powerUp = 0x01;
   	breakable[0].powerUp = 0x03;
    breakable[n-1].powerUp = 0x03;
	breakable[29].powerUp = 0x02;
	breakable[70].powerUp = 0x02;
	breakable[n].whatIsThis = 0x00;
}

void level3(breakable_t * breakable, ball_t * ball, player_t * player){
    int i, j, n=0;
	Tvector tempVec;
    // player setup
    player[0].whatIsThis = 0x01;
    player[0].changedSinceLast = 1;
    player[0].x1 = 20;
    player[0].y1 = 57;
    player[0].sizeX = 0x01;
    player[0].color = 0x0f;
    player[0].lives = 0x01;
    // Terminate array
    player[1].whatIsThis = 0x00;

    // Ball setup
    tempVec.x = LONG_TO_EIGHTEEN_FOURTEEN(-1);
    tempVec.y = LONG_TO_EIGHTEEN_FOURTEEN(0);
    ball[0].whatIsThis = 0x02;
	ball[0].changedSinceLast = 1;
    ball[0].xs = LONG_TO_EIGHTEEN_FOURTEEN(80);
	ball[0].ys = LONG_TO_EIGHTEEN_FOURTEEN(25);
    ball[0].x1 = ball[0].xs;
    ball[0].y1 = ball[0].ys;
    ball[0].direction = tempVec;
    ball[0].color = 0x0A;
    ball[1].whatIsThis = 0x00;
	
	//Breakable setup
    breakable[n].whatIsThis = 0x03;
    breakable[n].changedSinceLast = 1;
    breakable[n].x1 = 120;
    breakable[n].y1 = 25;
    breakable[n].sizeX = 0x0A;
    breakable[n].sizeY = 0x05;
    breakable[n].lives = 1;
    breakable[n].powerUp = 0x02;
    n++;
	breakable[n].whatIsThis = 0x03;
    breakable[n].changedSinceLast = 1;
    breakable[n].x1 = 200;
    breakable[n].y1 = 25;
    breakable[n].sizeX = 0x0A;
    breakable[n].sizeY = 0x05;
    breakable[n].lives = 1;
    breakable[n].powerUp = 0x02;
    n++;
    breakable[n].whatIsThis = 0x00;
}