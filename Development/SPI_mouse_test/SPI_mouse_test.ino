// Define the chip select pin
const int chipSelectPin = 10;
const int xVelPin = 5;
const int yVelPin = 20;
int xCum = 0;
int yCum = 0;
int xVel = 0;
int yVel = 0;
  
// the sensor communicates using SPI, so include the library:
#include <SPI.h>

//Sensor's memory register addresses:
//const int Delta_X = 0x03;      //3 most significant bits of pressure
//const int Delta_Y = 0x04;  //16 least significant bits of pressure
//const int SQUAL = 0x05;   //16 bit temperature reading

void setup() {
  Serial.begin(9600);

  pinMode(chipSelectPin, OUTPUT);
  digitalWrite(chipSelectPin,HIGH);
  analogWriteFrequency(xVelPin,187500);
  analogWriteFrequency(yVelPin,187500);
  analogWriteResolution(8);
  // start the SPI library:
  SPI.begin();
  // give the sensor time to set up:
  delay(100);
  SPI.setDataMode(SPI_MODE3);
  delay(100);
  SPI.setBitOrder(MSBFIRST);
  delay(1000);
  analogWriteResolution(8);
  //byte currentConfig = readRegister(0x0a);
  //Serial.println("Initial Config is: " + String(currentConfig));
  //writeRegister(0x0a, currentConfig | 0x10);
  //delay(1);
  //currentConfig = readRegister(0x0a);
  //Serial.println("New Config is: " + String(currentConfig));
  //Serial.println("Increase from 9 to 25 corresponds to high cpi mode operation");
  Serial.println("Startup Complete");
  delay(1000);
}

void loop() {
    delay(10);
    //Read the temperature data
    //byte currentConfig = readRegister(0x0a);
    //delay(1);
    //writeRegister(0x0a, currentConfig | 0x10);
    //delay(1);
    byte Motion = ((readRegister(0x02) & (1 << 8-1)) != 0);
    if (Motion == 1){
      xVel = readRegister(0x03);
      yVel = readRegister(0x04); 
      if (xVel > 127){
        xVel = xVel-256;
      }
      if (yVel > 127){
        yVel = yVel-256;
      }
      xCum = xCum + xVel;
      yCum = yCum + yVel;
     }
      else {
        xVel = 0;
        yVel = 0;
      }
    Serial.println("ID is " + String(readRegister(0x00), HEX));   
    //Serial.println("Current Config is " + String(currentConfig));
    Serial.println("Squal = " + String(readRegister(0x05)));
    Serial.println("Motion Status : " + String(Motion));
    Serial.println("intX = " + String(xCum));
    Serial.println("intY = " + String(yCum));
    analogWrite(xVelPin, (xVel+128));
    analogWrite(yVelPin, (yVel+128));
}

byte readRegister(byte address){ 
  //Take CS pin low to select chip
  digitalWrite(chipSelectPin,LOW);
  //Send register address and value
  SPI.transfer(address & 0x7f);
  delayMicroseconds(100);
  byte data = SPI.transfer(0x00);
  delayMicroseconds(1);
  digitalWrite(chipSelectPin,HIGH);
  return data;
}

void writeRegister(byte address, byte data){
  digitalWrite(chipSelectPin,LOW);
  SPI.transfer(address | 0x80);  
  SPI.transfer(data);
  delayMicroseconds(20);
  digitalWrite(chipSelectPin,HIGH);
}
