#include <TM1637Display.h>
#include <Wire.h>

#include "i2c.h"
#include "i2c_BMP280.h"

/*
 Connection list
 Arduino <--> RTC DS3231 (sign on my board)
 SDA     <--> SDA (D)
 SCL     <--> SCL (C)
 3.3V    <--> Vin (+) //2.3V-5.5V
 GND     <--> GND (-)

 Arduino <--> TM1637
 3       <--> DIO
 2       <--> CLK
 5V      <--> Vcc //3V-5V
 GND     <--> GND

 Arduino <--> BMP280
 SDA     <--> SDA
 SCL     <--> SCL
 3.3V    <--> Vcc //3V-5V
 GND     <--> GND 
 */


//Data
uint8_t hours, minutes, seconds;
float temperature, preasure;


//Display
#define CLK 2
#define DIO 3

TM1637Display display(CLK, DIO);

//BMP280
BMP280 bmp280;

//Setup of elements
void setup()
{
  Serial.begin(9600);
  
  //Display
  display.setBrightness(0x00);

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
//data init
  uint8_t data[4];

//reading data form RTC
  Wire.beginTransmission(0x68); //It's an address od RTC DS3231
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


  

//sending data on dispaly with dots turn on/off 1 per second
  data[0] = hours/10 == 0 ? 0x00 : display.encodeDigit(hours/10);
  data[1] = display.encodeDigit(hours % 10) + ( seconds << 7 );
  data[2] = display.encodeDigit(minutes / 10);
  data[3] = display.encodeDigit(minutes % 10);
  display.setSegments(data);
  delay(2000);


Serial.println(temperature);

//sending data on display (temperature)
  data[0] = 0x00;
  data[1] = display.encodeDigit( (int) temperature / 10);
  data[2] = display.encodeDigit( (int) temperature % 10);
  data[3] = 0x00;
  display.setSegments(data);
  delay(1000);

  
}
