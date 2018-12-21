/*
* spcProcessing.cpp
*
* Created: 12/20/2018 4:57:59 PM
* Author: Anthony
*/


#include "spcProcessing.h"
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

  if (IsInSimulationMode)
  {
    PrintRandomDiameterData();
    return;
  }
  
  digitalWrite(req, HIGH); // generate set request


  //currentMillis = millis();
  for (int i = 0; i < 13; i++ )
  {
    

    for (int j = 0; j < 4; j++)
    {
      //*****Edge from high to low*****//
      uint32_t delayCounts = 0;
      while ( digitalRead(clk) == HIGH)
      {
        delayCounts++;
        if (delayCounts >= spcTimeDelay){ break;}
        delayMicroseconds(1);
      } // hold until clock is high

      delayCounts = 0;
      while ( digitalRead(clk) == LOW)
      {
        delayCounts++;
        if (delayCounts >= spcTimeDelay){ break;}
        delayMicroseconds(1);
      } // hold until clock is low
      //***************************//
      
      //bitWrite(k, j, (digitalRead(dat) & 0x1 )); // read data bits, and reverse order )
      //Serial.print(digitalRead(dat));
      dataStream += digitalRead(dat);

    }
  }
  
  bool dataStreamValid = false;
  for (unsigned int i = 0; i < dataStream.length(); i++)
  {
    if (dataStream[i] == 0)
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
