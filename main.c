#include <eZ8.h>  						  // special encore constants, macros and flash functions
#include <sio.h>	                      // special encore serial i/o functions
#include "ansi.h"
#include "sinLUT.h"
#include "math.h"
#include "Z8encore.h"
#include "engine.h"


void showMenu() {
    window(10, 10, 70, 70, "Main menu", 1);
    gotoxy(12, 12);
    printf(" > Start game");
    printf("   Show controls");
    printf("   Show high scores");
    // TODO : implement input
/*    while(){

    }*/

}

entity *buildMap() {
    int i, j, n = 0;
    entity map[46];

    //player, ball, breakables, solid, nothing
    // TODO : Create constant MAPSIZE
    //Player setup
    map[n].whatIsThis = 0x01;
    map[n].changedSinceLast = 1;
    map[n].x1 = (char) (MAPSIZE / 2);
    map[n].y1 = (char) MAPSIZE;
    map[n].direction = {0, 0};
    map[n].size = 0x16;
    map[n].color = 0x00;
    n++;
    map[n].whatIsThis = 0x00;
    n++;

    //Ball.etup
    map[n].whatIsThis = 0x02;
    map[n].changedSinceLast = 1;
    map[n].x1 = (char) (MAPSIZE / 2);
    map[n].y1 = (char) (MAPSIZE) - 1;
    map[n].direction = {0, -1};
    map[n].size = 0x11;
    map[n].color = 0x00;
    n++;
    map[n].whatIsThis = 0x00;
    n++;

    //Creates 40 blocks to kill
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 10; j++) {
            map[n].changedSinceLast = 1;
            map[n].whatIsThis = 0x03;
            map[n].x1 = (char) (j * 5);
            map[n].y1 = (char) (i + 1);
            map[n].size = 0x14;
            map[n].color = 0x00;
            n++;
        }
    }

    // Creates the last nothing
    map[n].whatIsThis = 0x00;

    return map;
}

int winCondition(int score) {
    if (score >= 40) {
        return 1;
    }
    return 0;
}

void startGame() {
    entity *map;
    char buttonpress;
    int score = 0;
    map = buildMap();

    do {
        do {
            drawMap(map);
            do {
                //Determine where to move
                buttonpress = readKey();

                //Remember to do this a lot
                LEDUpdate();
            } while (readMsec() < 100);

            //Then move the player
            playerMovement(buttonpress, map);

        } while (readMsec() < 500);
        //Then move the ball
        ballMovement(map);
        //Check for collisions
        collisionCheck(map);
        //Update the map
        drawMap(map);

        //TODO : How do we update the score?? Return from collision check?

    } while (winCondition(score));
}

void showControls() {

}

void highScores() {

}

rom char string[LED_MAX_STR_LEN] = "    Breakout!";

void main() {
    char what_to_do = 0x01;
	init_uart(_UART0,_DEFFREQ,_DEFBAUD);  // set-up UART0 to 57600, 8n1
    LEDInit();
    LEDSetString(string);
    LEDScroll();
	do {
        LEDUpdate();

        switch (what_to_do) {
            case 1 :
                showMenu();
                break;
            case 2 :
                startGame();
                break;
            case 3 :
                showControls();
                break;
            case 4 :
                highScores();
                break;
            case 5 :
                return 1;
                break;
            default:
                what_to_do = 1;
                break;
        }


    } while (1 != 2);                  // stay here forever
}
