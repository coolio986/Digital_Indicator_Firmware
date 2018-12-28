#include "serialProcessing.h"
#include <Arduino.h>
#include "board.h"
#include "hardwareTypes.h"

#ifdef LEONARDO
#include <SoftwareSerial.h>
#endif

#ifdef TEENSY20
#include <usb_api.h>
#endif



//static char *receivedChars;
#ifdef TEENSY20

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
#endif // TEENSY20

char * CheckSerial(HardwareSerial port)
{
  static byte ndx = 0;
  char endMarker = '\r';
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


#ifdef LEONARDO
char * CheckSerial(Serial_ port)
{
  static byte ndx = 0;
  char endMarker = '\n';
  char rc;
  boolean newData = false;
  //receivedChars[numChars]; // an array to store the received data
  
  if (port.available() > 0 && newData == false) {
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

char * CheckSerial(SoftwareSerial *port)
{
  byte ndx = 0;
  char endMarker = '\n';
  char rc;
  boolean newData = false;
  char *chars2 = malloc(numChars);
  char chars[numChars] = {};
  
  
  
  while (port->available() > 0 && newData == false) {
    
    rc = port->read();
    
    if (rc != endMarker)
    {
      chars[ndx] = rc;
      ndx++;
      if (ndx >= numChars)
      {
        ndx = numChars - 1;
      }
    }
    
    else
    {
      
      chars[ndx] = '\0'; // terminate the string
      ndx = 0;
      newData = true;
    }
    
  }
  chars2 = chars;
  if (newData){
    //Serial.println(chars2);
  }
  //Serial.println(chars);
  return chars2;
}

char * CleanseData(char *data)
{
  char endMarker = 0x00; //null character
  boolean newData = false;
  char *chars2 = malloc(numChars);
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

#endif


serialCommand GetSerialArgs(char * serialData)
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

  if (!ExistsInIntArray(int_hardwareTypes, atoi(splitStrings[0]))){return c;}
  
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
