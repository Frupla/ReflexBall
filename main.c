#include <eZ8.h>  						  // special encore constants, macros and flash functions
#include <sio.h>	                      // special encore serial i/o functions
#include "ansi.h"
#include "sinLUT.h"
#include "math.h"
#include "LEDdisplay.h"
#include "engine.h"

void showMenu() {
    window(10, 10, 70, 70, "Main menu", 1);
}

void startGame() {

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
