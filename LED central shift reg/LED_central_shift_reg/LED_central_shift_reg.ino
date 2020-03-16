#include <IRremote.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//IR config
#define IR_SIG_pin 2
IRrecv IR(IR_SIG_pin);

//LCD
LiquidCrystal_I2C lcd(0x3F,16,2);

//serial register connection
#define SR_MR_pin 7
#define SR_SHCP_pin 6
#define SR_STCP_pin 5
#define SR_OE_pin 4
#define SR_DS_pin 3


//data
byte SR_data = 0xFF; //Shift reg data

unsigned long IR_data; //IR data
decode_results results; //decode IR data

uint8_t hours, minutes, seconds; //DS3231 data


void IR_change_value();


void setup()
{
	//serial register wait for other devices
	pinMode( SR_MR_pin, OUTPUT );
	pinMode( SR_OE_pin, OUTPUT );
	digitalWrite( SR_MR_pin, LOW );
	digitalWrite( SR_OE_pin, HIGH );

	//IR
	IR.enableIRIn();

	//RTC
	Wire.begin();
	Wire.beginTransmission(0x68);
	Wire.write(0x0E);
	Wire.write(0b00011100);
	Wire.endTransmission();

	//LCD
	lcd.init();
	lcd.backlight();
	lcd.print("Działam23");

	//set register
	pinMode( SR_SHCP_pin, OUTPUT );
	pinMode( SR_DS_pin, OUTPUT );
	pinMode( SR_STCP_pin, OUTPUT );

	//SERIAL
	Serial.begin(9600);
  
	//serial register start work
	digitalWrite( SR_MR_pin, HIGH );
	digitalWrite( SR_OE_pin, LOW );
}

void loop()
{
	//Print config of reg
	Serial.println(SR_data, BIN);

	//sent data to register
	digitalWrite( SR_STCP_pin, LOW );
	shiftOut( SR_DS_pin, SR_SHCP_pin, MSBFIRST, SR_data );
	digitalWrite( SR_STCP_pin, HIGH );

	//IR recive
	if (IR.decode(&results)) // sprawdza, czy otrzymano sygnał IR
	{
		IR_data = results.value;
		Serial.println(IR_data, HEX);
		IR.resume();
		delay(100);
	}

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

		//conversion of data (for more look to spec of RTC)
		seconds = (((seconds & 0b11110000) >> 4) * 10 + (seconds & 0b00001111));
		minutes = (((minutes & 0b11110000) >> 4) * 10 + (minutes & 0b00001111));
		hours = (((hours & 0b00100000) >> 5) * 20 + ((hours & 0b00010000) >> 4) * 10 + (hours & 0b00001111));
	}

	IR_change_value();
}

void IR_change_value()
{
	switch(IR_data)
	{
		case 0xFFA25D:  //CH-   0xFFA25D
			break;
		case 0xFF629D:  //CH  0xFF629D
			break;
		case 0xFFE21D:  //CH+   0xFFE21D
			break;
		case 0xFF22DD:  //<<  0xFF22DD
			break;
		case 0xFF02FD:  //>>  0xFF02FD
			break;
		case 0xFFC23D:  //>||   0xFFC23D
			break;
		case 0xFFE01F:  //–   0xFFE01F
			break;
		case 0xFFA857:  //+   0xFFA857
			break;
		case 0xFF906F:  //EQ  0xFF906F
			break;
		case 0xFF9867:  //100+  0xFF9867
			SR_data = 0x70;
			break;
		case 0xFFB04F:  //200+  0xFFB04F
			SR_data = 0x00;
			break;
		case 0xFF6897:  //0   0xFF6897
			SR_data = 0xFF;
			break;
		case 0xFF30CF:  //1   0xFF30CF
			SR_data = (SR_data & 0b11111110) + ((SR_data + 1) & 0x01);
			break;
		case 0xFF18E7:  //2   0xFF18E7
			SR_data = (SR_data & 0b11111101) + ((SR_data + 2) & 0x02);
			break;
		case 0xFF7A85:  //3   0xFF7A85
			break;
		case 0xFF10EF:  //4   0xFF10EF
			break;
		case 0xFF38C7:  //5   0xFF38C7
			break;
		case 0xFF5AA5:  //6   0xFF5AA5
			break;
		case 0xFF42BD:  //7   0xFF42BD
			break;
		case 0xFF4AB5:  //8   0xFF4AB5
			break;
		case 0xFF52AD:  //9   0xFF52AD
			break;
	}
}
