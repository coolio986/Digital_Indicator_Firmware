/*
* spcProcessing.cpp
*
* Created: 12/20/2018 4:57:59 PM
* Author: Anthony
*/


#include "SpcProcessing.h"
#include <Arduino.h>
#include "hardwareTypes.h"


// default constructor
SpcProcessing::SpcProcessing()
{

  
} //spcProcessing

void SpcProcessing::Setup(void)
{
  pinMode(req, OUTPUT);
  pinMode(clk, INPUT_PULLUP);
  pinMode(dat, INPUT_PULLUP);
  

  digitalWrite(req, HIGH); // set request at high via transistor
}

void SpcProcessing::RunSPCDataLoop(void)
{
  currentMillis = millis();

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
      Serial.print(INDICATOR);
      Serial.print(";");
      Serial.println(dataStream);
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
  
  Serial.print("3;111111111111111111110000");
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
