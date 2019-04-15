/*
* spcProcessing.cpp
*
* Created: 12/20/2018 4:57:59 PM
* Author: Anthony
*/


#include "SpcProcessing.h"
#include <Arduino.h>
#include "hardwareTypes.h"
#include "DataConversions.h"
#include "SerialProcessing.h"

SpcProcessing *SpcProcessing::firstinstance;


// default constructor main entry point
SpcProcessing::SpcProcessing()
{
	if(!firstinstance)
	{
		firstinstance = this;
	}

	
} //spcProcessing

void SpcProcessing::init(void)
{
	pinMode(req, OUTPUT);
	pinMode(clk, INPUT_PULLUP);
	pinMode(dat, INPUT_PULLUP);

	digitalWrite(req, HIGH); // set request at high via transistor needed for default state
}

void SpcProcessing::RunSPCDataLoop(void)
{
	
	
	if (IsInSimulationMode)
	{
		PrintRandomDiameterData();
		return;
	}

	PORTA |= digitalPinToBitMask(req); //generate set request
	
	char rawSPC[53] = {0};
	int pinState = 0;
	int lastPinState = 0;
	int loopCount = 0;

	for (int i = 0; i < 52;)
	{
		pinState = !(PINA & digitalPinToBitMask(clk)) == 0;

		if (pinState != lastPinState)
		{
			if (pinState == LOW)
			{
				rawSPC[i] = (PINA & digitalPinToBitMask(dat)) == 0 ? 48 : 49;
				i++;
			}
		}
		
		loopCount++;

		if (loopCount >= 15000){

			SerialCommand sError;
			sError.hardwareType = INDICATOR;
			sError.command = "INDICATOR";
			sError.value = "INDICATOR HAS BEEN DISCONNECTED";
			char sErrorOutput [MAX_CMD_LENGTH] = {0};
			BuildSerialOutput(&sError, sErrorOutput);
			Serial.println(sErrorOutput);
			return; //kicks out the loop if the SPC gets disconnected or has an error
		}
		
		lastPinState = pinState;
	}

	bool dataStreamValid = false;
	for (unsigned int i = 0; i < 12; i++)
	{
		if (rawSPC[i] == 48) //48 is 0 (zero) in ascii
		{
			dataStreamValid = false;

			SerialCommand sError;
			sError.hardwareType = INDICATOR;
			sError.command = "INDICATOR";
			sError.value = "A SPC PROCESSING ERROR HAS OCCURRED";
			char sErrorOutput [MAX_CMD_LENGTH] = {0};
			BuildSerialOutput(&sError, sErrorOutput);
			Serial.println(sErrorOutput);
			
			break;
		}
		else {dataStreamValid = true;}
	}
	
	if (dataStreamValid)
	{
		byte bytes[13] = {0};
		for (int i = 0; i < 13; i++)
		{
			int idx = (i*4) + 4;
			int bitPointer = 0;
			for (int j = i * 4; j < idx ; j++)
			{
				bitWrite(bytes[i], bitPointer, rawSPC[j] == 49); //49 ascii for 1 //grab nibbles from rawSPC
				bitPointer++;
			}
		}

		float preDecimalNumber = 0.0;
		char buf[7];

		for(int i=0;i<6;i++){ //grab array positions 5-10 for diameter numbers
			
			buf[i]=bytes[i+5]+'0';
			
			buf[6]=0;
			
			preDecimalNumber=atol(buf); //assembled measurement, no decimal place added
		}

		int decimalPointLocation = bytes[11];

		SPCDiameter = preDecimalNumber / (pow(10, decimalPointLocation)); //add decimal to number

		char outputBuffer[MAX_CMD_LENGTH] = {0};

		char decimalNumber[10] = {0};
		CONVERT_FLOAT_TO_STRING(SPCDiameter, decimalNumber);
		
		SerialCommand sCommand;
		sCommand.hardwareType = INDICATOR;
		sCommand.command = "INDICATOR";
		sCommand.value = decimalNumber;
		
		BuildSerialOutput(&sCommand, outputBuffer);
		Serial.println(outputBuffer);
		
	}
	

	
}

int SpcProcessing::PrintRandomDiameterData(void)
{
	char diameter[5];
	ltoa(random(17000, 18000), diameter, 10);
	
	Serial.print("0;111111111111111111110000");
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			Serial.print(((byte)diameter[i] >> j) & 1);
		}
	}
	Serial.println("00100000");
	return 0;
}

float SpcProcessing::GetDiameter(void){

	return SPCDiameter;
}


// default destructor
SpcProcessing::~SpcProcessing()
{
} //~spcProcessing


