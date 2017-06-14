#include <eZ8.h>  						  // special encore constants, macros and flash functions
#include <sio.h>	                      // special encore serial i/o functions
#include "engine.h"


breakable_t * level1(){
	int i, j, n=0;
	breakable_t breakable[36];
    //Breakable setup
    for (i = 15; i <= 105 ; i += 15) {
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
    return breakable;
}