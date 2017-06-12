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

rom char string[LED_MAX_STR_LEN] = "BREAKOUT";

void main() {
    entity map[6];
    int i, j, n = 0;
    Tvector tempVec;
	char button;
    int time1 = 100, time2 = 499;
	init_uart(_UART0,_DEFFREQ,_DEFBAUD);  // set-up UART0 to 57600, 8n1
	clrscr();
	// player setup
    map[n].whatIsThis = 0x01;
    map[n].changedSinceLast = 1;
    map[n].x1 = 50;
    map[n].y1 = 99;
    //map[n].direction = {0, 0};
    map[n].sizeX = 0x16;
	map[n].sizeY = 0;
    map[n].color = 0x00;
    n++;

    // Ball setup
    tempVec.x = convert(0);
    tempVec.y = convert(-1);
	rotate(&tempVec, 64);
    map[n].whatIsThis = 0x02;
    map[n].changedSinceLast = 1;
    map[n].x1 = 60;
    map[n].y1 = 50;
    map[n].direction = tempVec;
    map[n].sizeX = 0x11;
	map[n].sizeY = 0;
    map[n].color = 0x00;
    n++;

    //Breakable setup
    map[n].whatIsThis = 0x03;
    map[n].changedSinceLast = 1;
    map[n].x1 = 5;
    map[n].y1 = 5;
    //map[n].direction = {0, 0};
    map[n].sizeX = 0x16;
	map[n].sizeY = 0;
    map[n].color = 0x00;
    n++;
    map[n].whatIsThis = 0x00;
    n++;
	initiate();
	LEDInit();
    LEDSetString(string);

    do {
        do {
            drawMap(map);
            do {
                //Determine where to move
                button = readKey();

                //Remember to do this a lot
                LEDUpdate();
            } while (readMsec() < time1);
            //Then move the player
            playerMovement(button, map);

            time1 += 50;

        } while (readMsec() < time2);
        //Then move the ball
        ballMovement(map);
        rotate(&(map[1].direction), 16);

        LEDUpdate();
        time2 += 500;
        if (readMsec() < 30) {
            time1 = 50;
            time2 = 499;
        }
	} while (1);
}

