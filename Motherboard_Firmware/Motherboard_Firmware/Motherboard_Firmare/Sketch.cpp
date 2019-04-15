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
#include <semphr.h>  // add the FreeRTOS functions for Semaphores (or Flags).
#include "SerialProcessing.h"
#include "hardwareTypes.h"
#include "SpcProcessing.h"
#include "Screen.h"
#include "Device_Configuration.h"

#define INCLUDE_vTaskDelay   1
//#define configUSE_PREEMPTION 1

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
void checkSPC ();
//End of Auto generated function prototypes by Atmel Studio

//int req = 3; //mic REQ line goes to pin 5 through q1 (arduino high pulls request line low)
//int dat = 2; //mic Data line goes to pin 2
//int clk = 0; //mic Clock line goes to pin 3

// These are used to get information about static SRAM and flash memory sizes
extern "C" char __data_start[];    // start of SRAM data
extern "C" char _end[];     // end of SRAM data (used to check amount of SRAM this program's variables use)
extern "C" char __data_load_end[];  // end of FLASH (used to check amount of Flash this program's code and data uses)


bool SIMULATIONACTIVE = false; //sets default value for simulation


SerialCommand sCommand;
SpcProcessing spcProcessing;
Screen screen;
SerialProcessing serialProcessing;

SemaphoreHandle_t xSerialSemaphore;
bool isLocked = false;

unsigned long previousMillisSPC = 0;

ISR (TIMER4_OVF_vect){

	if (millis() > previousMillisSPC + 50)
	{
		spcProcessing.RunSPCDataLoop();

		previousMillisSPC = millis();
	}

	TCNT4 = 100;
}




