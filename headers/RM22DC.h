#ifndef RM22DC
#define RM22DC
// include the SPI library:
#include <SPI.h>

#define NumOfBytesForSensor 4

//uint8_t rx_buffer[NumOfBytesForSensor];
uint32_t ArrangeByteTo32Bit(uint8_t *DataByte);
void ClearArray(uint8_t *DataByte);
unsigned int ReadRM22DC(void *data, int ActivatePin);
unsigned int SerialDebugReadRM22DC(void *data, int ActivatePin);
void printBin(byte arg);
void printBin32(uint32_t arg);
#endif