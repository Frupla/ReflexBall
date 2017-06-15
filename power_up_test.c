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

#define LVLSIZE 100
#define STDWINDOWX 10
#define STDWINDOWY 10

int findMaxScore(breakable_t* breakables){
    int max_score = 0;
    while(breakables->whatIsThis){
        max_score += breakables->lives;
        breakables++;
    }
    return max_score;
}

int startGame(char lvl) {
    player_t player;
    ball_t ball[2];
    int i, j;
    breakable_t breakable[LVLSIZE];
    Tvector tempVec;
    char button;
    int score = 0;
    int max_score = 0;
    char string[LED_MAX_STR_LEN] = " 5/5";
    int time1 = 1, time2 = 15;
    string[0] = 0x7F;

    // player setup
    player.whatIsThis = 0x01;
    player.changedSinceLast = 1;
    player.x1 = 50;
    player.y1 = 57;
    //map[n].direction = {0, 0};
    player.sizeX = 0x04;
    player.color = 0x0f;
    player.lives = 4;

    // Ball setup
    tempVec.x = convert(-1);
    tempVec.y = convert(0);
    rotate(&tempVec, -47);
    ball[0].whatIsThis = 0x02;
    ball[0].changedSinceLast = 1;
    ball[0].x1 = LONG_TO_EIGHTEEN_FOURTEEN(50);
    ball[0].y1 = LONG_TO_EIGHTEEN_FOURTEEN(25);
    ball[0].direction = tempVec;
    ball[0].size = 0x00;
    ball[0].color = 0x0A;
    ball[1].whatIsThis = 0x00;

    //n counts the health
    string[1] = player.lives + 0x30;
    string[3] = player.lives + 0x30;

    //Choose level
    switch (lvl) {
        case 0x01:
            level1(breakable);
            break;
        case 0x02:
            level2(breakable);
            break;
        case 0x03:
            level3(breakable);
            break;
        default:
            player.lives = 0;
    }


    max_score = findMaxScore(breakable);

    initiate();
    timer1Setup();
    LEDSetString(string);
    drawMap(&player, ball, breakable);
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
                button = readKey();
                LEDUpdate();
                //Do this for 0.1 s
            } while (timer1() < time1);
            //Then move the player
            playerMovement(button, &player);

            LEDUpdate();
            //Reenter the above while - loop
            time1=timer1();
            //Do this for 0.5 s
        } while (timer1() < time2);
        //Then move the ball
        switch (ballMovement(ball, &player, breakable)) {
            case 0x00: //Nothing
                break;
            case 0x01: //Hit breakable
                score++;
                gotoxy(15,62);
                printf("%d", score);
                if(score >= max_score){
                    player.lives = 0;
                }
                break;
            case 0x02: //Ball dead
                player.lives--;
                gotoxy(EIGHTEEN_FOURTEEN_TO_INT(ball[0].x1), EIGHTEEN_FOURTEEN_TO_INT(ball[0].y1));
                printf(" ");
                ball[0].direction = tempVec;
                ball[0].x1 = LONG_TO_EIGHTEEN_FOURTEEN(50);
                ball[0].y1 = LONG_TO_EIGHTEEN_FOURTEEN(25);
                string[3] = player.lives + 48;
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

    } while (player.lives);
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
    window(STDWINDOWX, STDWINDOWY, 50, 21, "Highscore", 1);
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
    window(STDWINDOWX, STDWINDOWY, 50, 21, "Help!", 1);
    gotoxy(STDWINDOWX + 2, STDWINDOWY + 2);
    printf("You have three buttons:");
    gotoxy(STDWINDOWX + 2, STDWINDOWY + 3);
    printf(" < - Left   ^ - Action  > - Right\r\n");
    gotoxy(STDWINDOWX + 2, STDWINDOWY + 5);
    printf("Hit these:   With this:");
    gotoxy(STDWINDOWX + 2, STDWINDOWY + 7);
    fgcolor(5);
    for (i = 0; i < 8; i++) {
        printf("%c", BREAKABLETEXTURE);
    }
    gotoxy(STDWINDOWX + 2, STDWINDOWY + 8);
    for (i = 0; i < 8; i++) {
        printf("%c", BREAKABLETEXTURE);
    }
    gotoxy(STDWINDOWX + 20, STDWINDOWY + 7);
    fgcolor(0x0a);
    printf("%c", BALLTEXTURE);
    fgcolor(STDTEXTCOLOR);
    gotoxy(STDWINDOWX + 2, STDWINDOWY + 10);
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
    int score;

    do {
        window(STDWINDOWX, STDWINDOWY, 50, 21, "Choose level", 1);
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
        window(STDWINDOWX, STDWINDOWY, 50, 21, "Main menu", 1);
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



