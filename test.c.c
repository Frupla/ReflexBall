//
// Created by asger on 12-06-2017.
//
#include <eZ8.h>                          // special encore constants, macros and flash functions
#include <sio.h>                          // special encore serial i/o functions
#include "ansi.h"
#include "sinLUT.h"
#include "math.h"
#include "Z8encore.h"
#include "engine.h"
#include "standalone_timer.h"

rom char string[LED_MAX_STR_LEN] = "Shit";

void main() {
    player_t player[2];
    ball_t ball[2];
    breakable_t breakable[3];
    int i, j;
    int n = 0;
    Tvector tempVec;
	char button;
    int time1 = 1, time2 = 15;
	init_uart(_UART0,_DEFFREQ,_DEFBAUD);  // set-up UART0 to 57600, 8n1
	clrscr();
	// player setup
    player[0].whatIsThis = 0x01;
    player[0].changedSinceLast = 1;
    player[0].x1 = 50;
    player[0].y1 = 62;
    //map[n].direction = {0, 0};
    player[0].sizeX = 0x04;
    player[0].color = 0x00;

    // Ball setup
    tempVec.x = convert(0);
    tempVec.y = convert(-1);
    rotate(&tempVec, 47);
    ball[0].whatIsThis = 0x02;
    ball[0].changedSinceLast = 1;
    ball[0].x1 = LONG_TO_EIGHTEEN_FOURTEEN(7);
    ball[0].y1 = LONG_TO_EIGHTEEN_FOURTEEN(10);
    ball[0].direction = tempVec;
    ball[0].size = 0x00;
    ball[0].color = 0x00;
	
	i = 5;
	j = 5;
    //Breakable setup
    for(i = 10; i <= 90; i += 40){
    	for(j = 4; j <= 8; j += 4){
            breakable[n].whatIsThis = 0x03;
            breakable[n].changedSinceLast = 1;
            breakable[n].x1 = i;
            breakable[n].y1 = j;
            breakable[n].sizeX = 0x16;
            breakable[n].sizeY = 0x01;
            breakable[n].lives = 0x03;
            n++;
       	}
    }

    breakable[n].whatIsThis = 0x00;
    ball[1].whatIsThis = 0x00;
    player[1].whatIsThis = 0x00;
    n++;


	initiate();
    timer1Setup();
	LEDInit();
    LEDSetString(string);
    drawMap(player, ball, breakable);
    do {
        do {
            do {
                button = readKey();
                LEDUpdate();
                //Do this for 0.1 s
            } while (timer1() < time1);
            //Then move the player
            playerMovement(button, player);

            LEDUpdate();
            //Reenter the above while - loop
            time1++;
            //Do this for 0.5 s
        } while (timer1() < time2);
        //Then move the ball
        ballMovement(ball, player, breakable);

        LEDUpdate();

        //Reenter above loop
        time2 += 15;

        //Reset timers
        if (timer1() < 20) {
            time1 = 1;
            time2 = 15;
        }
	} while (1);

}

