//
// Created by asger on 12-06-2017.
//
#include <eZ8.h>                          // special encore constants, macros and flash functions
#include <sio.h>                          // special encore serial i/o functions
#include "ansi.h"
#include "sinLUT.h"
#include "math.h"
#include "Z8encore.h"
#include "engine.h"


void main() {
    entity map[6];
    int i, j, n = 0;

    map[n].whatIsThis = 0x01;
    map[n].changedSinceLast = 1;
    map[n].x1 = (char) (MAPSIZE / 2);
    map[n].y1 = (char) MAPSIZE;
    //map[n].direction = {0, 0};
    map[n].size = 0x16;
    map[n].color = 0x00;
    n++;

    // Ball setup
    map[n].whatIsThis = 0x02;
    map[n].changedSinceLast = 1;
    map[n].x1 = (char) (MAPSIZE / 2);
    map[n].y1 = (char) (MAPSIZE) - 1;
    //map[n].direction = {0, -1};
    map[n].size = 0x11;
    map[n].color = 0x00;
    n++;

    //Breakable setup
    map[n].whatIsThis = 0x03;
    map[n].changedSinceLast = 1;
    map[n].x1 = (char) (5);
    map[n].y1 = (char) (5);
    //map[n].direction = {0, 0};
    map[n].size = 0x11;
    map[n].color = 0x00;
    n++;
    map[n].whatIsThis = 0x00;
    n++;
	   
	
	initiate(100,100);
    drawMap(map);

    do {} while (1);
}