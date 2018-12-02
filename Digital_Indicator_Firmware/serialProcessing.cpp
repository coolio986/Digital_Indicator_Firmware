#include "serialProcessing.h"
#include <Arduino.h>
#include <usb_api.h>


//static char *receivedChars;

char * CheckSerial(usb_serial_class port)
{
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
  boolean newData = false;
  //receivedChars[numChars]; // an array to store the received data
  
  while (port.available() > 0 && newData == false) {
    rc = port.read();
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
  
  return receivedChars;
}

char * CheckSerial(HardwareSerial port)
{
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
  boolean newData = false;
  //receivedChars[numChars]; // an array to store the received data
  
  while (port.available() > 0 && newData == false) {
    rc = port.read();
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

  return receivedChars;
}

serialCommand GetSerialArgs(char * serialData, uint16_t hardwareTypes[])
{
  serialCommand c;
  char delim[] = ";";
  char *ptr = strtok(serialData, delim);
  uint16_t loopCounter = 0;  
  char *splitStrings[10];
  
  while(ptr != NULL)
  {
    if (loopCounter <= 10){ splitStrings[loopCounter] = ptr;}
    ptr = strtok(NULL, delim);
    loopCounter++;
  }

  if (!ExistsInIntArray(hardwareTypes, atoi(splitStrings[0]))){return c;}
     
  c.hardwareType = atoi(splitStrings[0]);
  c.value = splitStrings[1];
  
  
  //Serial.println(c.hardwareType);
  //Serial.println(c.value);
  
  return c;

}

bool ExistsInIntArray(uint16_t *arrayToCheck, uint16_t numberToCheck)
{
   for (unsigned int i = 0; i < sizeof(arrayToCheck); i++)
   {
      if (arrayToCheck[i] == numberToCheck)
      {
          return true;
      }
    }
   return false;
}
