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

    A
  +---+
  |   |
 B| H |C
  +---+
  |   |
 D| E |G
  +---+ oF
*/

#define A 5
#define B 6
#define C 9
#define D 19
#define E 18
#define F 15
#define G 14
#define H 16
#define D1 4
#define D2 7
#define D3 8
#define D4 10

//Data
uint8_t hours, minutes, seconds; //data RTC (DS3231)
float temperature = 0, preasure = 0; //data BMP280
uint8_t data[4] = {0,0,0,0}; //data for display (TM1637)


//BMP280
BMP280 bmp280;

char encodeDigit(int value)
{
  switch ( value )
  {
    case 0:
      return 0b11111010;
    case 1:
      return 0b00100010;
    case 2:
      return 0b10111001;
    case 3:
      return 0b11011001;
    case 4:
      return 0b01100011;
    case 5:
      return 0b11001011;
    case 6:
      return 0b11011011;
    case 7:
      return 0b10100010;
    case 8:
      return 0b11111011;
    case 9:
      return 0b11101011;
      
    default:
      return 0b00000000;
  }
}

void printData(uint8_t data[4])
{
  for ( int t = 0; t < 100000; t++ )
    for ( unsigned int bit_pos = 1; bit_pos <= 127; bit_pos *= 2 )
    {
      digitalWrite(A, HIGH);
      if(data[0]&bit_pos != 0)
        digitalWrite(D1, LOW);
      if(data[1]&bit_pos != 0)
        digitalWrite(D2, LOW);
      if(data[2]&bit_pos != 0)
        digitalWrite(D3, LOW);
      if(data[3]&bit_pos != 0)
        digitalWrite(D4, LOW);
        
      digitalWrite(B, LOW);
      if(data[0]&bit_pos != 0)
        digitalWrite(D1, HIGH);
      if(data[1]&bit_pos != 0)
        digitalWrite(D2, HIGH);
      if(data[2]&bit_pos != 0)
        digitalWrite(D3, HIGH);
      if(data[3]&bit_pos != 0)
        digitalWrite(D4, HIGH);
    }
}




//DO ZMIANY
void showTempAndPres()
{
  readingBMP();

  //show temperature (example 23,4 show as "23 4")
  data[0] = encodeDigit( (int) temperature / 10 );
  data[1] = encodeDigit( (int) temperature % 10);
  data[2] = 0x00;
  data[3] = encodeDigit( (int) (temperature * 10.00) % 10);
  printData(data);

  //show preasure in hPa
  preasure = preasure /100;

  data[0] = encodeDigit( (int) preasure / 1000);
  data[1] = encodeDigit( (int) (preasure / 100) % 10);
  data[2] = encodeDigit( (int) (preasure / 10) % 10);
  data[3] = encodeDigit( (int) preasure % 10);
  printData(data);  
}

//Setup of elements
void setup()
{
  Serial.begin(9600);

  Serial.println("poczatek sutup");

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

void readingBMP()
{
//reading data from BMP 280
  bmp280.awaitMeasurement();
  bmp280.getTemperature(temperature);
  bmp280.getPressure(preasure);
  bmp280.triggerMeasurement();

  Serial.print("t: "); Serial.print(temperature); Serial.print(" p: "); Serial.println(preasure);
}

//Operating system
void loop()
{

Serial.println("Poczaek loopa");
  
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
  
  Serial.print(hours); Serial.print(":"); Serial.print(minutes); Serial.print(":"); Serial.println(seconds);

  printData(data);
  
  if(digitalRead(10) == LOW)
    showTempAndPres();
}
