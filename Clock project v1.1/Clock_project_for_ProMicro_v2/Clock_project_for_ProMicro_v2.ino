#include <HID.h>

#include "i2c.h"
#include "i2c_BMP280.h"

/*
 Connection list
 Arduino <--> RTC DS3231 (sign on my board)
 SDA     <--> SDA (D)
 SCL     <--> SCL (C)
 3.3V    <--> Vin (+) //2.3V-5.5V
 GND     <--> GND (-)

 Arduino <--> BMP280
 SDA     <--> SDA
 SCL     <--> SCL
 3.3V    <--> Vcc //3V-5V
 GND     <--> GND 
 */


//Data
uint8_t hours, minutes, seconds; //data RTC (DS3231)
float temperature = 0, preasure = 0; //data BMP280
byte brightness = 0x02; //brightness of display (TM1637)
uint8_t data[4]; //data for display (TM1637)


//BMP280
BMP280 bmp280;

//DO ZMIANY
void showTempAndPres()
{
  
}

//Setup of elements
void setup()
{
  Serial.begin(9600);

  //Button
  pinMode(20, INPUT_PULLUP);

  //RTC
  Wire.begin();
  Wire.beginTransmission(0x68);
  Wire.write(0x0E);
  Wire.write(0b00011100);
  Wire.endTransmission();

  //BMP 280
  bmp280.initialize();
  bmp280.setEnabled(0);
  bmp280.triggerMeasurement();
}

//Operating system
void loop()
{
//reading data form RTC
  Wire.beginTransmission(0x68); //It's an address of RTC DS3231
  Wire.write(0);
  Wire.endTransmission();
  Wire.requestFrom(0x68, 3);

  while(Wire.available())
  {
    seconds = Wire.read();
    minutes = Wire.read();
    hours = Wire.read();

    //conversion of data (for more look to spec od RTC)
    seconds = (((seconds & 0b11110000) >> 4) * 10 + (seconds & 0b00001111));
    minutes = (((minutes & 0b11110000) >> 4) * 10 + (minutes & 0b00001111));
    hours = (((hours & 0b00100000) >> 5) * 20 + ((hours & 0b00010000) >> 4) * 10 + (hours & 0b00001111));

  }

//reading data from BMP 280
  bmp280.awaitMeasurement();
  bmp280.getTemperature(temperature);
  bmp280.getPressure(preasure);
  bmp280.triggerMeasurement();

Serial.print(hours); Serial.print(":"); Serial.print(minutes); Serial.print(":"); Serial.println(seconds);
Serial.print("t: "); Serial.print(temperature); Serial.print(" p: "); Serial.println(preasure);

//sending data on dispaly with dots turn on/off 1 per second
//DO NAPISANIA

  if(digitalRead(10) == LOW)
    showTempAndPres();
}
