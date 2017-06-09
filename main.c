#include <eZ8.h>  						  // special encore constants, macros and flash functions
#include <sio.h>	                      // special encore serial i/o functions
#include "ansi.h"
#include "sinLUT.h"
#include "math.h"
#include "LEDdisplay.h"
#include "engine.h"

#define map breakables

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
    int i, j, n;
    entity breakables[40];
    entity player[1];
    entity ball[1];
    entity solid[3];
    struct TVector nowhere = {0, 0};
    entity nothing = {0, 0x00, 0, 0, nowhere, 0, 0};
    entity map[5][40];

    //&player, &ball, &breakables, &solid, &nothing
    //Creates player
    map.whatIsThis = 0x01;

    //Creates 40 blocks to kill
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 10; j++) {
            map[2][n]->changedSinceLast = 1;
            map[2][n]->whatIsThis = 0x03;
            map[2][n]->x1 = j * 5;
            map[2][n]->y1 = i + 1;
            map[n][n]->size = 0x14;
            n++;
        }
    }
}

/*
 * typedef struct{
    char changedSinceLast;
    char whatIsThis;//player     - 0x01
    //ball       - 0x02
    //breakable  - 0x03
    //solid      - 0.04
    //nothing    - 0x00
    char x1; //1. coordinate, placement
    char y1; //2. coordiante, placement
    struct vector direction; //ADD THIS STRUCT TO THE INCLUDED CODE PLS
    //zones? - so far no zones
    char size;
    char color;
} entity;
 */


void startGame() {
    entity *map;
    map = buildMap();

}

void showControls() {

}

void highScores() {

}

rom char string[LED_MAX_STR_LEN] = "    sample text";

void main() {
    char what_to_do = 0x01;
	init_uart(_UART0,_DEFFREQ,_DEFBAUD);  // set-up UART0 to 57600, 8n1
	LEDinit();
	LEDsetstring(string);
	LEDscroll();
	do {
		LEDupdate();

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
