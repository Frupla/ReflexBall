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

int startGame() {
    player_t player[2];
    ball_t ball[2];
    breakable_t breakable[60];
    int i, j, n = 0;
    Tvector tempVec;
    char button;
    int score = 0, max_score = 10;
    char string[LED_MAX_STR_LEN] = "<3 3";
    int time1 = 1, time2 = 15;

    // player setup
    player[0].whatIsThis = 0x01;
    player[0].changedSinceLast = 1;
    player[0].x1 = 50;
    player[0].y1 = 58;
    //map[n].direction = {0, 0};
    player[0].sizeX = 0x04;
    player[0].color = 0x0f;

    // Ball setup
    tempVec.x = convert(-1);
    tempVec.y = convert(0);
    rotate(&tempVec, 47);
    ball[0].whatIsThis = 0x02;
    ball[0].changedSinceLast = 1;
    ball[0].x1 = LONG_TO_EIGHTEEN_FOURTEEN(50);
    ball[0].y1 = LONG_TO_EIGHTEEN_FOURTEEN(50);
    ball[0].direction = tempVec;
    ball[0].size = 0x00;
    ball[0].color = 0x0A;

    i = 5;
    j = 5;
    //Breakable setup
    for (i = 5; i <= 105 ; i += 15) {
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
    ball[1].whatIsThis = 0x00;
    player[1].whatIsThis = 0x00;
    n++;


    initiate();
    timer1Setup();
    LEDInit();
    LEDSetString(string);
    drawMap(player, ball, breakable);

    //n counts the health
    n = 5;
    do {
        do {
            do {
				//reset timer
				if (timer1() < 50) {
            		time1 = 1;
            		time2 = 15;
        		}
                button = readKey();
                gotoxy(5,62);
                printf("%d\n", score);
                LEDUpdate();
                //Do this for 0.1 s
            } while (timer1() < time1);
            //Then move the player
            playerMovement(button, player);

            LEDUpdate();
            //Reenter the above while - loop
            time1=timer1();
            //Do this for 0.5 s
        } while (timer1() < time2);
        //Then move the ball
        switch (ballMovement(ball, player, breakable)) {
            case 0x00: //Nothing
                break;
            case 0x01: //Hit breakable
                score++;
                if(score >= max_score){
                    n = 0;
                }
                break;
            case 0x02: //Ball dead
                n--;
                gotoxy(EIGHTEEN_FOURTEEN_TO_INT(ball[0].x1), EIGHTEEN_FOURTEEN_TO_INT(ball[0].y1));
                printf(" ");
                ball[0].direction = tempVec;
                ball[0].x1 = LONG_TO_EIGHTEEN_FOURTEEN(50);
                ball[0].y1 = LONG_TO_EIGHTEEN_FOURTEEN(50);
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
        // 15 can be changed to a variable, to increase difficulty
        time2 = timer1() + 15;

    } while (n);
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
    highscore[positionoflowest] = *score;
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
    window(10, 10, 50, 20, "Highscore", 1);
    for (i = 0; i < 5; i++){
        gotoxy(12,12 +i);
        printf("%d.  %d", i+1, highscore[i]);
    }
    do{
        if (readKey() == 0x00){
            flag = 0;
        }
    }while(!readKey() || (flag));
}


void main() {
    int highscore[5] = {0,0,0,0,0};
    int score;
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
                score = startGame();
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

