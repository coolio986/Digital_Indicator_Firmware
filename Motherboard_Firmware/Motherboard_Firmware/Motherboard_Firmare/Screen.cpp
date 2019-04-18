/*
* Screen.cpp
*
* Created: 4/3/2019 11:10:52 AM
* Author: Anthony
*/

#include "Screen.h"
#include "SPI.h"
#include "TFT_ILI9341.h"
#include "DataConversions.h"
#include "Error.h"

Screen *Screen::firstInstance;
int i = 0;

// Use hardware SPI
TFT_ILI9341 tft = TFT_ILI9341();



// default constructor
Screen::Screen()
{
	if (!firstInstance)
	{
		firstInstance = this;
	}
} //Screen

void Screen::init(){
	tft.init();
	tft.fillScreen(TFT_BLACK);
	tft.setRotation(3);
}

void Screen::UpdateScreen(float diameter){

	if (IsInSimulationMode){
		//		RunScreenSimulation();
		//		return;
	}

	if (SPCDiameter != diameter) //update screen only if necessary
	{
		SPCDiameter = diameter;
	}
	else
	{
		return;
	}

	char filamentDiameter[10] = {0};
	char wheelSpeed[10] = {0};
	char spoolSpeed[10] = {0};

	char randomNum[5] = {0};
	
	CONVERT_NUMBER_TO_STRING(INT_FORMAT, random(0, 1000), randomNum);
	

	for (int i = 0; i < 4; i++)
	{
		wheelSpeed[i] = randomNum[i];
		spoolSpeed[i] = randomNum[i];
	}
	
	
	CONVERT_FLOAT_TO_STRING(SPCDiameter, filamentDiameter);

	tft.setTextColor(TFT_GREEN, TFT_BLACK);
	tft.drawString(" Diameter: ", 0, 0, 4); //drawString faster then println
	tft.drawString(filamentDiameter, 150, 0, 4);
	if(filamentDiameter[5] == 0)
	{
		tft.drawString("    mm", 213, 0, 4);
	}

	
	tft.drawString(" Wheel RPM: ", 0, 25, 4);
	tft.drawString(wheelSpeed, 150, 25, 4);

	if (wheelSpeed[1] == 0){
		tft.drawString("    ", 165, 25, 4);
	}
	if (wheelSpeed[2] == 0){
		tft.drawString("    ", 178, 25, 4);
	}


	tft.drawString(" Spool RPM: ", 0, 50, 4);
	tft.drawString(spoolSpeed, 150, 50, 4);

	if (spoolSpeed[1] == 0){
		tft.drawString("    ", 165, 50, 4);
	}
	if (spoolSpeed[2] == 0){
		tft.drawString("    ", 178, 50, 4);
	}
}

void Screen::AddError(Error *eError)
{
	
}

void Screen::DisplayError(Error *eError)
{
	for (int i = 0; i < 10; i++)
	{
		errors[i] = eError[i];
	}

	//errors = *eError;
}

void Screen::ClearError(void)
{
	//error.errorLevel = 0;

}



// default destructor
Screen::~Screen()
{
} //~Screen
