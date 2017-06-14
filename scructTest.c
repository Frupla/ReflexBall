//
// Created by Irene Danvy on 14/06/2017.
//

#include <eZ8.h>  						  // special encore constants, macros and flash functions
#include <sio.h>	                      // special encore serial i/o functions
#include "ansi.h"
#include "Z8encore.h"
#include "math.h"



typedef struct{
	char changedSinceLast;
	char whatIsThis;//player           - 0x01
					//ball             - 0x02
					//breakable        - 0x03
					//solid            - 0x04
                    //broken breakable - 0x05
					//nothing          - 0x00
	long x1; //1. coordinate, placement, 18.14
	long y1; //2. coordiante, placement, 18.14
	Tvector direction; // Speed and direction (only relevant for the ball)
	//zones? - so far no zones
	char sizeX; // represent the horizontal size factor
    char sizeY;	// represent the vertical size factor
	char color; // n.o. lives for breakables  (color breakables after this). Is set to 3 in drawBreakables
                // 0x00 - dead, no lives
} entity_one;


typedef struct{
    char changedSinceLast;
    //not sure if I need this
    char whatIsThis;//player           - 0x01
    //ball             - 0x02
    //breakable        - 0x03
    //solid            - 0x04
    //broken breakable - 0x05
    //nothing          - 0x00
    long x1; //1. coordinate, placement, 18.14
    long y1; //2. coordiante, placement, 18.14
    Tvector direction; // Speed and direction (only relevant for the ball)
    char size; // represent the size factor, balls must be symmetric
    char color; // n.o. lives for breakables  (color breakables after this). Is set to 3 in drawBreakables
} ball_one;

typedef struct{
    char changedSinceLast;
    char whatIsThis;//player           - 0x01
    //ball             - 0x02
    //breakable        - 0x03
    //solid            - 0x04
    //broken breakable - 0x05
    //nothing          - 0x00
    int x1; //1. coordinate, placement, 18.14
    int y1; //2. coordiante, placement, 18.14
    //Tvector direction; // Speed and direction (only relevant for the ball)
    //zones? - so far no zones
    char sizeX; // represent the horizontal size factor
    char color; // n.o. lives for breakables  (color breakables after this). Is set to 3 in drawBreakables
    // 0x00 - dead, no lives
} player_one;

typedef struct{
    char changedSinceLast;
    char whatIsThis;
    int x1; //1. coordinate, placement, 18.14
    int y1; //2. coordiante, placement, 18.14
    //Tvector direction; // Speed and direction (only relevant for the ball)
    //zones? - so far no zones
    char sizeX; // represent the horizontal size factor
    char sizeY;	// represent the vertical size factor
    char lives; // n.o. lives for breakables  (color breakables after this). Is set to 3 in drawBreakables
    // 0x00 - dead, no lives
} breakable_one;

typedef struct{
    char changedSinceLast;
    char whatIsThis;//player           - 0x01
    //ball             - 0x02
    //breakable        - 0x03
    //solid            - 0x04
    //broken breakable - 0x05
    //nothing          - 0x00
    //zones? - so far no zones
    char sizeX; // represent the horizontal size factor
    char sizeY;	// represent the vertical size factor
    char color; // n.o. lives for breakables  (color breakables after this). Is set to 3 in drawBreakables
    // 0x00 - dead, no lives
    long x1; //1. coordinate, placement, 18.14
    long y1; //2. coordiante, placement, 18.14
    Tvector direction; // Speed and direction (only relevant for the ball)
} entity_two;


typedef struct{
    char changedSinceLast;
    //not sure if I need this
    char whatIsThis;//player           - 0x01
    //ball             - 0x02
    //breakable        - 0x03
    //solid            - 0x04
    //broken breakable - 0x05
    //nothing          - 0x00
    char size; // represent the size factor, balls must be symmetric
    char color; // n.o. lives for breakables  (color breakables after this). Is set to 3 in drawBreakables
    long x1; //1. coordinate, placement, 18.14
    long y1; //2. coordiante, placement, 18.14
    Tvector direction; // Speed and direction (only relevant for the ball)
} ball_two;

