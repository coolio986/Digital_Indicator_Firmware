#ifndef __HARDWARETYPES_H__
#define __HARDWARETYPES_H__


const int INTERNALDEVICE = 1;
const int SPOOLER = 2; //spooler
const int INDICATOR = 3; //indicator
const int EXTRUDER = 4; //extruder

typedef struct{
  
  uint16_t internal; //internal
  uint16_t spooler; //spooler
  uint16_t indicator; //indicator
  uint16_t extruder; //extruder
} struct_hardwareTypes;

const static struct_hardwareTypes hardwareType =
{
  .internal = INTERNALDEVICE,
  .spooler = SPOOLER,
  .indicator = INDICATOR,
  .extruder = EXTRUDER  
};

static uint16_t int_hardwareTypes[] =
{
  
  INTERNALDEVICE, // 1 = internal
  SPOOLER, //2 = spooler
  INDICATOR, //3 = indicator
  EXTRUDER, //4 = extruder
};

#endif//__HARDWARETYPES_H__
