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
#include "levels.h"

#define LVLSIZE 100     // The size of the levels. Should probably be in engine.h
#define STDWINDOWX 50   // The standard placement for the upper left corner for the windows.
#define STDWINDOWY 21   // The standard placement for the upper left corner for the windows.


int startGame(char lvl) {
    player_t player[3];
    ball_t ball[5];
    int i, j,n;
    breakable_t breakable[LVLSIZE];
    Tvector tempVec[4];
    char button, whatDidTheyHit;
    int score = 0;
    int max_score = 0;
    char string[LED_MAX_STR_LEN] = " 5/5";
    int time1 = 1, time2 = 15;
	string[0] = 0x7F;

    //Choose level
    switch (lvl) {
        case 0x01:
            level1(breakable, ball, player);
            //Update with number of Tempvecs!!!
            initiateWithCeiling();
            for (i = 0; i < 4; i++) {
                tempVec[i] = ball[i].direction;
            }
            break;
        case 0x02:
            level2(breakable, ball, player);
            initiateWithoutCeiling();
            //Update with number of Tempvecs!!!
            for (i = 0; i < 4; i++) {
                tempVec[i] = ball[i].direction;
            }
            break;
        case 0x03:
            level3(breakable, ball, player);
            initiateWithCeiling();
            //Update with number of Tempvecs!!!
            for (i = 0; i < 4; i++) {
                tempVec[i] = ball[i].direction;
            }
            break;
        default:
            player[0].lives = 0;
    }

    string[1] = player[0].lives + 0x30;
    string[3] = player[0].lives + 0x30;

    max_score = findMaxScore(breakable);


    timer1Setup();
    LEDSetString(string);
    drawMap(player, ball, breakable);
    gotoxy(5,62);
    printf("score:");
    gotoxy(5,63);
    printf("out of:   %d", max_score);

    do {
        do {
            do {
				//reset timer
				if (timer1() < 50) {
            		time1 = 1;
            		time2 = 15;
        		}
                LEDUpdate();
                //Do this for 0.1 s
            } while (timer1() < time1);
            button = readKey();
            //Then move the player
            playerMovement(button, player);
            LEDUpdate();
            //Reenter the above while - loop
            time1=timer1();
            //Do this for 0.5 s
        } while (timer1() < time2);
        //Then move the ball

        whatDidTheyHit = ballMovement(ball, player, breakable, lvl);
        if (string[1]-0x30 != player[0].lives) {
            string[1] = player[0].lives+0x30;
            LEDSetString(string);
        }
        //Checks if it hit anything, and if so, how much
        if (whatDidTheyHit & 0x0F) {
            score += (whatDidTheyHit & 0x0F);
            gotoxy(15, 62);
                printf("%d", score);
            if (score >= max_score) {
                    player[0].lives = 0;
                }
        }
        //Checks if any of them died
        if (whatDidTheyHit & 0xF0) {
            player[0].lives -= (whatDidTheyHit & 0x10)>>4;
            player[0].lives -= (whatDidTheyHit & 0x20)>>5;
            player[0].lives -= (whatDidTheyHit & 0x40)>>6;
            player[0].lives -= (whatDidTheyHit & 0x80)>>7;
            for (j = 0; ball[j].whatIsThis != 0x00; j++) {
                gotoxy(EIGHTEEN_FOURTEEN_TO_INT(ball[j].x1), EIGHTEEN_FOURTEEN_TO_INT(ball[j].y1));
                printf(" ");
            }
            if (whatDidTheyHit & 0x10) {
                ball[0].direction = tempVec[0];
                ball[0].x1 = ball[0].xs;
                ball[0].y1 = ball[0].ys;
            }
            if (whatDidTheyHit & 0x20) {
                ball[1].direction = tempVec[1];
                ball[1].x1 = ball[1].xs;
                ball[1].y1 = ball[1].ys;
            }
            if (whatDidTheyHit & 0x40) {
                ball[2].direction = tempVec[2];
                ball[2].x1 = ball[2].xs;
                ball[2].y1 = ball[2].ys;
            }
            if (whatDidTheyHit & 0x80) {
                ball[3].direction = tempVec[3];
                ball[3].x1 = ball[3].xs;
                ball[3].y1 = ball[3].ys;
            }
            LEDSetString(string);
        }

        LEDUpdate();

        //Reenter above loop
        // 15 can be changed to a variable, to increase difficulty
        time2 = timer1() + 15;

    } while (player[0].lives);
    return score;
}

void addHighscore(int * score, int * highscore){
    int i;
    int positionoflowest = 0;
    for(i = 1; i <= 5; i++){
        if(highscore[positionoflowest] > highscore[i]){
            positionoflowest = i;  
        }
    }
    if (*score > highscore[positionoflowest]){
        highscore[positionoflowest] = *score;
        gotoxy(20,62);
        printf("You made the highscore");
    }
}

