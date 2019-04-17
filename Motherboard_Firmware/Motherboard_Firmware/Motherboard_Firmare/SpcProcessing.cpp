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

void myISR();

// default constructor main entry point
SpcProcessing::SpcProcessing()
{
	if(!firstinstance)
	{
		firstinstance = this;
	}

	
} //spcProcessing

//char SpcProcessing::rawSPC_ISR[53] = {0};
//int SpcProcessing::loopCount = 0;
//bool SpcProcessing::lock = false;
volatile bool lock = false;
volatile char rawSPC_ISR[53] = {0};
volatile int loopCount = 0;
volatile int previousCount = 0;
char rawSPC[53] = {0};


void SpcProcessing::init(void)
{
	pinMode(INDICATOR_REQ, OUTPUT);
	pinMode(INDICATOR_CLK, INPUT_PULLUP);
	pinMode(INDICATOR_DAT, INPUT_PULLUP);

	digitalWrite(INDICATOR_REQ, HIGH); // set request at high via transistor needed for default state
	attachInterrupt(digitalPinToInterrupt(INDICATOR_CLK), myISR, FALLING);

}

void myISR()
{
	lock = true;
	rawSPC_ISR[loopCount++] = (PINA & digitalPinToBitMask(INDICATOR_DAT)) == 0 ? 48 : 49; //Keep track of ISR bits

	//previousCount = loopCount;
	if (loopCount >= 52)
	{
		
		PORTC &= ~digitalPinToBitMask(INDICATOR_REQ); //set req low to stop ISR
		
		//rawSPC_ISR[53] = 0; //add 0 to last position
		//Serial.println(SpcProcessing::rawSPC);
		loopCount = 0; //set loop counter back to 0
		for (int i = 0; i < 52; i++)
		{
			rawSPC[i] = rawSPC_ISR[i];
			rawSPC_ISR[i] = 0;
		}
		lock = false;
		
		
		//detachInterrupt(digitalPinToInterrupt(INDICATOR_CLK));
		//PORTC |= digitalPinToBitMask(INDICATOR_REQ); //set req high to restart ISR
	}
	
}

void SpcProcessing::RunSPCDataLoop(void)
{
	
	
	if (IsInSimulationMode)
	{
		PrintRandomDiameterData();
		return;
	}


	if ((previousCount > loopCount + 10)){ //track the loops to see if the ISR is still firing. Sometimes it doesn't trigger properly due to the screen updates
		Serial.println("loop error");
		PORTC &= ~digitalPinToBitMask(INDICATOR_REQ); //set req low to stop ISR
		PORTC |= digitalPinToBitMask(INDICATOR_REQ); //set req high to restart ISR
		loopCount = 0;
		previousCount = 0;
	}

	previousCount++;

	if (!lock)
	{

		if (rawSPC[0] == 0){
			//Serial.println("RESETTING FROM NO DATA");
			//PORTC |= digitalPinToBitMask(INDICATOR_REQ); //set req high to restart ISR
			return;
		}

		//Serial.print(rawSPC);
		//Serial.print("  ");
		//Serial.println(millis());

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

				loopCount = 0;
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
			
			
			
			char decimalNumber[10] = {0};
			CONVERT_FLOAT_TO_STRING(SPCDiameter, decimalNumber);
			
			SerialCommand sCommand;
			sCommand.hardwareType = INDICATOR;
			sCommand.command = "INDICATOR";
			sCommand.value = decimalNumber;
			
			BuildSerialOutput(&sCommand, serialOutputBuffer);
			newData = true;

			for (int i = 0; i < 52; i++)
			{
				rawSPC[i] = 0;
			}

			previousCount= loopCount;
			PORTC |= digitalPinToBitMask(INDICATOR_REQ); //set req high to restart ISR
		}
		

	}
	//else
	//{
	//retries++;
	//if (retries >= 52){
	//loopCount = 0;
	//retries = 0;
	//Serial.println("SPC ERROR");
	////PORTC &= ~digitalPinToBitMask(INDICATOR_REQ); //set req low to stop ISR
	////PORTC |= digitalPinToBitMask(INDICATOR_REQ); //set req high to restart ISR
	//}
	//}


	

	//for (int i = 0; i < 52;)
	//{
	//if (millis() > previousMillis + 5){
	//pinState = !(PINA & digitalPinToBitMask(clk)) == 0;
	//
	//if (pinState != lastPinState)
	//{
	//if (pinState == LOW)
	//{
	//rawSPC[i] = (PINA & digitalPinToBitMask(dat)) == 0 ? 48 : 49;
	//i++;
	//}
	//}
	//previousMillis = millis();
	//}
	//
	//loopCount++;
	//
	//if (loopCount >= 15000){
	//
	//SerialCommand sError;
	//sError.hardwareType = INDICATOR;
	//sError.command = "INDICATOR";
	//sError.value = "INDICATOR HAS BEEN DISCONNECTED";
	//char sErrorOutput [MAX_CMD_LENGTH] = {0};
	//BuildSerialOutput(&sError, sErrorOutput);
	//Serial.println(sErrorOutput);
	//return; //kicks out the loop if the SPC gets disconnected or has an error
	//}
	//
	//lastPinState = pinState;
	//}
	//
	//bool dataStreamValid = false;
	//for (unsigned int i = 0; i < 12; i++)
	//{
	//if (rawSPC[i] == 48) //48 is 0 (zero) in ascii
	//{
	//dataStreamValid = false;
	//
	//SerialCommand sError;
	//sError.hardwareType = INDICATOR;
	//sError.command = "INDICATOR";
	//sError.value = "A SPC PROCESSING ERROR HAS OCCURRED";
	//char sErrorOutput [MAX_CMD_LENGTH] = {0};
	//BuildSerialOutput(&sError, sErrorOutput);
	//Serial.println(sErrorOutput);
	//
	//break;
	//}
	//else {dataStreamValid = true;}
	//}
	//
	//if (dataStreamValid)
	//{
	//byte bytes[13] = {0};
	//for (int i = 0; i < 13; i++)
	//{
	//int idx = (i*4) + 4;
	//int bitPointer = 0;
	//for (int j = i * 4; j < idx ; j++)
	//{
	//bitWrite(bytes[i], bitPointer, rawSPC[j] == 49); //49 ascii for 1 //grab nibbles from rawSPC
	//bitPointer++;
	//}
	//}
	//
	//float preDecimalNumber = 0.0;
	//char buf[7];
	//
	//for(int i=0;i<6;i++){ //grab array positions 5-10 for diameter numbers
	//
	//buf[i]=bytes[i+5]+'0';
	//
	//buf[6]=0;
	//
	//preDecimalNumber=atol(buf); //assembled measurement, no decimal place added
	//}
	//
	//int decimalPointLocation = bytes[11];
	//
	//SPCDiameter = preDecimalNumber / (pow(10, decimalPointLocation)); //add decimal to number
	//
	//
	//
	//char decimalNumber[10] = {0};
	//CONVERT_FLOAT_TO_STRING(SPCDiameter, decimalNumber);
	//
	//SerialCommand sCommand;
	//sCommand.hardwareType = INDICATOR;
	//sCommand.command = "INDICATOR";
	//sCommand.value = decimalNumber;
	//
	//BuildSerialOutput(&sCommand, serialOutputBuffer);
	//newData = true;
	////Serial.println(serialOutputBuffer);
	//
	//}
	//

	
}

char *SpcProcessing::GetSerialOutputBuffer(void)
{
	newData = false;
	return serialOutputBuffer;
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


