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
    player[0].lives = 0x04;
    // Terminate array
    player[1].whatIsThis = 0x00;

    // Ball setup
    tempVec.x = convert(-1);
    tempVec.y = convert(0);
    rotate(&tempVec, -64);
    //Ball 1
    ball[0].whatIsThis = 0x02;
    ball[0].changedSinceLast = 1;
    ball[0].xs = LONG_TO_EIGHTEEN_FOURTEEN((long)40);
    ball[0].ys = LONG_TO_EIGHTEEN_FOURTEEN((long)25);
    ball[0].x1 = ball[0].xs;
    ball[0].y1 = ball[0].ys;
    ball[0].direction = tempVec;
    ball[0].color = 0x0A;
    //Ball 2
    ball[1].whatIsThis = 0x02;
    ball[1].changedSinceLast = 1;
    ball[1].xs = LONG_TO_EIGHTEEN_FOURTEEN((long)45);
    ball[1].ys = LONG_TO_EIGHTEEN_FOURTEEN((long)25);
    ball[1].x1 = ball[1].xs;
    ball[1].y1 = ball[1].ys;
    ball[1].direction = tempVec;
    ball[1].color = 0xB;
    //Ball 3
    ball[2].whatIsThis = 0x02;
    ball[2].changedSinceLast = 1;
    ball[2].xs = LONG_TO_EIGHTEEN_FOURTEEN((long)50);
    ball[2].ys = LONG_TO_EIGHTEEN_FOURTEEN((long)25);
    ball[2].x1 = ball[2].xs;
    ball[2].y1 = ball[2].ys;
    ball[2].direction = tempVec;
    ball[2].color = 0xC;
    //Ball 4
    ball[3].whatIsThis = 0x02;
    ball[3].changedSinceLast = 1;
    ball[3].xs = LONG_TO_EIGHTEEN_FOURTEEN((long)55);
    ball[3].ys = LONG_TO_EIGHTEEN_FOURTEEN((long)25);
    ball[3].x1 = ball[3].xs;
    ball[3].y1 = ball[3].ys;
    ball[3].direction = tempVec;
    ball[3].color = 0xD;
    
    ball[4].whatIsThis = 0x00;

 	//Breakable setup
    for (i = 2; i <= 119 ; i += 13) {
        for (j = 8; j <= 16; j += 2) {
            breakable[n].whatIsThis = 0x03;
            breakable[n].changedSinceLast = 1;
            breakable[n].x1 = i;
            breakable[n].y1 = j;
            breakable[n].sizeX = 0x0C;
            breakable[n].sizeY = 0x01;
			if(flag >= 2){
            	breakable[n].lives = 0x03;
				flag = 0;
			} else{
				breakable[n].lives = 0x02;
			}
            if(j >= 8 && j <= 10 && i != 119){
                breakable[n].powerUp = 0x02;
            } else{
                breakable[n].powerUp = 0x00;
            }
			flag++;
            n++;
       }
    }
	for (i = 119; i <= 225 ; i += 13) {
        for (j = 26; j <= 	34; j += 2) {
            breakable[n].whatIsThis = 0x03;
            breakable[n].changedSinceLast = 1;
            breakable[n].x1 = i;
            breakable[n].y1 = j;
            breakable[n].sizeX = 0x0C;
            breakable[n].sizeY = 0x01;
			if(flag >= 2){
            	breakable[n].lives = 0x03;
				flag = 0;
			} else{
				breakable[n].lives = 0x02;
            }
			flag++;
            if(j >= 26 && j <= 28 && i != 119){
                breakable[n].powerUp = 0x01;
            } else{
                breakable[n].powerUp = 0x00;
            }
            n++;
       }
    }
    breakable[n].whatIsThis = 0x00;
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
    player[0].lives = 0x04;
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
    tempVec.x = convert(-1);
    tempVec.y = convert(0);
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
            breakable[n].y1 = 5 * j;
            breakable[n].sizeX = 0x0A;
            breakable[n].sizeY = 0x01;
            breakable[n].lives = 5 - j / 2;
            breakable[n].powerUp = 0x02;
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
            breakable[n].powerUp = 0x01;
            n++;
        }
    }
    breakable[0].powerUp = 0x01;
    breakable[n-1].powerUp = 0x01;
    breakable[n].whatIsThis = 0x00;
}

void level3(breakable_t * breakable, ball_t * ball, player_t * player){
    int i, j, n=0;
	Tvector tempVec;
    // player setup
    player[0].whatIsThis = 0x01;
    player[0].changedSinceLast = 1;
    player[0].x1 = 50;
    player[0].y1 = 57;
    player[0].sizeX = 0x01;
    player[0].color = 0x0f;
    player[0].lives = 0x04;
    // Terminate array
    player[1].whatIsThis = 0x00;

    // Ball setup
    tempVec.x = convert(1);
    tempVec.y = convert(0);
	for(i = 0; i < 3; i++){
		ball[i].whatIsThis = 0x02;
	    ball[i].changedSinceLast = 1;
	    ball[0].ys = LONG_TO_EIGHTEEN_FOURTEEN(25);
		ball[1].ys = LONG_TO_EIGHTEEN_FOURTEEN(26);
		ball[2].ys = LONG_TO_EIGHTEEN_FOURTEEN(27);
	    ball[i].xs = LONG_TO_EIGHTEEN_FOURTEEN(100);
	    ball[i].x1 = ball[i].xs;
	    ball[i].y1 = ball[i].ys;
	    ball[i].direction = tempVec;
    	ball[i].color = 0x0A;
	}
	ball[3].whatIsThis = 0x02;
	ball[3].changedSinceLast = 1;
	ball[3].xs = LONG_TO_EIGHTEEN_FOURTEEN(100);
	ball[3].ys = LONG_TO_EIGHTEEN_FOURTEEN(10);
	ball[3].x1 = ball[3].xs;
	ball[3].y1 = ball[3].ys;
	rotate(&tempVec, 64);
	ball[3].direction = tempVec;
    ball[3].color = 0x0A;
	ball[4].whatIsThis = 0x00;

    breakable[n].whatIsThis = 0x03;
    breakable[n].changedSinceLast = 1;
    breakable[n].x1 = 115;
    breakable[n].y1 = 25;
    breakable[n].sizeX = 0x03;
    breakable[n].sizeY = 0x02;
    breakable[n].lives = 0x06;
    breakable[n].powerUp = 0x00;
    n++;
    breakable[n].whatIsThis = 0x00;
}