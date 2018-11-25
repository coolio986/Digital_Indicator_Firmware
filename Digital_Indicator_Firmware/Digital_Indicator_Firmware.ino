/*===========================================================
* Project: Digital_Indicator_Firmware
* Developled using Arduino v1.8.5
* Developed by: Anthony Kaul(3D Excellence LLC) in collaboration with Filabot (Triex LLC)
* This firmware converts SPC (Statistical Process Control) into signals that can be recognized by a PC
* All rights reserved


* Version information:
* v1.2 - beta
* ===========================================================*/

#include "serialProcessing.h"
#include "hardwareTypes.h"


int req = 3; //mic REQ line goes to pin 5 through q1 (arduino high pulls request line low)
int dat = 2; //mic Data line goes to pin 2
int clk = 0; //mic Clock line goes to pin 3


String dataStream;
uint32_t spcTimeDelay = 100;

serialCommand sCommand;

// set this to the hardware serial port you wish to use
#define HWSERIAL Serial1


void setup()
{
  Serial.begin(9600);
  pinMode(req, OUTPUT);
  pinMode(clk, INPUT_PULLUP);
  pinMode(dat, INPUT_PULLUP);
  
  HWSERIAL.begin(115200);
  HWSERIAL.setTimeout(100);
  digitalWrite(req, HIGH); // set request at high via transistor
}

void loop()
{
  RunSPCDataLoop();
  CheckSerialCommands();
    
  delay(50);
}

void CheckSerialCommands()
{
  
  if (Serial.available() > 0 ) 
  {
    char * usbData = CheckSerial(Serial);
    sCommand = GetSerialArgs(usbData, hardwareTypes);

    switch (sCommand.hardwareType)
    {
      case 0:
        break;
      case 1:
        break;
      case 2:
        HWSERIAL.println(sCommand.value);
        Serial.print("sent to spooler: ");
        Serial.println(sCommand.value);
        break;
      case 3:
        break;
      default:
        break;
      
    }
    
  }
    
  if (HWSERIAL.available() > 0) {
    char * serial1Data = CheckSerial(HWSERIAL);

    Serial.print("2;");
    Serial.println(serial1Data);
    
  }
}


void RunSPCDataLoop()
{
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
        Serial.print("3;");
        Serial.println(dataStream);
    }
  
    dataStream = "";
    digitalWrite(req, LOW);
}


  