typedef struct{
    char changedSinceLast;
    char whatIsThis;//player           - 0x01
    //ball             - 0x02
    //breakable        - 0x03
    //solid            - 0x04
    //broken breakable - 0x05
    //nothing          - 0x00
    //Tvector direction; // Speed and direction (only relevant for the ball)
    //zones? - so far no zones
    char sizeX; // represent the horizontal size factor
    char color; // n.o. lives for breakables  (color breakables after this). Is set to 3 in drawBreakables
    // 0x00 - dead, no lives
    int x1; //1. coordinate, placement, 18.14
    int y1; //2. coordiante, placement, 18.14
} player_two;

typedef struct{
    char changedSinceLast;
    char whatIsThis;
    //Tvector direction; // Speed and direction (only relevant for the ball)
    //zones? - so far no zones
    char sizeX; // represent the horizontal size factor
    char sizeY;	// represent the vertical size factor
    char lives; // n.o. lives for breakables  (color breakables after this). Is set to 3 in drawBreakables
    // 0x00 - dead, no lives
    int x1; //1. coordinate, placement, 18.14
    int y1; //2. coordiante, placement, 18.14
} breakable_two;

typedef struct{
    long x1; //1. coordinate, placement, 18.14
    long y1; //2. coordiante, placement, 18.14
    char changedSinceLast;
    char whatIsThis;//player           - 0x01
    //ball             - 0x02
    //breakable        - 0x03
    //solid            - 0x04
    //broken breakable - 0x05
    //nothing          - 0x00
    //zones? - so far no zones
    char sizeX; // represent the horizontal size factor
    char sizeY;	// represent the vertical size factor
    char color; // n.o. lives for breakables  (color breakables after this). Is set to 3 in drawBreakables
    // 0x00 - dead, no lives
    Tvector direction; // Speed and direction (only relevant for the ball)
} entity_three;


typedef struct{
    long x1; //1. coordinate, placement, 18.14
    long y1; //2. coordiante, placement, 18.14
    char changedSinceLast;
    //not sure if I need this
    char whatIsThis;//player           - 0x01
    //ball             - 0x02
    //breakable        - 0x03
    //solid            - 0x04
    //broken breakable - 0x05
    //nothing          - 0x00
    char size; // represent the size factor, balls must be symmetric
    char color; // n.o. lives for breakables  (color breakables after this). Is set to 3 in drawBreakables
    Tvector direction; // Speed and direction (only relevant for the ball)
} ball_three;

typedef struct{
    int x1; //1. coordinate, placement, 18.14
    int y1; //2. coordiante, placement, 18.14
    char changedSinceLast;
    char whatIsThis;//player           - 0x01
    //ball             - 0x02
    //breakable        - 0x03
    //solid            - 0x04
    //broken breakable - 0x05
    //nothing          - 0x00
    //Tvector direction; // Speed and direction (only relevant for the ball)
    //zones? - so far no zones
    char sizeX; // represent the horizontal size factor
    char color; // n.o. lives for breakables  (color breakables after this). Is set to 3 in drawBreakables
    // 0x00 - dead, no lives
} player_three;

typedef struct{
    int x1; //1. coordinate, placement, 18.14
    int y1; //2. coordiante, placement, 18.14
    char changedSinceLast;
    char whatIsThis;
    //Tvector direction; // Speed and direction (only relevant for the ball)
    //zones? - so far no zones
    char sizeX; // represent the horizontal size factor
    char sizeY;	// represent the vertical size factor
    char lives; // n.o. lives for breakables  (color breakables after this). Is set to 3 in drawBreakables
    // 0x00 - dead, no lives
} breakable_three;


