/*===========================================================
* Project: Digital_Indicator_Firmware
* Developled using Arduino v1.8.5
* Developed by: Anthony Kaul(3D Excellence LLC) in collaboration with Filabot (Triex LLC)
* This firmware converts SPC (Statistical Process Control) into signals that can be recognized by a PC
* All rights reserved


* Version information:
* v1.2 - beta
* ===========================================================*/

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "SerialProcessing.h"
#include "hardwareTypes.h"
#include "SpcProcessing.h"
#include "Screen.h"
#include "Device_Configuration.h"

#define INCLUDE_vTaskDelay   1
#define configUSE_PREEMPTION 1

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
void TaskCheckSerialCommands( void *pvParameters );
void TaskRunSimulation(void *pvParameters );
//End of Auto generated function prototypes by Atmel Studio

//int req = 3; //mic REQ line goes to pin 5 through q1 (arduino high pulls request line low)
//int dat = 2; //mic Data line goes to pin 2
//int clk = 0; //mic Clock line goes to pin 3

// These are used to get information about static SRAM and flash memory sizes
extern "C" char __data_start[];    // start of SRAM data
extern "C" char _end[];     // end of SRAM data (used to check amount of SRAM this program's variables use)
extern "C" char __data_load_end[];  // end of FLASH (used to check amount of Flash this program's code and data uses)


bool SIMULATIONACTIVE = true; //sets default value for simulation


SerialCommand sCommand;
SpcProcessing spcProcessing;
Screen screen;
SerialProcessing serialProcessing;



void setup()
{
	Serial.begin(SERIAL_BAUD);
	
	
	spcProcessing.init();
	screen.init();
	serialProcessing.init();

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

	xTaskCreate(
	TaskCheckSerialCommands
	,  (const portCHAR *)"CheckSerialCommands"   // A name just for humans
	,  500  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );

	xTaskCreate(
	TaskRunSimulation
	,  (const portCHAR *)"RunSimulation"   // A name just for humans
	,  500  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );


	
}

void loop()
{
	//nothing to do here, all done in RTOS tasks
}

void TaskCheckSPC(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	

	for (;;) // A Task shall never return or exit.
	{
		spcProcessing.RunSPCDataLoop();
		vTaskDelay( 1000 / portTICK_PERIOD_MS); // wait for one second
	}

}

void TaskCheckSerialExpander(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	
	for (;;) // A Task shall never return or exit.
	{
		//serialPortExpander.RunSerialExpanderDataLoop();
		vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for one second
	}

}

void TaskCheckSerialCommands(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	
	for (;;) // A Task shall never return or exit.
	{
		//CheckSerialCommands();
		serialProcessing.CheckSerialCommands();
		vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for one second
	}

}

void TaskRunScreen(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	UBaseType_t uxHighWaterMark;
	
	
	for (;;) // A Task shall never return or exit.
	{
		screen.IsInSimulationMode = SIMULATIONACTIVE;
		screen.UpdateScreen();
		
		uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
	}

}

void TaskRunSimulation(void *pvParameters)  // This is a task.
{
	for (;;) // A Task shall never return or exit.
	{
		if (spcProcessing.IsInSimulationMode != SIMULATIONACTIVE)
		{
			spcProcessing.IsInSimulationMode = SIMULATIONACTIVE;
		}
		vTaskDelay( 50 / portTICK_PERIOD_MS  ); // wait for one second
	}
}
