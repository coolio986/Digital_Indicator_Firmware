/*
* hardwareTypes.h
*
* Created: 3/12/2019 4:49:14 PM
* Author: Anthony
*/


#ifndef __HARDWARETYPES_H__
#define __HARDWARETYPES_H__

const int INDICATOR = 0; //indicator
const int SPOOLER = 1; //spooler stepper
const int EXTRUDER = 2; //extruder
const int TRAVERSE = 3; //traverse
const int INTERNALDEVICE = 100;
const int ERROR = 101;

typedef struct{
	uint16_t indicator; //indicator
	uint16_t spooler; //spooler
	uint16_t extruder; //extruder
	uint16_t traverse; //traverse
	uint16_t internal; //internal
	uint16_t error; //error
} struct_hardwareTypes;

const static struct_hardwareTypes hardwareType =
{
	
	.indicator = INDICATOR,
	.spooler = SPOOLER,
	.extruder = EXTRUDER,
	.traverse = TRAVERSE,
	.internal = INTERNALDEVICE,
	.error = ERROR
};

static uint16_t int_hardwareTypes[] =
{
	
	
	INDICATOR, //0 = indicator
	SPOOLER, //1 = spooler
	EXTRUDER, //2 = extruder
	TRAVERSE, //3 = traverse
	INTERNALDEVICE, // 100 = internal
	ERROR,	//101 = Error
};


#endif //__HARDWARETYPES_H__