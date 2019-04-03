#include "serialProcessing.h"
#include <Arduino.h>
#include "board.h"
#include "hardwareTypes.h"
#include <stdio.h>
#include "DataConversions.h"


template<size_t SIZE, class T> inline size_t array_size(T (&arr)[SIZE]);

char *CheckSerial(HardwareSerial *port)
{
HardwareSerial _port = *port;
  static byte ndx = 0;
  char endMarker = '\r';
  char rc;
  boolean newData = false;
  //receivedChars[numChars]; // an array to store the received data
  
  while (port->available() > 0 && newData == false) {
    rc = port->read();
    if (rc != endMarker)
    {
      receivedChars[ndx] = rc;
      ndx++;
      if (ndx >= numChars)
      {
        ndx = numChars - 1;
      }
    }
    
    else
    {
      
      receivedChars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
  }

  //if (newData){
  //  port.println(receivedChars);
  //}

  while (port->available() > 0){port->read();} //flush
    

  return receivedChars;
}


char * CleanseData(char *data)
{
  char endMarker = 0x00; //null character
  boolean newData = false;
  char *chars2 = (char*)malloc(numChars);
  char chars[numChars] = {};

  for (byte i = 0; i < numChars; i++)
  {
    if (data[i] != endMarker && newData == false )
    {
      
      if (data[i] != 0x0A)
      {
        chars[i] = data[i];
      }
      else
      {
        chars[i] = 0x20;
      }
    }
    
    else
    {
      i = 0;
      chars2 = chars;
      //Serial.println(chars2);
      newData = true;
      return chars2;
    }
  }
  
  return chars2;
}




SerialCommand GetSerialArgs(char *serialData)
{
  SerialCommand c;
  char delim[] = ";";
  char *ptr = strtok(serialData, delim);
  uint16_t loopCounter = 0;
  char *splitStrings[10] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
  
  while(ptr != NULL)
  {
    if (loopCounter <= 10){ splitStrings[loopCounter] = ptr;}
    ptr = strtok(NULL, delim);
    loopCounter++;
  }
  //for (int i = loopCounter; i < 10; i++)
  //{
	//splitStrings[i] = NULL; //null out rest of point array
  //}

  if (!ExistsInIntArray(int_hardwareTypes, array_size(int_hardwareTypes), atoi(splitStrings[0]))){return c;}
  
  
  c.hardwareType = atoi(splitStrings[0]);
  c.command = splitStrings[1];
  c.value = splitStrings[2];
  
  return c;

}

bool ExistsInIntArray(uint16_t *arrayToCheck, size_t arraySize, uint16_t numberToCheck)
{
	

  for (unsigned int i = 0; i < arraySize; i++)
  {
	  if (arrayToCheck[i] == numberToCheck)
	  {
		  return true;
	  }
  }
  return false;
}

void BuildSerialOutput(SerialCommand *sCommand, char *outputBuffer)
{
	sprintf(outputBuffer, OUTPUT_STRING_FORMAT, sCommand->hardwareType, sCommand->command, sCommand->value); 
}

template<size_t SIZE, class T> inline size_t array_size(T (&arr)[SIZE]) {
	return SIZE;
}

