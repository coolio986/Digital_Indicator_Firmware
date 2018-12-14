#ifndef __BOARD_H__
#define __BOARD_H__

#include <Arduino.h>

#define SERIAL_BAUD (32400) //baud rate for the serial ports

#define BOARDTYPE 1 //0 = TEENSY20   1 = LEONARDO

#if BOARDTYPE == 0
#define TEENSY20
#endif
#if BOARDTYPE == 1
#define LEONARDO
#endif


#endif//__BOARD_H__
