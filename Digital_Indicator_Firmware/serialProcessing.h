#ifndef __SERIALPROCESSING_H__
#define __SERIALPROCESSING_H__

#include <Arduino.h>

const byte numChars = 32;
static char receivedChars[numChars]; // an array to store the received data

typedef struct
{
  uint16_t hardwareType; //see hardwareTypes.h
  char *value;
  
  
} serialCommand;


char * CheckSerial(usb_serial_class port);
char * CheckSerial(HardwareSerial port);
serialCommand GetSerialArgs(char * serialData, uint16_t hardwareTypes[]);
bool ExistsInIntArray(uint16_t *arrayToCheck, uint16_t numberToCheck);


#endif//__SERIALPROCESSING_H__