void main(){
    //normal
    entity_one eone;
    player_one pone;
    ball_one bone;
    breakable_one breakone;
    //increasing
    entity_two etwo;
    player_two ptwo;
    ball_two btwo;
    breakable_two breaktwo;

    //decreasing
    entity_three ethree;
    player_three pthree;
    ball_three bthree;
    breakable_three breakthree;

    init_uart(_UART0,_DEFFREQ,_DEFBAUD);
    clrscr();
    eone.x1 = 1;
    eone.y1 = 1;
    eone.whatIsThis = 1;
    eone.direction.x = 1;
	eone.direction.y = 1;
    eone.color = 1;
    eone.sizeX = 1;
    eone.sizeY = 1;

    pone.sizeX = 1;
    pone.color = 1;
    pone.whatIsThis = 1;
    pone.changedSinceLast = 1;
    pone.x1 = 1;
    pone.y1 = 1;

    bone.y1 = 1;
    bone.x1 = 1;
    bone.changedSinceLast = 1;
    bone.whatIsThis = 1;
    bone.color = 1;
    bone.direction.x = 1;
    bone.direction.y = 1;
    bone.size = 1;

    breakone.whatIsThis = 1;
    breakone.changedSinceLast = 1;
    breakone.x1 = 1;
    breakone.y1 = 1;
    breakone.sizeX = 1;
    breakone.sizeY = 1;
    breakone.lives = 1;

    etwo.x1 = 1;
    etwo.y1 = 1;
    etwo.whatIsThis = 1;
    etwo.direction.x  = 1;
    etwo.direction.y = 1;
    etwo.color = 1;
    etwo.sizeX = 1;
    etwo.sizeY = 1;

    ptwo.sizeX = 1;
    ptwo.color = 1;
    ptwo.whatIsThis = 1;
    ptwo.changedSinceLast = 1;
    ptwo.x1 = 1;
    ptwo.y1 = 1;

    btwo.y1 = 1;
    btwo.x1 = 1;
    btwo.changedSinceLast = 1;
    btwo.whatIsThis = 1;
    btwo.color = 1;
    btwo.direction.x = 1;
    btwo.direction.y = 1;
    btwo.size = 1;

    breaktwo.whatIsThis = 1;
    breaktwo.changedSinceLast = 1;
    breaktwo.x1 = 1;
    breaktwo.y1 = 1;
    breaktwo.sizeX = 1;
    breaktwo.sizeY = 1;
    breaktwo.lives = 1;

    ethree.x1 = 1;
    ethree.y1 = 1;
    ethree.whatIsThis = 1;
    ethree.direction.x = 1;
    ethree.direction.y = 1;
    ethree.color = 1;
    ethree.sizeX = 1;
    ethree.sizeY = 1;

    pthree.sizeX = 1;
    pthree.color = 1;
    pthree.whatIsThis = 1;
    pthree.changedSinceLast = 1;
    pthree.x1 = 1;
    pthree.y1 = 1;

    bthree.y1 = 1;
    bthree.x1 = 1;
    bthree.changedSinceLast = 1;
    bthree.whatIsThis = 1;
    bthree.color = 1;
    bthree.direction.x = 1;
    bthree.direction.y = 1;
    bthree.size = 1;

    breakthree.whatIsThis = 1;
    breakthree.changedSinceLast = 1;
    breakthree.x1 = 1;
    breakthree.y1 = 1;
    breakthree.sizeX = 1;
    breakthree.sizeY = 1;
    breakthree.lives = 1;

    printf(" normal: entity %x, breakable %x, ball %x,player %x \n", sizeof(eone), sizeof(breakone), sizeof(bone),
           sizeof(pone));
    printf(" increasing: entity %x, breakable %x, ball %x,player %x \n", sizeof(etwo), sizeof(breaktwo), sizeof(btwo),
           sizeof(ptwo));
    printf(" decreasing: entity %x, breakable %x, ball %x,player %x \n", sizeof(ethree), sizeof(breakthree), sizeof(bthree),
           sizeof(pthree));
    do{} while (1);
}