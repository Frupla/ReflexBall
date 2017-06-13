#ifndef _ENGINE_H_
#define _ENGINE_H_

#define MAPSIZE 64
#define LONG_TO_EIGHTEEN_FOURTEEN(a) (a << 14)


typedef struct{
    char changedSinceLast;
    char whatIsThis;//player     - 0x01
    //ball       - 0x02
    //breakable  - 0x03
    //solid      - 0.04
    //nothing    - 0x00
    long x1; //1. coordinate, placement
    long y1; //2. coordiante, placement
    Tvector direction; // Direction. Vector uses 18.14 encoding
    char sizeX, sizeY;
    char color;
} entity;

void initiate();
void drawMap(entity *);
void playerMovement(char, entity *);

char ballMovement(entity *);
void collisionCheck(entity *);



#endif /*! _ENGINE_H_ */