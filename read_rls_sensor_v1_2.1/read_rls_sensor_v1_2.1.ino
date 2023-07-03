/*
  * SDI - to digital pin 11 (MOSI pin)
  * CLK - to digital pin 13 (SCK pin)
*/
// include the SPI library:
#include <SPI.h>

#define numOfBytes 4


uint8_t rx_buffer[numOfBytes];
int activatePin = 2;
void setup() {
  //initialize Serial
  Serial.begin(9600);
  // initialize SPI:
  SPI.begin();
  pinMode(activatePin, OUTPUT);
  digitalWrite(activatePin, LOW);
  pinMode(3, INPUT_PULLUP);
}

void loop() {
  
  if(digitalRead(3))
  {
  unsigned int mydata1 = 0;
  unsigned int mydata2 = 0;
  //TODO: loop to compere the two datas
  ReadRM22DC(mydata1, activatePin);
  ReadRM22DC(mydata2, activatePin);
  }
  

}

void printBin(byte arg) {
  for (int8_t aBit = 7; aBit >= 0; aBit--)
    Serial.write(bitRead(arg, aBit) ? '1' : '0');
}
void printBin32(uint32_t arg) {
  for (int8_t abyte =3; abyte >= 0; abyte--)
  {
    for (int8_t aBit = 7; aBit >= 0; aBit--)
      Serial.write(bitRead(arg, aBit + abyte*8) ? '1' : '0');
    Serial.print(" ");
  }
}

unsigned int ReadRM22DC(void *data, int ActivatePin)
{
  digitalWrite(ActivatePin, HIGH);    //activate the max485 for reading and writing
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
    uint8_t byte_0 = SPI.transfer(0);  // read firt 8 bits
    uint8_t byte_1 = SPI.transfer(0);  // read second 8 bits
    uint8_t byte_2 = SPI.transfer(0);
    uint8_t byte_3 = SPI.transfer(0);
  SPI.endTransaction();// close SPI transaction
  Serial.print("raw data:\n");
  printBin(byte_0);
  Serial.print(" ");
  printBin(byte_1);
  Serial.print(" ");
  printBin(byte_2);
  Serial.print(" ");
  printBin(byte_3);
  Serial.print(" ");
  Serial.print("\n");
  uint32_t receivedData = ((uint32_t)byte_0 << 24) | ((uint32_t)byte_1 << 16) | ((uint32_t)(byte_2) << 8) | byte_3;
  receivedData = receivedData << 6u;
  receivedData = receivedData >> 18u;
  Serial.print("received Data:\n");
  printBin32(receivedData);
  Serial.print("\n");
  delay(250);
  digitalWrite(ActivatePin, LOW);    //deactivate the max485 for reading and writing
  return (unsigned int)receivedData;
}
