/*

  Made by Bram van Berlo and Calvin Kho: Programming end assigment CreaTe Module 6 - 13/01/2016
  This code is a modified version of the example code "Software Serial Example" on the arduino website:
  https://www.arduino.cc/en/Tutorial/SoftwareSerialExample

*/

//Software and hardware serial Object declaration
#include <SoftwareSerial.h>
SoftwareSerial incomingBluetoothData(10, 11); // TX, RX

void setup() {

  //Software and hardware serial Object init
  Serial.begin(9600);
  incomingBluetoothData.begin(38400);

  //Pull the keypin high for being able to connect wire for AT command mode if necessary
  pinMode(9, OUTPUT);
  digitalWrite(9, HIGH);
}

void loop() {
  
  //Check if the hardware serial connection works
  if (incomingBluetoothData.available()) {

    //Analyze incoming data for different ints and floats
    int sensorType = incomingBluetoothData.parseInt();
    int logCount = incomingBluetoothData.parseInt();
    float dataX = incomingBluetoothData.parseFloat();
    float dataY = incomingBluetoothData.parseFloat();
    float dataZ = incomingBluetoothData.parseFloat();

    //Variables to cast the float variables to
    int dataXcast, dataYcast, dataZcast;

    //Conditionals in order to mimic floor and ceiling behavior
    if (dataX >= 0) {dataXcast = (int) dataX + 0.5;}
    else if (dataX < 0) {dataXcast = (int) dataX - 0.5;}
    
    if (dataY >= 0) {dataYcast = (int) dataY + 0.5;}
    else if (dataY < 0) {dataYcast = (int) dataY - 0.5;}
    
    if (dataZ >= 0) {dataZcast = (int) dataZ + 0.5;}
    else if (dataZ < 0) {dataZcast = (int) dataZ - 0.5;}

    //Cast all int values to string variables
    String value0 = (String) sensorType;
    String value1 = (String) logCount;
    String value2 = (String) dataXcast;
    String value3 = (String) dataYcast;
    String value4 = (String) dataZcast;

    //Connect all values after eachother into one string with delimiter &
    String ofInformation = value0 + " " + value1 + " " + value2 + " " + value3 + " " + value4 + "&";

    //Send values over the software serial port
    Serial.print(ofInformation);
  }

  //The following code can be used in order to send/receive AT commands/information:
  /*
    // Keep reading from HC-05 and send to Arduino Serial Monitor
    if (incomingBluetoothData.available())
      Serial.write(incomingBluetoothData.read());

    // Keep reading from Arduino Serial Monitor and send to HC-05
    if (Serial.available())
      incomingBluetoothData.write(Serial.read());
  */
}
