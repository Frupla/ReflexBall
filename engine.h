#ifndef _ENGINE_H_
#define _ENGINE_H_

#define MAPSIZE 400

//kode
typedef struct{
    char changedSinceLast;
    char whatIsThis;//player     - 0x01
    //ball       - 0x02
    //breakable  - 0x03
    //solid      - 0.04
    //nothing    - 0x00
    char x1; //1. coordinate, placement
    char y1; //2. coordiante, placement
    Tvector direction; //ADD THIS STRUCT TO THE INCLUDED CODE PLS
    //zones? - so far no zones
    char size;
    char color;
} entity;

void drawPlayer(entity);

void drawBall(entity);

void drawBreakable(entity);

void drawSolid(entity);

void drawMap(entity[]);

void playerMovement(entity[]);

void ballMovement(entity *);

void collisionCheck(entity[]);



#endif /*! _ENGINE_H_ */