void setup()
{
	Serial.begin(SERIAL_BAUD);

	spcProcessing.init();
	screen.init();

	noInterrupts();

	//normal mode
	TCCR4A = 0x00;
	OCR4A   = 0;
	TCCR4B = (1<<CS40);
	TIMSK4 |= bit (TOIE4);

	
	
	interrupts();
	

	//if ( xSerialSemaphore == NULL )  // Check to confirm that the Serial Semaphore has not already been created.
	//{
	//xSerialSemaphore = xSemaphoreCreateMutex();  // Create a mutex semaphore we will use to manage the Serial Port
	//if ( ( xSerialSemaphore ) != NULL )
	//xSemaphoreGive( ( xSerialSemaphore ) );  // Make the Serial Port available for use, by "Giving" the Semaphore.
	//}


	//xTaskCreate(
	//TaskCheckSPC
	//,  (const portCHAR *)"CheckSPC"   // A name just for humans
	//,  500  // This stack size can be checked & adjusted by reading the Stack Highwater
	//,  NULL
	//,   5 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	//,  NULL );
	////
	//xTaskCreate(
	//TaskCheckSerialExpander
	//,  (const portCHAR *)"CheckSerialExpander"   // A name just for humans
	//,  500  // This stack size can be checked & adjusted by reading the Stack Highwater
	//,  NULL
	//,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	//,  NULL );
	//
	xTaskCreate(
	TaskRunScreen
	,  (const portCHAR *)"RunScreen"   // A name just for humans
	,  500  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  1 // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
	//
	//xTaskCreate(
	//TaskCheckSerialCommands
	//,  (const portCHAR *)"CheckSerialCommands"   // A name just for humans
	//,  500  // This stack size can be checked & adjusted by reading the Stack Highwater
	//,  NULL
	//,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	//,  NULL );
	//
	//xTaskCreate(
	//TaskRunSimulation
	//,  (const portCHAR *)"RunSimulation"   // A name just for humans
	//,  500  // This stack size can be checked & adjusted by reading the Stack Highwater
	//,  NULL
	//,  2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	//,  NULL );

	//xTaskCreate(
	//TaskCheckSPC
	//,  (const portCHAR *)"CheckSPC"   // A name just for humans
	//,  500  // This stack size can be checked & adjusted by reading the Stack Highwater
	//,  NULL
	//,   2  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	//,  NULL );

	//sei(); // Enable global interrupt

}

void loop()
{
	//nothing to do here, all done in RTOS tasks

	//spcProcessing.RunSPCDataLoop();
	//delay(100);
}

void TaskCheckSPC(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	UBaseType_t uxHighWaterMark;

	for (;;) // A Task shall never return or exit.
	{

		//if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 10 ) == pdTRUE )
		//{
		
		//spcProcessing.RunSPCDataLoop();
		
		

		//xSemaphoreGive( xSerialSemaphore ); // Now free or "Give" the Serial Port for others.


		
		//delay(1);
		//uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
		//Serial.print("Hiwater for TaskCheckSPC: ");
		//Serial.println(uxHighWaterMark);
		//}
		//delay(100);
		//vTaskDelay( 10 / portTICK_PERIOD_MS ); // wait for 100 milliseconds
		//vTaskDelay(1);
	}

}

void TaskCheckSerialExpander(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	
	for (;;) // A Task shall never return or exit.
	{
		//serialPortExpander.RunSerialExpanderDataLoop();
		vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for 100 milliseconds
	}

}

void TaskCheckSerialCommands(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	
	for (;;) // A Task shall never return or exit.
	{
		//CheckSerialCommands();
		serialProcessing.CheckSerialCommands();
		vTaskDelay( 100 / portTICK_PERIOD_MS ); // wait for 100 milliseconds
	}

}

void TaskRunScreen(void *pvParameters)  // This is a task.
{
	(void) pvParameters;
	UBaseType_t uxHighWaterMark;
	
	
	for (;;) // A Task shall never return or exit.
	{
		//screen.IsInSimulationMode = SIMULATIONACTIVE;
		//if ( xSemaphoreTake( xSerialSemaphore, ( TickType_t ) 11 ) == pdTRUE )
		//{
		screen.IsInSimulationMode = false;
		screen.UpdateScreen(spcProcessing.GetDiameter());

		//vTaskDelay(2);
		//xSemaphoreGive( xSerialSemaphore ); // Now free or "Give" the Serial Port for others.
		//}
		
		//vTaskDelay( 3); // wait for 100 milliseconds
		
		//uxHighWaterMark = uxTaskGetStackHighWaterMark( NULL );
		//
		//Serial.print("Hiwater for TaskRunScreen: ");
		//Serial.println(uxHighWaterMark);
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
		vTaskDelay( 50 / portTICK_PERIOD_MS  ); // wait for 50 milliseconds
	}
}

//void TaskCheckSPC(void *pvParameters)  // This is a task.
//{
//(void) pvParameters;
//
//
//for (;;) // A Task shall never return or exit.
//{
//checkSPC();
////TaskDelay( 1000 / portTICK_PERIOD_MS); // wait for one second
//}
//
//}


void checkSPC() { // get data from mic

	int req = 30; //mic REQ line goes to pin 5 through q1 (arduino high pulls request line low)

	int dat = 28; //mic Data line goes to pin 2

	int clk = 29; //mic Clock line goes to pin 3

	int i = 0; int j = 0; int k = 0;

	byte mydata[14];

	float num;



	digitalWrite(req, HIGH); // generate set request

	for(i = 0; i < 13; i++ ) {

		k = 0;

		for (j = 0; j < 4; j++) {

			while( digitalRead(clk) == LOW) {// hold until clock is high

			}

			while( digitalRead(clk) == HIGH) {// hold until clock is low

			}

			bitWrite(k, j, (digitalRead(dat) & 0x1)); // read data bits, and reverse order )

		}

		// extract data

		mydata[i] = k;
	}

	// sign = mydata[4];

	// decimal = mydata[11];

	// units = mydata[12];

	// assemble measurement from bytes

	char buf[7];

	for(int lp=0;lp<6;lp++){

		buf[lp]=mydata[lp+5]+'0';

		buf[6]=0;

		num=atol(buf); //assembled measurement, no decimal place added

		Serial.println(num/1000, 3); //add decimal
		//Serial.println("test");

	}
}