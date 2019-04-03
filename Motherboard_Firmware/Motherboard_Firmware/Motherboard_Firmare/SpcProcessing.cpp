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
	currentMillis = millis();
	int maxRetries = 5;


	if (currentMillis >= (previousMillis + loopTime))
	{
		if (IsInSimulationMode)
		{
			PrintRandomDiameterData();
			return;
		}
		
		digitalWrite(req, HIGH); // generate set request

		int pinState = 0;
		int lastPinState = 0;
		for (int i = 0; i < 52;)
		{
			
			pinState = digitalRead(clk);

			if (pinState != lastPinState)
			{
				if (pinState == LOW)
				{
					dataStream += digitalRead(dat);
					i++;
				}
			}
			else
			{
				maxRetries--;
			}
			
			if (maxRetries <= 0){return;}

			lastPinState = pinState;
		}

		
		bool dataStreamValid = false;
		for (unsigned int i = 0; i < dataStream.length(); i++)
		{
			if (dataStream[i] == '0')
			{
				dataStreamValid = true;
				break;
			}
		}
		
		
		if (dataStreamValid)
		{
			char charBuilder[100];
			SerialCommand sCommand;
			sCommand.hardwareType = INDICATOR;
			strcpy (sCommand.command, dataStream.c_str());
			BuildSerialOutput(&sCommand, charBuilder);
			//BUILD_SERIAL_OUTPUT(INDICATOR, dataStream.c_str(), charBuilder);
			//BUILD_SERIAL_OUTPUT(sCommand, charBuilder);

			Serial.println(charBuilder);
		}
		
		dataStream = "";
		digitalWrite(req, LOW);
		previousMillis = currentMillis;
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

// default destructor
SpcProcessing::~SpcProcessing()
{
} //~spcProcessing
