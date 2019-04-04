#ifndef __SERIALPROCESSING_H__
#define __SERIALPROCESSING_H__

#include <Arduino.h>
#include "board.h"
#include "hardwareTypes.h"
#include "SerialCommand.h"
#include "Device_Configuration.h"

const byte numChars = 32;
static char receivedChars[numChars]; // an array to store the received data



class SerialProcessing {

public:
SerialProcessing();
~SerialProcessing();
void CheckSerialCommands();
void init();





private:
static SerialProcessing *firstInstance;
SerialProcessing( const SerialProcessing &c );
SerialProcessing& operator=( const SerialProcessing &c );
void CheckInteralCommands(SerialCommand *sCommand);
void CheckSpoolerCommands(SerialCommand *sCommand);

};

char * CheckSerial(HardwareSerial *port);
char * CleanseData(char *data);

SerialCommand GetSerialArgs(char * serialData);
bool ExistsInIntArray(uint16_t *arrayToCheck, size_t arraySize, uint16_t numberToCheck);
void BuildSerialOutput(SerialCommand *sCommand, char *outputBuffer);
bool startsWith(const char *pre, const char *str);
void PrintRandomRPMData();




#endif//__SERIALPROCESSING_H__

