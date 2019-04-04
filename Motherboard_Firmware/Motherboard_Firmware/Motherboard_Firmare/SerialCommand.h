/*
 * SerialCommand.h
 *
 * Created: 4/4/2019 8:00:27 AM
 *  Author: Anthony
 */ 


#ifndef SERIALCOMMAND_H_
#define SERIALCOMMAND_H_


typedef struct
{
	uint16_t hardwareType; //see hardwareTypes.h
	char *command;
	char *value;
} SerialCommand;


#endif /* SERIALCOMMAND_H_ */