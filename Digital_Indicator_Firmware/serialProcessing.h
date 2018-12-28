#ifndef __SERIALPROCESSING_H__
#define __SERIALPROCESSING_H__

#include <Arduino.h>
#include "board.h"
#include "hardwareTypes.h"

#ifdef LEONARDO
#include <SoftwareSerial.h>
#endif

#ifdef TEENSY20
#include <usb_api.h>
#endif



const byte numChars = 32;
static char receivedChars[numChars]; // an array to store the received data

typedef struct
{
  uint16_t hardwareType; //see hardwareTypes.h
  char *value;
  
  
} serialCommand;

#ifdef TEENSY20
char * CheckSerial(usb_serial_class port);
#endif

char * CheckSerial(HardwareSerial port);

#ifdef LEONARDO
char * CheckSerial(Serial_ port);
char * CheckSerial(SoftwareSerial *port);
char * CleanseData(char *data);
#endif

serialCommand GetSerialArgs(char * serialData);
bool ExistsInIntArray(uint16_t *arrayToCheck, uint16_t numberToCheck);


#endif//__SERIALPROCESSING_H__
