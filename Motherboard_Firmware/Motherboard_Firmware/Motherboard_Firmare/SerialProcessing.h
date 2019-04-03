#ifndef __SERIALPROCESSING_H__
#define __SERIALPROCESSING_H__

#include <Arduino.h>
#include "board.h"
#include "hardwareTypes.h"

const byte numChars = 32;
static char receivedChars[numChars]; // an array to store the received data

typedef struct
{
  uint16_t hardwareType; //see hardwareTypes.h
  char *command;
  char *value;
} SerialCommand;

char * CheckSerial(HardwareSerial *port);
char * CleanseData(char *data);


SerialCommand GetSerialArgs(char * serialData);
bool ExistsInIntArray(uint16_t *arrayToCheck, size_t arraySize, uint16_t numberToCheck);
void BuildSerialOutput(SerialCommand *sCommand, char *outputBuffer);


#endif//__SERIALPROCESSING_H__

