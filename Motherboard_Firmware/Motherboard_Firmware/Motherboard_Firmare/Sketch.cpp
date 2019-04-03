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
#include "SerialPortExpander.h"
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



bool SIMULATIONMODE = true; //sets default value for simulation
bool IsInSimulationMode;

SerialCommand sCommand;
SpcProcessing spcProcessing;
SerialPortExpander serialPortExpander;
Screen screen;



void setup()
{
	Serial.begin(SERIAL_BAUD);
	
	
	spcProcessing.init();
	serialPortExpander.init();
	screen.init();

	IsInSimulationMode = SIMULATIONMODE;
	

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
		CheckSerialCommands();
		vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for one second
	}

}

void TaskRunScreen(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	UBaseType_t uxHighWaterMark;
	
	
	for (;;) // A Task shall never return or exit.
	{
		screen.IsInSimulationMode = IsInSimulationMode;
		screen.UpdateScreen();
		
		uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
	}

}

void TaskRunSimulation(void *pvParameters)  // This is a task.
{
	for (;;) // A Task shall never return or exit.
	{
		if (spcProcessing.IsInSimulationMode != IsInSimulationMode)
		{
			spcProcessing.IsInSimulationMode = IsInSimulationMode;
		}
		vTaskDelay( 50 / portTICK_PERIOD_MS  ); // wait for one second
	}
}


void CheckSerialCommands()
{
	
	if (Serial.available() > 0 )
	{
		
		char *usbData = CheckSerial(&Serial);
		sCommand = GetSerialArgs(usbData);
		

		if(sCommand.hardwareType == hardwareType.internal)
		{
			CheckInteralCommands(sCommand.command);
		}

		if(sCommand.hardwareType == hardwareType.spooler)
		{
			CheckSpoolerCommands(sCommand.command);
		}

		if((sCommand.hardwareType > hardwareType.indicator) && (sCommand.hardwareType < hardwareType.internal))
		{
			//serialPortExpander.channel
			serialPortExpander.ProcessSerialExpander(&sCommand);
		}

		

		
		

		if (!IsInSimulationMode)
		{
			
			char * serialData = CheckSerial(&Serial);
			if (strlen(serialData) > 1)
			{
				Serial.print(sCommand.hardwareType);
				Serial.print(";");
				Serial.print(sCommand.command);
				Serial.println(";");
			}
		}
	}

	if (Serial1.available() > 0)
	{
		
	}

}



int CheckInteralCommands(char *code)
{
	//Serial.println("sim mode routine");
	if (strcmp(sCommand.command, "IsInSimulationMode") == 0)
	{
		
		IsInSimulationMode = strcmp(sCommand.value, "true") == 0;
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
		Serial.println(code);
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


