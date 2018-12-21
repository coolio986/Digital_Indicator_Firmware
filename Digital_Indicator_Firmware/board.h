#ifndef __BOARD_H__
#define __BOARD_H__

#include <Arduino.h>

#define SERIAL_BAUD (32400) //baud rate for the serial ports

//#define BOARDTYPE 0 //0 = TEENSY20   1 = LEONARDO


#if defined(__AVR_ATmega32U4__)
#if defined(CORE_TEENSY)
#define TEENSY20
#else
#define LEONARDO
#endif
#endif


#define INDICATOR_REQ  3
#define INDICATOR_DAT  2
#define INDICATOR_CLK  0

#endif//__BOARD_H__
