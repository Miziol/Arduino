#include <HID.h>

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
uint8_t hours, minutes, seconds; //data RTC (DS3231)
float temperature = 0, preasure = 0; //data BMP280
byte brightness = 0x02; //brightness of display (TM1637)
uint8_t data[4]; //data for display (TM1637)


//Display
#define CLK 14
#define DIO 15

TM1637Display display(CLK, DIO);

//BMP280
BMP280 bmp280;

//FUNCTION
void changeBrightness()
{
  //display "bri" and actual level of brightness
  data[0] = 0b01111100;
  data[1] = 0b01010000;
  data[2] = 0b00010000;
  data[3] = display.encodeDigit( brightness );
  display.setSegments(data);

  delay(1000); //waiting for user stop presing the button
  
  for(int i = 0; i < 2000; i++)
  {
    if(digitalRead(10) == LOW) //if user press button
    {
      brightness = (brightness + 1) % 8; //brightness level +1
      i = 0;

      //display "bri" and actual level of brightness
      display.setBrightness(brightness);
      data[0] = 0b01111100;
      data[1] = 0b01010000;
      data[2] = 0b00010000;
      data[3] = display.encodeDigit( brightness );
      display.setSegments(data);

      //waiting for user stop presing the button
      delay(500);
    }

    delay(1);
  }
}

void showTempAndPres()
{
  //show temperature (example 23,4 show as "23 4")
  data[0] = display.encodeDigit( (int) temperature / 10 );
  data[1] = display.encodeDigit( (int) temperature % 10);
  data[2] = 0x00;
  data[3] = display.encodeDigit( (int) (temperature * 10.00) % 10);
  display.setSegments(data);
  delay(3000); //display show value for 3 seconds

  if(digitalRead(10) == LOW) //if button pressed go to change brightness menu
    changeBrightness();

  //show preasure in hPa
  preasure = preasure /100;

  data[0] = display.encodeDigit( (int) preasure / 1000);
  data[1] = display.encodeDigit( (int) (preasure / 100) % 10);
  data[2] = display.encodeDigit( (int) (preasure / 10) % 10);
  data[3] = display.encodeDigit( (int) preasure % 10);
  display.setSegments(data);
  delay(3000);//display show value for 3 seconds
}




//Setup of elements
void setup()
{
  Serial.begin(9600);

  //Button
  pinMode(10, INPUT_PULLUP);
  
  //Display
  display.setBrightness(brightness);

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
  data[0] = hours/10 == 0 ? 0x00 : display.encodeDigit(hours/10);
  data[1] = display.encodeDigit(hours % 10) + ( seconds << 7 );
  data[2] = display.encodeDigit(minutes / 10);
  data[3] = display.encodeDigit(minutes % 10);
  display.setSegments(data);
  delay(100);

  if(digitalRead(10) == LOW)
    showTempAndPres();
}
