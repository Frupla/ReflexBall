#ifndef _ENGINE_H_
#define _ENGINE_H_

#define MAPSIZE 64
#define LONG_TO_EIGHTEEN_FOURTEEN(a) (a << 14)

//kode
/*
typedef struct{
    char changedSinceLast;
    char whatIsThis;//player     - 0x01
    //ball       - 0x02
    //breakable  - 0x03
    //solid      - 0.04
    //nothing    - 0x00
    long x1; //1. coordinate, placement
    long y1; //2. coordiante, placement
    Tvector direction; //ADD THIS STRUCT TO THE INCLUDED CODE PLS
    //zones? - so far no zones
    char sizeX, sizeY;
    char color;
} entity;
*/
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
} ball_t;

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
    char sizeX; // represent the horizontal size factor
    char color; // n.o. lives for breakables  (color breakables after this). Is set to 3 in drawBreakables
    // 0x00 - dead, no lives
} player_t;

typedef struct{
    char changedSinceLast;
    char whatIsThis;
    int x1; //1. coordinate, placement, 18.14
    int y1; //2. coordiante, placement, 18.14
    //Tvector direction; // Speed and direction (only relevant for the ball)
    char sizeX; // represent the horizontal size factor
    char sizeY;	// represent the vertical size factor
    char lives; // n.o. lives for breakables  (color breakables after this). Is set to 3 in drawBreakables
    // 0x00 - dead, no lives
} breakable_t;

void initiate();

//void drawPlayer(entity *);

//void drawBall(entity *);

//void drawBreakable(entity *);

//void drawSolid(entity *);

void drawMap(player_t *, ball_t *, breakable_t *);

void playerMovement(char, player_t *);

void ballMovement(ball_t *, player_t *, breakable_t *);

void collisionCheck(player_t *, breakable_t *);



#endif /*! _ENGINE_H_ */