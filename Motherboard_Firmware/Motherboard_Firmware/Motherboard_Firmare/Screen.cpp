/*
* Screen.cpp
*
* Created: 4/3/2019 11:10:52 AM
* Author: Anthony
*/


#include "Screen.h"
#include "SPI.h"
#include "TFT_ILI9341.h"

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

void Screen::UpdateScreen(){

	if (IsInSimulationMode){
		RunScreenSimulation();
	}

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
