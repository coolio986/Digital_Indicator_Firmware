/*
* SerialPortExpander.cpp
*
* Created: 12/27/2018 10:18:01 AM
* Author: Anthony
*/


#include "SerialPortExpander.h"
#include "board.h"
#include <Arduino.h>
#include "serialProcessing.h"


// default constructor
SerialPortExpander::SerialPortExpander()
{
} //SerialPortExpander


void SerialPortExpander::Setup(void)
{
  pinMode(s1, OUTPUT);              //Set the digital pin as output
  pinMode(s2, OUTPUT);              //Set the digital pin as output
  pinMode(s3, OUTPUT);              //Set the digital pin as output
  Serial.begin(SERIAL_BAUD);               //Set the hardware serial port to 9600
  Serial.setTimeout(5);
  
  Serial1.begin(SERIAL_BAUD);
  Serial1.setTimeout(20);
  
}

void SerialPortExpander::RunSerialExpanderDataLoop(void)
{
  if (Serial.available() > 0)
  {
  
    computer_bytes_received = Serial.readBytesUntil(13, computerdata, numberOfBufferBytes); //We read the data sent from the serial monitor(pc/mac/other) until we see a <CR>. We also count how many characters have been received
    computerdata[computer_bytes_received] = 0; //We add a 0 to the spot in the array just after the last character we received.. This will stop us from transmitting incorrect data that may have been left in the buffer
    
  }
  if (computer_bytes_received != 0) {             //If computer_bytes_received does not equal zero
    channel = strtok(computerdata, ";");          //Let's parse the string at each colon
    cmd = strtok(NULL, ";");                      //Let's parse the string at each colon
    Open_channel();                               //Call the function "open_channel" to open the correct data path

    if (cmd != 0) {                               //If a command has been sent
      Serial1.print(cmd);
      Serial1.print("\r");
      //altSerial.print(cmd);                       //Send the command from the computer to the Atlas Scientific device using the softserial port
      //altSerial.print("\r");                      //After we send the command we send a carriage return <CR>
    }
    computer_bytes_received = 0;                  //Reset the var computer_bytes_received to equal 0
  }

  if (Serial1.available() > 0) {                 //If data has been transmitted from an Atlas Scientific device
    
    sensor_bytes_received = Serial1.readBytesUntil(13, sensordata, numberOfBufferBytes); //we read the data sent from the Atlas Scientific device until we see a <CR>. We also count how many character have been received
    sensordata[sensor_bytes_received] = 0;         //we add a 0 to the spot in the array just after the last character we received. This will stop us from transmitting incorrect data that may have been left in the buffer
    
    Serial.print((port + 1));
    Serial.print(";");
    
    char *serialData = CleanseData(sensordata);       //clense the data of unecessary newlines and reterminate
    Serial.println(serialData);                       //let’s transmit the data received from the Atlas Scientific device to the serial monitor
    
  }


}

void SerialPortExpander::Open_channel(void) 
{                             //This function controls what UART port is opened.

  port = atoi(channel);                           //Convert the ASCII char value of the port to be opened into an int
  if (port < 1 || port > 8)port = 1;              //If the value of the port is within range (1-8) then open that port. If it’s not in range set it port 1
  port -= 1;                                      //So, this device knows its ports as 0-1 but we have them labeled 1-8 by subtracting one from the port to be opened we correct for this.

  digitalWrite(s1, bitRead(port, 0));             //Here we have two commands combined into one.
  digitalWrite(s2, bitRead(port, 1));             //The digitalWrite command sets a pin to 1/0 (high or low)
  digitalWrite(s3, bitRead(port, 2));             //The bitRead command tells us what the bit value is for a specific bit location of a number
  delay(2);                                       //this is needed to make sure the channel switching event has completed
  return;                                         //go back
}

// default destructor
SerialPortExpander::~SerialPortExpander()
{
} //~SerialPortExpander