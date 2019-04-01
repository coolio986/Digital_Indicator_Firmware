/* 
* SerialPortExpander.h
*
* Created: 12/27/2018 10:18:01 AM
* Author: Anthony
*/


#ifndef __SERIALPORTEXPANDER_H__
#define __SERIALPORTEXPANDER_H__

#include <Arduino.h>

class SerialPortExpander
{
//variables
public:
protected:
private:
  byte computer_bytes_received = 0;    //We need to know how many characters bytes have been received
  byte sensor_bytes_received = 0;      //We need to know how many characters bytes have been received
  const static byte numberOfBufferBytes = 32;
  int s1 = 9;                         //Arduino pin 9 to control pin S1
  int s2 = 8;                         //Arduino pin 8 to control pin S2
  int s3 = 7;                         //Arduino pin 7 to control pin S3
  int port = 0;                       //what port to open
  char computerdata[numberOfBufferBytes];               //A 20 byte character array to hold incoming data from a pc/mac/other
  char sensordata[numberOfBufferBytes];                 //A 30 byte character array to hold incoming data from the sensors
  char *channel;                       //Char pointer used in string parsing
  char *cmd;                           //Char pointer used in string parsing
  

//functions
public:
  SerialPortExpander();
  void Setup(void);
  void RunSerialExpanderDataLoop(void);
  ~SerialPortExpander();
protected:
private:
  SerialPortExpander( const SerialPortExpander &c );
  void Open_channel(void);
  SerialPortExpander& operator=( const SerialPortExpander &c );

}; //SerialPortExpander

#endif //__SERIALPORTEXPANDER_H__

