# ReadSensorRM22DC
 my code to use the RLS RM22DC sensor with arduino

Need critical debugging for the headers and for the exampleRM22DC.
copy the functions from read_rls_sensor_v1_2.1.ino.

SPI.h need to be included, also the global variable and the define need to be added.
the ReadRM22DC(void *data, int ActivatePin) function recive the data and the pin to activate the comunication with the sensor.
a circuit to activate the sensor is needed. route the wire like inm the picture.
the function return Unsigned int (16 bit) as follow:
bit 0 (lsb) and bit 1 are the error status of the sensor 11 = OK, and 00, 01, 10 = critical error.
bit 2 to bit 15 are the postion data from the sensor.

the function needs to be called twice, and there compere the two data recived to check if the data is correct and valid, if the two results are not the same, the function will be called again twice, and so on until the two results are the same.

for faster reading, the print functions should be commented.

in the future a better validation will be implemented by using the CRC data from the sensor.