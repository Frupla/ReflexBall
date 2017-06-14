//
// Created by asger on 13-06-2017.
//
#include <eZ8.h>                          // special encore constants, macros and flash functions
#include <sio.h>                          // special encore serial i/o functions
#include "ansi.h"
#include "sinLUT.h"
#include "math.h"
#include "Z8encore.h"
#include "engine.h"
#include "standalone_timer.h"

void startGame() {
    entity map[7];
    int i, j, n = 0;
    Tvector tempVec;
    char button;
    char string[LED_MAX_STR_LEN] = "<3 3";
    int time1 = 1, time2 = 5;

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
    map[n].x1 = LONG_TO_EIGHTEEN_FOURTEEN(50);
    map[n].y1 = LONG_TO_EIGHTEEN_FOURTEEN(61);
    map[n].direction = tempVec;
    map[n].sizeX = 0x00;
    map[n].sizeY = 0x00;
    map[n].color = 0x00;
    n++;

    //Breakable setup
    map[n].whatIsThis = 0x03;
    map[n].changedSinceLast = 1;
    map[n].x1 = LONG_TO_EIGHTEEN_FOURTEEN(5);
    map[n].y1 = LONG_TO_EIGHTEEN_FOURTEEN(5);
    map[n].sizeX = 0x16;
    map[n].sizeY = 0x01;
    map[n].color = 0x03;
    n++;

    map[n].whatIsThis = 0x03;
    map[n].changedSinceLast = 1;
    map[n].x1 = LONG_TO_EIGHTEEN_FOURTEEN(5);
    map[n].y1 = LONG_TO_EIGHTEEN_FOURTEEN(20);
    map[n].sizeX = 0x16;
    map[n].sizeY = 0x01;
    map[n].color = 0x02;
    n++;

    map[n].whatIsThis = 0x03;
    map[n].changedSinceLast = 1;
    map[n].x1 = LONG_TO_EIGHTEEN_FOURTEEN(5);
    map[n].y1 = LONG_TO_EIGHTEEN_FOURTEEN(35);
    map[n].sizeX = 0x16;
    map[n].sizeY = 0x01;
    map[n].color = 0x01;
    n++;

    map[n].whatIsThis = 0x00;
    n++;


    initiate();
    timer1Setup();
    LEDInit();
    LEDSetString(string);
    drawMap(map);

    //n counts the health
    n = 3;
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
        switch (ballMovement(map)) {
            case 0x00: //Nothing
                break;
            case 0x01: //Hit breakable
                break;
            case 0x02: //Ball dead
                n--;
                gotoxy(map[1].x1-1, map[1].y1);
                printf("H");
                map[1].direction = tempVec;
                map[1].x1 = LONG_TO_EIGHTEEN_FOURTEEN(50);
                map[1].y1 = LONG_TO_EIGHTEEN_FOURTEEN(61);
                string[3] = n + 48;
                LEDSetString(string);
                break;
            case 0x03: //Hit paddle
                break;
            default:
                break;
        }

        LEDUpdate();

        //Reenter above loop
        time2 += 5;

        //Reset timers
        if (timer1() < 20) {
            time1 = 1;
            time2 = 5;
        }
    } while (n);
}


void main() {
    char button, flag1 = 1, flag2 = 0, output = 0;
    char string[LED_MAX_STR_LEN] = "Hello";
    init_uart(_UART0, _DEFFREQ, _DEFBAUD);  // set-up UART0 to 57600, 8n1
    clrscr();
    LEDSetString(string);

    do {
        window(10, 10, 50, 20, "Main menu", 1);
        gotoxy(12, 12);
        printf(" > Start game");
		gotoxy(13,13);
		printf("  Show controls");
		gotoxy(13,14);
		printf("  Show high scores");
		gotoxy(13,15);
		printf("  Exit game");
        do {
            LEDUpdate();
            button = readKey();
            if ((button != 0x00) && (!flag2)) {
                switch (button) {
                    case 0x01 :
						output += 1;
						if(output <= 3 && output >= 0){
                        	gotoxy(13, 12 + output - 1);
                        	printf(" ");
                        	gotoxy(13, 12 + output);
                        	printf(">");
							gotoxy(13,12);
					   	} else{
							output -= 1;
							gotoxy(13, 12 + output - 1);
                        	printf(" ");
                        	gotoxy(13, 12 + output);
                        	printf(">");
							gotoxy(13,12);
						}
						break;
                    case 0x04 :
						output -= 1;
						if(output <= 3 && output >= 0){
							gotoxy(13, 12 + output + 1);
                        	printf(" ");
                        	gotoxy(13, 12 + output);
                        	printf(">");
							gotoxy(13,12);
						} else{
							output += 1;
							gotoxy(13, 12 + output - 1);
                        	printf(" ");
                        	gotoxy(13, 12 + output);
                        	printf(">");
							gotoxy(13,12);
						}
                        break;
                    case 0x02 :
                        flag1 = 0;
						gotoxy(13,12);
                        break;
                }
                flag2 = 1;
            } else if (flag2 && (button == 0x00)) {
                flag2 = 0;
            }
        } while (flag1);

        switch (output) {
            case 0:
                clrscr();
                startGame();
                break;
            case 1:
                string[0] = 'N';
                string[1] = 'o';
                string[2] = 'p';
                string[3] = 'e';
				printf("case 1");
                LEDSetString(string);
                break;
            case 2:
                string[0] = 'N';
                string[1] = 'o';
                string[2] = 'p';
                string[3] = 'e';
                printf("case 2");
				LEDSetString(string);
                break;
            case 3:
                string[0] = 'N';
                string[1] = 'o';
                string[2] = 'p';
                string[3] = 'e';
                printf("case 3");
				LEDSetString(string);
                break;
        }
		output = 0;
        flag1 = 1;
    } while (1);


}

