#include "RM22DC.h"
/*
  * SDI - to digital pin 11 (MOSI pin)
  * CLK - to digital pin 13 (SCK pin)
*/
#include <SPI.h>
//#include <HardwareSerial.h>
//#include <Arduino.h>
//#include <stdint.h>
#define NumOfBytesForSensor 4


uint8_t rx_buffer[NumOfBytesForSensor];

uint32_t ArrangeByteTo32Bit(uint8_t *DataByte)
{
    uint32_t Data32Bit = 0;
    Data32Bit = ((uint32_t)DataByte[0] << 24) | ((uint32_t)DataByte[1] << 16) | ((uint32_t)(DataByte[2]) << 8) | DataByte[3];
    Data32Bit = Data32Bit << 6u;//remove ACK, START, and CDS bits
    Data32Bit = Data32Bit >> 18u;//remove CRC bits and right align the data
    return Data32Bit;
}
void ClearArray(uint8_t *DataByte)
{
    for (int i = 0; i < NumOfBytesForSensor; i++)
    {
        DataByte[i] =0;  // clear the data array
    }
    return NULL;
}
unsigned int ReadRM22DC(void *data, int ActivatePin)
{
    digitalWrite(ActivatePin, HIGH);    //activate the max485 for reading and writing
    uint32_t SensorRead[2]={0xFFFFFFFF, 0x00000000};
    uint8_t DataByte[NumOfBytesForSensor] ={0};
    int alternate =0;
    while (SensorRead[0] != SensorRead[1])  //only if the two last read are the same, the data is valid
    {
        ClearArray(DataByte);
        SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
        for (int i = 0; i < NumOfBytesForSensor; i++)
        {
            DataByte[i] = SPI.transfer(0);   //read the data from the sensor
            //int a = printBin(DataByte[i]);  //print the data in binary
        }
        
        SPI.endTransaction();   //close SPI transaction
        
        SensorRead[alternate ^ 1]= ArrangeByteTo32Bit(DataByte); //arrange the data to 32 bit
        
        //roughly 4kHz sampling rate
        delay(250);
        delayMicroseconds(83);
        if((SensorRead[alternate & 0x01]& 0x00000003) != 0x00000003)  //sensor error bits is not valid
        {
            digitalWrite(ActivatePin, LOW); //deactivate the max485
            return (unsigned int) SensorRead[alternate & 0x01]& 0x00000003; //return the error bits
        }
        alternate ^= 1;//alternate between 0 and 1
    }
    digitalWrite(ActivatePin, LOW); //deactivate the max485
    return (unsigned int) SensorRead[0];    //return the data
}

unsigned int SerialDebugReadRM22DC(void *data, int ActivatePin)
{
    
    digitalWrite(ActivatePin, HIGH);// activate the max485 for reading and writing
    SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    uint8_t byte_0 = SPI.transfer(0);   //read firt 8 bits
    uint8_t byte_1 = SPI.transfer(0);   //read second 8 bits
    uint8_t byte_2 = SPI.transfer(0);   //read third 8 bits
    uint8_t byte_3 = SPI.transfer(0);   //read fourth 8 bits
    SPI.endTransaction();   //close SPI transaction
    Serial.print("raw data:\n");    //make clear that the next data is the raw data
    printBin(byte_0);   //print the byte_0 data in binary
    Serial.print(" ");  //print space
    printBin(byte_1);
    Serial.print(" ");
    printBin(byte_2);
    Serial.print(" ");
    printBin(byte_3);
    Serial.print(" ");
    Serial.print("\n"); //print new line
    //arrange the data to 32 bit
    uint32_t receivedData = ((uint32_t)byte_0 << 24) | ((uint32_t)byte_1 << 16) | ((uint32_t)(byte_2) << 8) | byte_3; 
    receivedData = receivedData << 6u;  //remove ACK, START, and CDS bits
    receivedData = receivedData >> 18u; //remove CRC bits and right align the data
    Serial.print("receivedData:\n"); //make clear that the next data is the received data
    printBin32(receivedData);   //print the received data in binary
    Serial.print("\n");
    //roughly 4kHz sampling rate
    delay(250);
    delayMicroseconds(83);
    return (unsigned int) receivedData;   //return the data
}

int printBin(void *arg)
{
    for (int8_t aBit = 7; aBit >= 0; aBit--)
    {
        Serial.write(bitRead((uint8_t)arg, aBit) ? '1' : '0');
    }
    Serial.print("-----\n");
    return 0;
}

void printBin32(uint32_t arg)
{
    for (int8_t abyte =3; abyte >= 0; abyte--)
    {
        for (int8_t aBit = 7; aBit >= 0; aBit--)
        {
            Serial.write(bitRead(arg, aBit + abyte*8) ? '1' : '0');
            Serial.print(" ");
        }
    }
}