/* 
* Screen.h
*
* Created: 4/3/2019 11:10:52 AM
* Author: Anthony
*/


#ifndef __SCREEN_H__
#define __SCREEN_H__
#include <Arduino.h>


class Screen
{
//variables
public:
	
	bool IsInSimulationMode;
protected:
private:

//functions
public:
	Screen();
	~Screen();
	void init();
	void UpdateScreen();
protected:
private:
	Screen( const Screen &c );
	Screen& operator=( const Screen &c );
	static Screen *firstInstance;
	void RunScreenSimulation();

}; //Screen

#endif //__SCREEN_H__
