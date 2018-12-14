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


#ifdef LEONARDO
#include <SoftwareSerial.h>

#endif



int req = 3; //mic REQ line goes to pin 5 through q1 (arduino high pulls request line low)
int dat = 2; //mic Data line goes to pin 2
int clk = 0; //mic Clock line goes to pin 3


//****SoftwareSerial1 port pins****//
#define SS1_RX 8
#define SS1_TX 9

bool IsInSimulationMode;

String dataStream;
uint32_t spcTimeDelay = 100;

serialCommand sCommand;

// set this to the hardware serial port you wish to use
#ifdef TEENSY20
#define HWSERIAL Serial1
#endif

#ifdef LEONARDO
SoftwareSerial *HWSERIAL;

#endif


void setup()
{
  Serial.begin(115200);
  pinMode(req, OUTPUT);
  pinMode(clk, INPUT_PULLUP);
  pinMode(dat, INPUT_PULLUP);

  
  
  HWSERIAL = &SoftwareSerial(SS1_RX, SS1_TX);
  
  pinMode(SS1_RX, INPUT);
  pinMode(SS1_TX, OUTPUT);

  HWSERIAL->begin(SERIAL_BAUD);
  HWSERIAL->listen();
  #ifdef TEENSY20
  HWSERIAL.setTimeout(100);
  #endif
  
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
    sCommand = GetSerialArgs(usbData);
    
    (sCommand.hardwareType == hardwareType.internal) && (CheckInteralCommands(sCommand.value));

    (sCommand.hardwareType == hardwareType.spooler) &&
    (
      //HWSERIAL.println(sCommand.value);
      //Serial.print("sent to spooler: ");
      //Serial.println(sCommand.value);
      CheckSpoolerCommands(sCommand.value)
    );

    (sCommand.hardwareType == hardwareType.indicator) && (0);

    (sCommand.hardwareType == hardwareType.extruder) && (0);

    //if (sCommand.hardwareType == hardwareType.internal)
    //{
    //CheckInteralCommands(sCommand.value);
    //}

    
    //case struct_hardwareTypes.spooler:
    //HWSERIAL.println(sCommand.value);
    //Serial.print("sent to spooler: ");
    //Serial.println(sCommand.value);
    //break;
    //
    //case struct_hardwareTypes.indicator:
    //break;
    //
    //default:
    //break;
    //
    //}
    
  }
  

  if (!IsInSimulationMode)
  {
  
    //if (HWSERIAL->available() > 0) {
           
      char * serialData = CheckSerial(HWSERIAL);
      if (strlen(serialData) > 1) 
      {
        Serial.print(hardwareType.spooler);
        Serial.print(";");
        Serial.print(serialData);
        Serial.println(";");
      }  
      
    
  //}
  }
}

void RunSPCDataLoop()
{

  if (IsInSimulationMode)
  {
    PrintRandomDiameterData();
  }
  else
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
}

int CheckInteralCommands(char *code)
{
  //Serial.println("sim mode routine");
  if (strcmp(sCommand.value, "IsInSimulationMode = true") == 0)
  {
    //Serial.println("sim mode active");
    IsInSimulationMode = true;
  }
  if (strcmp(sCommand.value, "IsInSimulationMode = false") == 0)
  {
    //Serial.println("sim mode inactive");
    IsInSimulationMode = false;
  }
  return 0;
}
int CheckSpoolerCommands(char *code)
{
  
  if (IsInSimulationMode){
    if (startsWith("getrpm", code))
    {
      PrintRandomRPMData();
    }
    
  }
  else
    {
       
      HWSERIAL->println(code);
      
      
    }
  return 0;
}

int PrintRandomDiameterData()
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

int PrintRandomRPMData()
{
  long rpm = random(10, 15);

  Serial.print(hardwareType.spooler);
  Serial.print(";getrpm = ");
  Serial.print(rpm);
  Serial.println(";");
}

bool startsWith(const char *pre, const char *str)
{
  size_t lenpre = strlen(pre),
  lenstr = strlen(str);
  return lenstr < lenpre ? false : strncmp(pre, str, lenpre) == 0;
}
