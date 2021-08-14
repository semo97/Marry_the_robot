/*
  Name:    getVescValues.ino
  Created: 19-08-2018
  Author:  SolidGeek
  Description:  This example is made using a Arduino Micro (Atmega32u4) that has a HardwareSerial port (Serial1) seperated from the Serial port.
                A Arduino Nano or Uno that only has one Serial port will not be able to display the data returned.
*/

#include <VescUart.h>

/** Initiate VescUart class */
VescUart UART;

void setup() {

  /** Setup Serial port to display data */
  Serial.begin(115200);

  /** Setup UART port (Serial1 on Atmega32u4) */
  Serial2.begin(115200);

  while (!Serial) {
    ;
  }

  /** Define which ports to use as UART */
  UART.setSerialPort(&Serial2);
  //UART.setDebugPort(&Serial);
  //UART.getVescValues();

}
void loop() {

  /** Call the function getVescValues() to acquire data from VESC */
  if ( UART.getVescValues() ) {
    Serial.println("Data From Main");
    Serial.println(UART.data.tempFet);
    Serial.println(UART.data.inpVoltage);
    Serial.println(UART.data.ampHours);
    Serial.println(UART.data.tachometerAbs);
    Serial.println();
  }
  else
  {
    Serial.println("Failed to get data from Main!");
  }

  if ( UART.getVescValues(58) ) {
    Serial.println("Data From Slave 58");
    Serial.println(UART.data.tempFet);
    Serial.println(UART.data.inpVoltage);
    Serial.println(UART.data.ampHours);
    Serial.println(UART.data.tachometerAbs);
    Serial.println();
  }
  else
  {
    Serial.println("Failed to get data from 58!");
  }

  delay(1000);
}
