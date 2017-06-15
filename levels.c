#include <eZ8.h>  						  // special encore constants, macros and flash functions
#include <sio.h>	                      // special encore serial i/o functions
#include "engine.h"
#include "ansi.h"

void level1(breakable_t* breakable, ball_t * ball){
	int i, j, n=0, flag = 2;
	Tvector tempVec;
    // Ball setup
    tempVec.x = convert(-1);
    tempVec.y = convert(0);
    rotate(&tempVec, -47);
    ball[0].whatIsThis = 0x02;
    ball[0].changedSinceLast = 1;
    ball[0].xs = LONG_TO_EIGHTEEN_FOURTEEN(50);
    ball[0].ys = LONG_TO_EIGHTEEN_FOURTEEN(25);
    ball[0].x1 = ball[0].xs;
    ball[0].y1 = ball[0].ys;
    ball[0].direction = tempVec;
    ball[0].size = 0x00;
    ball[0].color = 0x0A;
    ball[1].whatIsThis = 0x00;

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
            n++;
       }
    }
    breakable[n].whatIsThis = 0x00;
}

void level2(breakable_t * breakable, ball_t * ball){
    int i, j, n=0;
	Tvector tempVec;
    // Ball setup
    tempVec.x = convert(-1);
    tempVec.y = convert(0);
    rotate(&tempVec, 64);
    ball[0].whatIsThis = 0x02;
    ball[0].changedSinceLast = 1;
    ball[0].xs = LONG_TO_EIGHTEEN_FOURTEEN(182);
    ball[0].ys = LONG_TO_EIGHTEEN_FOURTEEN(5);
    ball[0].x1 = ball[0].xs;
    ball[0].y1 = ball[0].ys;
    ball[0].direction = tempVec;
    ball[0].size = 0x00;
    ball[0].color = 0x0A;
    ball[1].whatIsThis = 0x00;
    //Breakable setup
    for (i = 1; i <= 9 ; i++) {
        for (j = 1; j <= 9-i; j++) {
            breakable[n].whatIsThis = 0x03;
            breakable[n].changedSinceLast = 1;
            breakable[n].x1 = 20*i;
            breakable[n].y1 = 5*j;
            breakable[n].sizeX = 0x0A;
            breakable[n].sizeY = 0x01;
            breakable[n].lives = 5 - j/2;
            n++;
       }
    }
	    for (i = 9; i >= 1 ; i--) {
        for (j = 9; j > 9-i; j--) {
            breakable[n].whatIsThis = 0x03;
            breakable[n].changedSinceLast = 1;
            breakable[n].x1 = 20*i + 20;
            breakable[n].y1 = 5*j + 5;
            breakable[n].sizeX = 0x0A;
            breakable[n].sizeY = 0x01;
            breakable[n].lives = 5 - j/2;
            n++;
       }
    }
    breakable[n].whatIsThis = 0x00;
}

void level3(breakable_t * breakable, ball_t * ball){
    int i, j, n=0;
	Tvector tempVec;
    // Ball setup
    tempVec.x = convert(-1);
    tempVec.y = convert(0);
    rotate(&tempVec, -47);
    ball[0].whatIsThis = 0x02;
    ball[0].changedSinceLast = 1;
    ball[0].xs = LONG_TO_EIGHTEEN_FOURTEEN(50);
    ball[0].ys = LONG_TO_EIGHTEEN_FOURTEEN(25);
    ball[0].x1 = ball[0].xs;
    ball[0].y1 = ball[0].ys;
    ball[0].direction = tempVec;
    ball[0].size = 0x00;
    ball[0].color = 0x0A;
    ball[1].whatIsThis = 0x00;
    //Breakable setup
    for (i = 15; i <= 105 ; i += 15) {
        for (j = 4; j <= 20; j += 4) {
            breakable[n].whatIsThis = 0x03;
            breakable[n].changedSinceLast = 1;
            breakable[n].x1 = i;
            breakable[n].y1 = j;
            breakable[n].sizeX = 0x0A;
            breakable[n].sizeY = 0x01;
            breakable[n].lives = 0x03;
            n++;
       }
    }
    breakable[2].lives = 1;
    breakable[4].lives = 1;
    breakable[6].lives = 1;
    breakable[10].lives = 2;
    breakable[14].lives = 2;
    breakable[12].lives = 4;
    breakable[n].whatIsThis = 0x00;
}