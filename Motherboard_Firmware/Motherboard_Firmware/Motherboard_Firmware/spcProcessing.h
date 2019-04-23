/*
* SpcProcessing.h
*
* Created: 12/20/2018 4:57:59 PM
* Author: Anthony
*/


#ifndef __SPCPROCESSING_H__
#define __SPCPROCESSING_H__

#include <Arduino.h>
#include "board.h"
#include "Device_Configuration.h"
#include "Error.h"

class SpcProcessing
{
	//variables
	public:
	bool IsInSimulationMode = false;
	bool newData = false;
	
	

	protected:
	
	private:
	char serialOutputBuffer[MAX_CMD_LENGTH] = {0};
	Error eError;
	float SPCDiameter;

		

	//functions
	public:
	SpcProcessing();
	~SpcProcessing();
	void RunSPCDataLoop(void);
	void init(void);
	float GetDiameter(void);
	char *GetSerialOutputBuffer(void);
	Error *GetError(void);
	bool HasError(void);
	
	
	
	protected:
	
	private:
	static SpcProcessing *firstinstance;
	SpcProcessing( const SpcProcessing &c );
	SpcProcessing& operator=( const SpcProcessing &c );
	int PrintRandomDiameterData(void);
	
}; //spcProcessing

#endif //__SPCPROCESSING_H__
