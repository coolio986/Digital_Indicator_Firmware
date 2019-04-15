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
		RunScreenSimulation();
		return;
	}


	SPCDiameter = diameter;

	char decimalNumber[10] = {0};
	CONVERT_FLOAT_TO_STRING(SPCDiameter, decimalNumber);

	tft.setTextColor(TFT_GREEN, TFT_BLACK);
	tft.drawString(" Diameter: ", 0, 0, 4);
	tft.drawString(decimalNumber, 130, 0, 4);
	if(decimalNumber[5] == 0)
	{
	tft.drawString("      ", 193, 0, 4);
	}

	tft.drawString(" Diameter: ", 0, 25, 4);
	tft.drawString(decimalNumber, 130, 25, 4);

	tft.drawString(" Diameter: ", 0, 50, 4);
	tft.drawString(decimalNumber, 130, 50, 4);
	

	//tft.setTextColor(TFT_GREEN, TFT_BLACK);
	//
	//tft.setTextSize(3);
	//tft.setCursor(0, 0);
	//tft.print(F("Diameter: "));
	//tft.println(SPCDiameter);
	//tft.print(F("Diameter: "));
	//tft.println(SPCDiameter);
	//tft.print(F("Diameter: "));
	//tft.println(SPCDiameter);
	
	
}



void Screen::RunScreenSimulation(){

	
	
	tft.setTextColor(TFT_GREEN, TFT_BLACK);
	//tft.setTextColor(TFT_GREEN);
	tft.setTextSize(3);
	tft.setCursor(0, 0);
	tft.print(F("Diameter: 1.7"));
	i = i>9 ? 0 : i;
	tft.println(i++);
	
}


// default destructor
Screen::~Screen()
{
} //~Screen