void printHighscore(int * highscore){
    // bubble sort
    int i, j, temp, flag = 1;
    for(i = 0; i < 4; i++){
        for (j = 0; j < 4-i; j++){
            if(highscore[j] < highscore[j+1]){
                temp = highscore[j+1];
                highscore[j+1] = highscore[j];
                highscore[j] = temp; 
            }
        }     
    }
    window(10, 10,STDWINDOWX, STDWINDOWY, " Highscore ", 1);
    for (i = 0; i < 5; i++){
        gotoxy(12,12 +i);
        printf("%d.  %d", i+1, highscore[i]);
    }
    do{
        LEDUpdate();
        if (readKey() == 0x00){
            flag = 0;
        }
    }while(!readKey() || (flag));
    for (i = 0; i >=23; i++){
         gotoxy(20+i,62);
         printf(" ");
    }
}

void showControls() {
    int i, flag = 1;
    window(10, 10,STDWINDOWX, STDWINDOWY, " Help! ", 1);
    gotoxy(12, 12);
    printf("You have three buttons:");
    gotoxy(12, 13);
    printf(" < - Left   ^ - Action  > - Right\r\n");
    gotoxy(12, 15);
    printf("Hit these:   With this:");
    gotoxy(12, 17);
    fgcolor(5);
    for (i = 0; i < 8; i++) {
        printf("%c", BREAKABLETEXTURE);
    }
    gotoxy(12, 18);
    for (i = 0; i < 8; i++) {
        printf("%c", BREAKABLETEXTURE);
    }
    gotoxy(30, 17);
    fgcolor(0x0a);
    printf("%c", BALLTEXTURE);
    fgcolor(STDTEXTCOLOR);
    gotoxy(12, 20);
    printf("You have %d lives!", 5);
    do {
        LEDUpdate();
        if (readKey() == 0x00) {
            flag = 0;
        }
    } while (!readKey() || (flag));
}

int lvlMenu() {
    char exit_flag = 1, flag1 = 1, flag2 = 0, output = 0, button = 0x00;
    int score = 0;

    do {
        window(10, 10,STDWINDOWX, STDWINDOWY, " Choose level ", 1);
        gotoxy(12, 12);
        printf(" > Level 1");
        gotoxy(13, 13);
        printf("  Level 2");
        gotoxy(13, 14);
        printf("  Level 3");
        gotoxy(13, 15);
        printf("  Go back");
        do {
            LEDUpdate();
            button = readKey();
            if ((button != 0x00) && (!flag2)) {
                switch (button) {
                    case 0x01 :
                        output += 1;
                        if (output <= 3 && output >= 0) {
                            gotoxy(13, 12 + output - 1);
                            printf(" ");
                            gotoxy(13, 12 + output);
                            printf(">");
                            gotoxy(13, 12);
                        } else {
                            output -= 1;
                            gotoxy(13, 12 + output - 1);
                            printf(" ");
                            gotoxy(13, 12 + output);
                            printf(">");
                            gotoxy(13, 12);
                        }
                        break;
                    case 0x04 :
                        output -= 1;
                        if (output <= 3 && output >= 0) {
                            gotoxy(13, 12 + output + 1);
                            printf(" ");
                            gotoxy(13, 12 + output);
                            printf(">");
                            gotoxy(13, 12);
                        } else {
                            output += 1;
                            gotoxy(13, 12 + output - 1);
                            printf(" ");
                            gotoxy(13, 12 + output);
                            printf(">");
                            gotoxy(13, 12);
                        }
                        break;
                    case 0x02 :
                        flag1 = 0;
                        gotoxy(13, 12);
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
                score = startGame(0x01);
                exit_flag = 0;
                break;
            case 1:
                clrscr();
                score = startGame(0x02);
                exit_flag = 0;
                break;
            case 2:
                clrscr();
                score = startGame(0x03);
                exit_flag = 0;
                break;
            case 3:
                clrscr();
                exit_flag = 0;
                break;
        }
        output = 0;
        flag1 = 1;
    } while (exit_flag);
    return score;
}

void main() {
    int highscore[5] = {0,0,0,0,0};
    int score;
    char button, flag1 = 1, flag2 = 0, output = 0;
    char string[LED_MAX_STR_LEN] = "Hello";
    init_uart(_UART0, _DEFFREQ, _DEFBAUD);  // set-up UART0 to 57600, 8n1
    clrscr();
    LEDInit();
    LEDSetString(string);

    do {
        fgcolor(STDTEXTCOLOR);
        clrscr();
        window(10, 10, STDWINDOWX, STDWINDOWY, " Main menu ", 1);
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
                score = lvlMenu();
                addHighscore(&score, highscore);
                printHighscore(highscore);
                break;
            case 1:
                string[0] = 'N';
                string[1] = 'o';
                string[2] = 'p';
                string[3] = 'e';
				printf("case 1");
                LEDSetString(string);
                showControls();
                break;
            case 2:
                string[0] = 'N';
                string[1] = 'o';
                string[2] = 'p';
                string[3] = 'e';
                printf("case 2");
				LEDSetString(string);
                printHighscore(highscore);
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

