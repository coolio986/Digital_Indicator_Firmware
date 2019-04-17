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
	
	void init();
	void Poll(void);
	unsigned int CheckSerial(HardwareSerial *port, int portNumber);
	unsigned int CommandParse(SerialCommand *sCommand, char str[MAX_CMD_LENGTH]);
	//char *GetSerialOutputBuffer(void);
	bool newData = false;




	private:
	static SerialProcessing *firstInstance;
	SerialProcessing( const SerialProcessing &c );
	SerialProcessing& operator=( const SerialProcessing &c );
	void CheckInteralCommands(SerialCommand *sCommand);
	//void CheckSpoolerCommands(SerialCommand *sCommand);
	//void CheckSerialCommands();
	byte computer_bytes_received = 0;
	char computerdata[MAX_CMD_LENGTH] = {0};
	char *hardwareID;                       //Char pointer used in string parsing
	char *cmd;                           //Char pointer used in string parsing
	char *arguments;
	//int strcicmp(char const *a, char const *b);
	char serialOutputBuffer[MAX_CMD_LENGTH] = {0};
	unsigned int ProcessUSBData(SerialCommand *sCommand);
	unsigned int ProcessExpanderData(SerialCommand *sCommand);

};


char * CleanseData(char *data);

SerialCommand GetSerialArgs(char * serialData);
bool ExistsInIntArray(uint16_t *arrayToCheck, size_t arraySize, uint16_t numberToCheck);
void BuildSerialOutput(SerialCommand *sCommand, char *outputBuffer);
bool startsWith(const char *pre, const char *str);
void PrintRandomRPMData();






#endif//__SERIALPROCESSING_H__

