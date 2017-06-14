//
// Created by asger on 13-06-2017.
//
#include <eZ8.h>                          // special encore constants, macros and flash functions
#include <sio.h>                          // special encore serial i/o functions
#include "standalone_timer.h"

int timerValue = 0;            // counts the time in 0.1 s, resets at 32001

#pragma interrupt
void timer1int() {
    timerValue += 1;
    if (timerValue >= 32000) {
        timerValue = 0;
    }
}

void timer1Setup() {
    T1CTL = 0x01;       // 0 0 000 001 - Disabled - cont mode
    T1H = 0x1;          //Begin at 1
    T1L = 0x0;          // -||-
    T1RH = 0xE1;        //Reset every 0.1 sec = 100 ms
    T1RL = 0x00;
    SET_VECTOR(TIMER1, timer1int);
    IRQ0ENH &= ~0x40;   //Interrupt priority
    IRQ0ENL |= 0x40;    // -||-
    T1CTL |= 0x80;      //Turn it on
    EI();               //Turn on interrupts
}

int timer1() {
    return timerValue;
}