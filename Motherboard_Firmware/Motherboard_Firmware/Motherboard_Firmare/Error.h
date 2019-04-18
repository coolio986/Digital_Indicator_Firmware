/*
 * Error.h
 *
 * Created: 4/18/2019 9:20:54 AM
 *  Author: Anthony
 */ 


#ifndef ERROR_H_
#define ERROR_H_

typedef struct
{
	uint16_t hardwareType; //see hardwareTypes.h
	//char *command;
	char *errorValue;
	byte errorLevel;
} Error;

//errorLevel:
// 1 = datastream validation failed
// 2 = device disconnected
// 3 = undefined



#endif /* ERROR_H_ */