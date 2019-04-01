/*Begining of Auto generated code by Atmel studio */
#include <Arduino.h>

/*End of auto generated code by Atmel studio */


/*===========================================================
* Project: Digital_Indicator_Firmware
* Developled using Arduino v1.8.5
* Developed by: Anthony Kaul(3D Excellence LLC) in collaboration with Filabot (Triex LLC)
* This firmware converts SPC (Statistical Process Control) into signals that can be recognized by a PC
* All rights reserved


* Version information:
* v1.2 - beta
* ===========================================================*/
#include <Arduino_FreeRTOS.h>
#include "serialProcessing.h"
#include "hardwareTypes.h"
#include "SpcProcessing.h"
#include "SerialPortExpander.h"
#include "SPI.h"
#include "TFT_ILI9341.h"
//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio


//Beginning of Auto generated function prototypes by Atmel Studio
//End of Auto generated function prototypes by Atmel Studio






//Beginning of Auto generated function prototypes by Atmel Studio
void CheckSerialCommands();
void RunSPCDataLoop();
int CheckInteralCommands(char* code);
int CheckSpoolerCommands(char* code);
int PrintRandomDiameterData();
void PrintRandomRPMData();
bool startsWith(const char* pre, const char* str);
void TaskCheckSPC( void *pvParameters );
void TaskCheckSerialExpander( void *pvParameters );
void TaskRunScreen( void *pvParameters );
//End of Auto generated function prototypes by Atmel Studio




//int req = 3; //mic REQ line goes to pin 5 through q1 (arduino high pulls request line low)
//int dat = 2; //mic Data line goes to pin 2
//int clk = 0; //mic Clock line goes to pin 3

// These are used to get information about static SRAM and flash memory sizes
extern "C" char __data_start[];    // start of SRAM data
extern "C" char _end[];     // end of SRAM data (used to check amount of SRAM this program's variables use)
extern "C" char __data_load_end[];  // end of FLASH (used to check amount of Flash this program's code and data uses)

// Use hardware SPI
TFT_ILI9341 tft = TFT_ILI9341();


bool IsInSimulationMode;

String dataStream;
uint32_t spcTimeDelay = 100;

serialCommand sCommand;

#define HWSERIAL Serial1


SpcProcessing _spcProcessing;
SerialPortExpander _serialPortExpander;



void setup()
{
  Serial.begin(SERIAL_BAUD);
  //HWSERIAL.begin(SERIAL_BAUD);
  //HWSERIAL.setTimeout(100);
  
  _spcProcessing.Setup();
  _serialPortExpander.Setup();
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(3);

  xTaskCreate(
  TaskCheckSPC
  ,  (const portCHAR *)"CheckSPC"   // A name just for humans
  ,  500  // This stack size can be checked & adjusted by reading the Stack Highwater
  ,  NULL
  ,   2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
  ,  NULL );
//
  xTaskCreate(
  TaskCheckSerialExpander
  ,  (const portCHAR *)"CheckSerialExpander"   // A name just for humans
  ,  500  // This stack size can be checked & adjusted by reading the Stack Highwater
  ,  NULL
  ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
  ,  NULL );

  xTaskCreate(
  TaskRunScreen
  ,  (const portCHAR *)"RunScreen"   // A name just for humans
  ,  500  // This stack size can be checked & adjusted by reading the Stack Highwater
  ,  NULL
  ,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
  ,  NULL );


  
}

void loop()
{
  //RunSPCDataLoop();
  _spcProcessing.IsInSimulationMode = IsInSimulationMode;
  
  
  

  //CheckSerialCommands();

  
  //delay(10);
}

void TaskCheckSPC(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  
  
  for (;;) // A Task shall never return or exit.
  {
    _spcProcessing.RunSPCDataLoop();
    vTaskDelay( 50 / portTICK_PERIOD_MS); // wait for one second
     
     
  }

}

void TaskCheckSerialExpander(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  
  for (;;) // A Task shall never return or exit.
  {
    _serialPortExpander.RunSerialExpanderDataLoop();
    vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for one second
  }

}

void TaskRunScreen(void *pvParameters)  // This is a task.
{
  (void) pvParameters;
  UBaseType_t uxHighWaterMark;
  int i = 0;
  
  for (;;) // A Task shall never return or exit.
  {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    //tft.setTextColor(TFT_GREEN);
    tft.setTextSize(3);
    tft.setCursor(0, 0);
    tft.print(F("Diameter: 1.7"));
    i = i>9 ? 0 : i;
    tft.println(i++);
    vTaskDelay( 50 / portTICK_PERIOD_MS  ); // wait for one second
    
    
    uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
  }

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
    HWSERIAL.println(code);
  }
  return 0;
}



void PrintRandomRPMData()
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



