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
    entity map[11];
    long i, j;
    int n = 0;
    Tvector tempVec;
	char button;
    int time1 = 1, time2 = 5;
	init_uart(_UART0,_DEFFREQ,_DEFBAUD);  // set-up UART0 to 57600, 8n1
	clrscr();
	// player setup
    map[n].whatIsThis = 0x01;
    map[n].changedSinceLast = 1;
    map[n].x1 = LONG_TO_EIGHTEEN_FOURTEEN(50);
    map[n].y1 = LONG_TO_EIGHTEEN_FOURTEEN(62);
    //map[n].direction = {0, 0};
    map[n].sizeX = 0x04;
	map[n].sizeY = 0;
    map[n].color = 0x00;
    n++;

    // Ball setup
    tempVec.x = convert(0);
    tempVec.y = convert(-1);
    rotate(&tempVec, 47);
    map[n].whatIsThis = 0x02;
    map[n].changedSinceLast = 1;
    map[n].x1 = LONG_TO_EIGHTEEN_FOURTEEN(7);
    map[n].y1 = LONG_TO_EIGHTEEN_FOURTEEN(10);
    map[n].direction = tempVec;
    map[n].sizeX = 0x00;
	map[n].sizeY = 0x00;
    map[n].color = 0x00;
    n++;

    //Breakable setup
    for(i = 10; i <= 90; i += 40){
        for(j = 5; j <= 15; j += 5){
            map[n].whatIsThis = 0x03;
            map[n].changedSinceLast = 1;
            map[n].x1 = LONG_TO_EIGHTEEN_FOURTEEN(i);
            map[n].y1 = LONG_TO_EIGHTEEN_FOURTEEN(j);
            map[n].sizeX = 0x16;
            map[n].sizeY = 0x01;
            map[n].color = 0x03;
            n++;
        }
    }

    map[n].whatIsThis = 0x00;
    n++;


	initiate();
    timer1Setup();
	LEDInit();
    LEDSetString(string);
    drawMap(map);
    do {
        do {
            do {
                button = readKey();
                LEDUpdate();
                //Do this for 0.1 s
            } while (timer1() < time1);
            //Then move the player
            playerMovement(button, map);

            LEDUpdate();
            //Reenter the above while - loop
            time1++;
            //Do this for 0.5 s
        } while (timer1() < time2);
        //Then move the ball
        ballMovement(map);

        LEDUpdate();

        //Reenter above loop
        time2 += 5;

        //Reset timers
        if (timer1() < 20) {
            time1 = 1;
            time2 = 5;
        }
	} while (1);

}

