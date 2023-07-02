//#ifndef RM22DC
#define RM22DC
// include the SPI library:
//#include <Arduino.h>
//#include <SPI.h>
#include <stdint.h>
#define NumOfBytesForSensor 4

//uint8_t rx_buffer[NumOfBytesForSensor];
extern uint32_t ArrangeByteTo32Bit(uint8_t *DataByte);
extern void ClearArray(uint8_t *DataByte);
extern unsigned int ReadRM22DC(void *data, int ActivatePin);
extern unsigned int SerialDebugReadRM22DC(void *data, int ActivatePin);
extern int printBin(void *arg);
extern void printBin32(uint32_t arg);
//#endif