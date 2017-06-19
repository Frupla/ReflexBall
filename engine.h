#ifndef _ENGINE_H_
#define _ENGINE_H_

//Required inclusions
#include "math.h"

//Definitions

#define MAPSIZE 59
#define LONG_TO_EIGHTEEN_FOURTEEN(a) (a << 14)
#define EIGHTEEN_FOURTEEN_TO_INT(a) ((int)((a + 0x2000) >> 14))
#define SOLIDTEXTURE 219
#define PLAYERTEXTURE 223
#define BALLTEXTURE 184
#define BREAKABLETEXTURE 219
#define BACKGROUNDTEXTURE 32
#define STDTEXTCOLOR 15

//Structures:

typedef struct{
    char changedSinceLast;
    char whatIsThis;    //player           - 0x01
    //ball             - 0x02
    //breakable        - 0x03
    //solid            - 0x04
    //broken breakable - 0x05
    //nothing          - 0x00
    long x1; //1. coordinate, placement. Format: 18.14
    long y1; //2. coordiante, placement. Format: 18.14
    long xs; //1. coordinate, starting point. Format: 18.14
    long ys; //2. coordiante, starting point. Format: 18.14
    Tvector direction; // Speed and direction
    char color; // Color of the ball
} ball_t;

typedef struct{
    char changedSinceLast;
    char whatIsThis;//player           - 0x01
    //ball             - 0x02
    //breakable        - 0x03
    //solid            - 0x04
    //broken breakable - 0x05
    //nothing          - 0x00
    int x1; //1. coordinate, placement. Format: 18.14
    int y1; //2. coordiante, placement. Format: 18.14
    char sizeX; // represent the horizontal size factor
    char color; // Color of striker
    char lives; // Saves
} player_t;

typedef struct{
    char changedSinceLast;
    char whatIsThis;
    int x1; //1. coordinate, placement. Format: 18.14
    int y1; //2. coordiante, placement. Format: 18.14
    char sizeX; // represent the horizontal size factor
    char sizeY;	// represent the vertical size factor
    char lives; // n.o. lives for breakables  (color breakables after this).
    // 0x00 - dead, no lives
    char powerUp;   //Does this breakable have powerup?
    // 0x00 - None
    // 0x01 - Extra health
    // 0x02 - Larger striker
    // 0x03 - TBD
} breakable_t;

void initiateWithCeiling();

void initiateWithoutCeiling();

int findMaxScore(breakable_t*);

//void drawPlayer(entity *);

//void drawBall(entity *);

//void drawBreakable(entity *);

//void drawSolid(entity *);

void drawMap(player_t *, ball_t *, breakable_t *);

void playerMovement(char, player_t *);

char ballMovement(ball_t *, player_t *, breakable_t *, char);

//void collisionCheck(ball_t *, player_t *, breakable_t *, char);



#endif /*! _ENGINE_H_ */