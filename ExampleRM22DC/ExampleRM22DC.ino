//#include <SPI.h>
#include "RM22DC.h"

//uint8_t activatePin = 3;
//unsigned int mydata = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);//initialize Serial
  
}

void loop() {
  // put your main code here, to run repeatedly:
  //SerialDebugReadRM22DC(mydata, activatePin);
  //printBin(activatePin);
  Serial.println("hello world");
  unsigned int data=0;
  ReadRM22DC(data, 3);
  //Serial.println(data);
}
