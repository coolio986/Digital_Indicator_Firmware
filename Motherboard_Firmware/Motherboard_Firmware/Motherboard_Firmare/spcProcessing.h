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

class SpcProcessing
{
  //variables
  public:
	bool IsInSimulationMode = false;
	
	

  protected:
  
  private:
	  //int req = INDICATOR_REQ; //mic REQ line goes to pin 5 through q1 (arduino high pulls request line low)
	  //int dat = INDICATOR_DAT; //mic Data line goes to pin 2
	  //int clk = INDICATOR_CLK; //mic Clock line goes to pin 3
	  uint32_t loopTime = 50;
	  uint32_t currentMillis = 0;
	  uint32_t previousMillis = 0;
	  String dataStream;
	  char serialOutputBuffer[MAX_CMD_LENGTH] = {0};
	  int retries = 0;
	  

  //functions
  public:
	  SpcProcessing();
	  ~SpcProcessing();
	  void RunSPCDataLoop(void);
	  void init(void);
	  float GetDiameter(void);
	  char *GetSerialOutputBuffer(void);
	  bool newData = false;
  
  protected:
  
  private:
	  static SpcProcessing *firstinstance;
	  SpcProcessing( const SpcProcessing &c );
	  SpcProcessing& operator=( const SpcProcessing &c );
	  int PrintRandomDiameterData(void);
	  
	  float SPCDiameter;
}; //spcProcessing





#endif //__SPCPROCESSING_H__
