#include <IRremote.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

enum pilot
{
	ChanalDown, Chanal, ChanalUp,
	Left, Right, Play,
	Minus, Plus, EQ,
	Zero, Hundred, TwoHundred,
	One, Two, Three,
	Four, Five, Six,
	Seven, Eight, Nine
};

//IR config
#define IR_SIG_pin 2
IRrecv IR(IR_SIG_pin);

//LCD
LiquidCrystal_I2C lcd(0x3F,16,2);

//LED chanal connection
#define LED_1 3
#define LED_2 4
#define LED_3 5
#define LED_4 6
#define LED_5 7
#define LED_6 8
#define LED_7 9
#define LED_8 10

//data
#define display_options 2
bool change_display = true, display_working = true;
unsigned int display_state = 0;
int button, state = 0;
String time, reg;

byte LED_data = 0xFF; //data for LED

unsigned long IR_data; //IR data
decode_results results; //decode IR data

uint8_t hours, minutes, seconds; //DS3231 data
uint8_t new_hours, new_minutes, new_seconds;


void read_RTC();
void set_RTC();
void map_IR_code();
void set_LED();


void setup()
{
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
	lcd.print("Start");

	//set LED
	pinMode( LED_1, OUTPUT );
	pinMode( LED_2, OUTPUT );
	pinMode( LED_3, OUTPUT );
	pinMode( LED_4, OUTPUT );
	pinMode( LED_5, OUTPUT );
	pinMode( LED_6, OUTPUT );
	pinMode( LED_7, OUTPUT );
	pinMode( LED_8, OUTPUT );
	set_LED();
}

void loop()
{
	time = "    " + (hours < 10 ? (" " + String(hours)) : String(hours) ) + ":" + (minutes < 10 ? ("0" + String(minutes)) : String(minutes) ) + ":" + (seconds < 10 ? ("0" + String(seconds)) : String(seconds) );
	reg = "    " + String(LED_data, BIN);

	//set LED
	set_LED();

	//IR recive
	if (IR.decode(&results)) // sprawdza, czy otrzymano sygnał IR
	{
		IR_data = results.value;
		Serial.println(IR_data, HEX);
		IR.resume();
		map_IR_code();
	}

	//reading data form RTC
	read_RTC();

	//state logic
	if(change_display && display_working)
	{
		switch(display_state)
		{
			case 110:
				lcd.clear();
				lcd.print("    Settings");
				lcd.setCursor(0,1);
				lcd.print("1. Clock");
				lcd.noCursor();
				lcd.noBlink();
				break;
			case 111:
				lcd.clear();
				lcd.print("Hours");
				lcd.setCursor(1,1);
				lcd.cursor();
				lcd.blink();
				break;
			case 112:
				lcd.clear();
				lcd.print("Hours");
				lcd.setCursor(1,1);
				lcd.cursor();
				lcd.print(new_hours);
				lcd.blink();
				break;
			case 113:
				lcd.clear();
				lcd.print("Hours");
				lcd.setCursor(1,1);
				lcd.noCursor();
				lcd.print(new_hours);
				lcd.noBlink();
				break;
			case 114:
				lcd.clear();
				lcd.print("Minutes");
				lcd.setCursor(1,1);
				lcd.cursor();
				lcd.blink();
				break;
			case 115:
				lcd.clear();
				lcd.print("Minutes");
				lcd.setCursor(1,1);
				lcd.cursor();
				lcd.print(new_minutes);
				lcd.blink();
				break;
			case 116:
				lcd.clear();
				lcd.print("Minutes");
				lcd.setCursor(1,1);
				lcd.noCursor();
				lcd.print(new_minutes);
				lcd.noBlink();
				break;
			case 117:
				lcd.clear();
				lcd.print("Seconds");
				lcd.setCursor(1,1);
				lcd.cursor();
				lcd.blink();
				break;
			case 118:
				lcd.clear();
				lcd.print("Seconds");
				lcd.setCursor(1,1);
				lcd.cursor();
				lcd.print(new_seconds);
				lcd.blink();
				break;
			case 119:
				lcd.clear();
				lcd.print("Seconds");
				lcd.setCursor(1,1);
				lcd.noCursor();
				lcd.print(new_seconds);
				lcd.noBlink();
				break;
			case 1:
				lcd.clear();
				lcd.print(time);
				lcd.setCursor(0,1);
				lcd.print(reg);
				lcd.noCursor();
				lcd.noBlink();
				break;
			case 0:
			default:
				lcd.clear();
				lcd.print(time);
				lcd.setCursor(0,1);
				lcd.noCursor();
				lcd.noBlink();
				break;

		}

		change_display = false;
	}
	
	switch(state)
	{
		case 110:
			switch(button)
			{
				case EQ:
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Play:
					state = 0;
					display_state = 0;
					change_display = true;
					break;
				default:
					break;
			}
			break;
		case 111:
			switch(button)
			{
				case Zero:
					new_hours = 0;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case One:
					new_hours = 1;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Two:
					new_hours = 2;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Play:
					state = 110;
					display_state = 110;
					change_display = true;
					break;
				default:
					break;
			}
			break;
		case 112:
			if(new_hours == 2)
			{
				switch(button)
				{
					case Zero:
						new_hours = new_hours * 10 + 0;
						state = state + 1;
						display_state = display_state + 1;
						change_display = true;
						break;
					case One:
						new_hours = new_hours * 10 + 1;
						state = state + 1;
						display_state = display_state + 1;
						change_display = true;
						break;
					case Two:
						new_hours = new_hours * 10 + 2;
						state = state + 1;
						display_state = display_state + 1;
						change_display = true;
						break;
					case Three:
						new_hours = new_hours * 10 + 3;
						state = state + 1;
						display_state = display_state + 1;
						change_display = true;
						break;
					case Left:
						new_hours = new_hours / 10;
						state = state -1;
						display_state = display_state - 1;
						change_display = true;
						break;
					case Play:
						state = 110;
						display_state = 110;
						change_display = true;
						break;
					default:
						break;
				}
			}else{
				switch(button)
				{
					case Zero:
						new_hours = new_hours * 10 + 0;
						state = state + 1;
						display_state = display_state + 1;
						change_display = true;
						break;
					case One:
						new_hours = new_hours * 10 + 1;
						state = state + 1;
						display_state = display_state + 1;
						change_display = true;
						break;
					case Two:
						new_hours = new_hours * 10 + 2;
						state = state + 1;
						display_state = display_state + 1;
						change_display = true;
						break;
					case Three:
						new_hours = new_hours * 10 + 3;
						state = state + 1;
						display_state = display_state + 1;
						change_display = true;
						break;
					case Four:
						new_hours = new_hours * 10 + 4;
						state = state + 1;
						display_state = display_state + 1;
						change_display = true;
						break;
					case Five:
						new_hours = new_hours * 10 + 5;
						state = state + 1;
						display_state = display_state + 1;
						change_display = true;
						break;
					case Six:
						new_hours = new_hours * 10 + 6;
						state = state + 1;
						display_state = display_state + 1;
						change_display = true;
						break;
					case Seven:
						new_hours = new_hours * 10 + 7;
						state = state + 1;
						display_state = display_state + 1;
						change_display = true;
						break;
					case Eight:
						new_hours = new_hours * 10 + 8;
						state = state + 1;
						display_state = display_state + 1;
						change_display = true;
						break;
					case Nine:
						new_hours = new_hours * 10 + 9;
						state = state + 1;
						display_state = display_state + 1;
						change_display = true;
						break;
					case Left:
						new_hours = new_hours / 10;
						state = state -1;
						display_state = display_state - 1;
						change_display = true;
						break;
					case Play:
						state = 110;
						display_state = 110;
						change_display = true;
						break;
					default:
						break;
				}
			}
			break;
		case 113:
			switch(button)
			{
				case EQ:
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Left:
					new_hours = new_hours / 10;
					state = state -1;
					display_state = display_state - 1;
					change_display = true;
					break;
				case Play:
					state = 110;
					display_state = 110;
					change_display = true;
					break;
				default:
					break;
			}
			break;
		case 114:
			switch(button)
			{
				case Zero:
					new_minutes = 0;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case One:
					new_minutes = 1;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Two:
					new_minutes = 2;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Three:
					new_minutes = 3;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Four:
					new_minutes = 4;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Five:
					new_minutes = 5;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Play:
					state = 110;
					display_state = 110;
					change_display = true;
					break;
				default:
					break;
			}
			break;
		case 115:
			switch(button)
			{
				case Zero:
					new_minutes = new_minutes * 10 + 0;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case One:
					new_minutes = new_minutes * 10 + 1;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Two:
					new_minutes = new_minutes * 10 + 2;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Three:
					new_minutes = new_minutes * 10 + 3;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Four:
					new_minutes = new_minutes * 10 + 4;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Five:
					new_minutes = new_minutes * 10 + 5;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Six:
					new_minutes = new_minutes * 10 + 6;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Seven:
					new_minutes = new_minutes * 10 + 7;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Eight:
					new_minutes = new_minutes * 10 + 8;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Nine:
					new_minutes = new_minutes * 10 + 9;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Left:
					new_minutes = new_minutes / 10;
					state = state -1;
					display_state = display_state - 1;
					change_display = true;
					break;
				case Play:
					state = 110;
					display_state = 110;
					change_display = true;
					break;
				default:
					break;
			}
			break;
		case 116:
			switch(button)
			{
				case EQ:
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Left:
					new_minutes = new_minutes / 10;
					state = state -1;
					display_state = display_state - 1;
					change_display = true;
					break;
				case Play:
					state = 110;
					display_state = 110;
					change_display = true;
					break;
				default:
					break;
			}
			break;

		case 117:
			switch(button)
			{
				case Zero:
					new_seconds = 0;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case One:
					new_seconds = 1;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Two:
					new_seconds = 2;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Three:
					new_seconds = 3;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Four:
					new_seconds = 4;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Five:
					new_seconds = 5;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Play:
					state = 110;
					display_state = 110;
					change_display = true;
					break;
				default:
					break;
			}
			break;
		case 118:
			switch(button)
			{
				case Zero:
					new_seconds = new_seconds * 10 + 0;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case One:
					new_seconds = new_seconds * 10 + 1;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Two:
					new_seconds = new_seconds * 10 + 2;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Three:
					new_seconds = new_seconds * 10 + 3;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Four:
					new_seconds = new_seconds * 10 + 4;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Five:
					new_seconds = new_seconds * 10 + 5;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Six:
					new_seconds = new_seconds * 10 + 6;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Seven:
					new_seconds = new_seconds * 10 + 7;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Eight:
					new_seconds = new_seconds * 10 + 8;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Nine:
					new_seconds = new_seconds * 10 + 9;
					state = state + 1;
					display_state = display_state + 1;
					change_display = true;
					break;
				case Left:
					new_seconds = new_seconds / 10;
					state = state -1;
					display_state = display_state - 1;
					change_display = true;
					break;
				case Play:
					state = 110;
					display_state = 110;
					change_display = true;
					break;
				default:
					break;
			}
			break;
		case 119:
			switch(button)
			{
				case EQ:
					state = 0;
					display_state = 0;
					set_RTC();
					change_display = true;
					break;
				case Left:
					new_seconds = new_seconds / 10;
					state = state -1;
					display_state = display_state - 1;
					change_display = true;
					break;
				case Play:
					state = 110;
					display_state = 110;
					change_display = true;
					break;
				default:
					break;
			}
			break;
		case 0:
		default:
			switch(button)
			{
				case Zero:
					LED_data = 0xFF;
					break;
				case One:
					LED_data = (LED_data & 0b11111110) + ((LED_data + 1) & 0x01);
					break;
				case Two:
					LED_data = (LED_data & 0b11111101) + ((LED_data + 2) & 0x02);
					break;
				case Three:
					LED_data = (LED_data & 0b11111011) + ((LED_data + 4) & 0x04);
					break;
				case Four:
					LED_data = (LED_data & 0b11110111) + ((LED_data + 8) & 0x08);
					break;
				case Five:
					LED_data = (LED_data & 0b11101111) + ((LED_data + 16) & 0x10);
					break;
				case Six:
					LED_data = (LED_data & 0b11011111) + ((LED_data + 32) & 0x20);
					break;
				case Seven:
					LED_data = (LED_data & 0b10111111) + ((LED_data + 64) & 0x40);
					break;
				case Eight:
					LED_data = (LED_data & 0b01111111) + ((LED_data + 128) & 0x80);
					break;
				case Nine:
					LED_data = 0x00;
					break;
				case Left:
					display_state = (display_state - 1) % display_options;
					change_display = true;
					break;
				case Right:
					display_state = (display_state + 1) % display_options;
					change_display = true;
					break;
				case Play:
					if(display_working)
					{
						lcd.noDisplay();
						lcd.noBacklight();
						display_working = false;
					}else{
						lcd.backlight();
						lcd.display();
						display_working = true;
					}
					break;
				case EQ:
					state = 110;
					display_state = 110;
					change_display = true;
					break;
				case -1:
				default:
					break;
			}
	}

	button = -1;
}

void read_RTC()
{
	int seconds_old = seconds;
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

	if(seconds != seconds_old)
		change_display = true;
}

void set_RTC()
{
	new_hours = 0b00000000 + (((new_hours / 10) << 4) & 0b00110000) + ((new_hours % 10) & 0b00001111);;
	new_minutes = 0b00000000 + (((new_minutes / 10) << 4) & 0b01110000) + ((new_minutes % 10) & 0b00001111);
	new_seconds = 0b00000000 + (((new_seconds / 10) << 4) & 0b01110000) + ((new_seconds % 10) & 0b00001111);
	//HOURS
	Wire.beginTransmission(0x68);
	Wire.write(0x02);
	Wire.write(new_hours);
	Wire.endTransmission();
	//MINUTES
	Wire.beginTransmission(0x68);
	Wire.write(0x01);
	Wire.write(new_minutes);
	Wire.endTransmission();
	//SECONDS
	Wire.beginTransmission(0x68);
	Wire.write(0x00);
	Wire.write(new_seconds);
	Wire.endTransmission();
}

void map_IR_code()
{
	switch(IR_data)
	{
		case 0xFFA25D:  //CH-   0xFFA25D
			button = ChanalDown;
			break;
		case 0xFF629D:  //CH  0xFF629D
			button = Chanal;
			break;
		case 0xFFE21D:  //CH+   0xFFE21D
			button = ChanalUp;
			break;
		case 0xFF22DD:  //<<  0xFF22DD
			button = Left;
			break;
		case 0xFF02FD:  //>>  0xFF02FD
			button = Right;
			break;
		case 0xFFC23D:  //>||   0xFFC23D
			button = Play;
			break;
		case 0xFFE01F:  //–   0xFFE01F
			button = Minus;
			break;
		case 0xFFA857:  //+   0xFFA857
			button = Plus;
			break;
		case 0xFF906F:  //EQ  0xFF906F
			button = EQ;
			break;
		case 0xFF9867:  //100+  0xFF9867
			button = Hundred;
			break;
		case 0xFFB04F:  //200+  0xFFB04F
			button = TwoHundred;
			break;
		case 0xFF6897:  //0   0xFF6897
			button = Zero;
			break;
		case 0xFF30CF:  //1   0xFF30CF
			button = One;
			break;
		case 0xFF18E7:  //2   0xFF18E7
			button = Two;
			break;
		case 0xFF7A85:  //3   0xFF7A85
			button = Three;
			break;
		case 0xFF10EF:  //4   0xFF10EF
			button = Four;
			break;
		case 0xFF38C7:  //5   0xFF38C7
			button = Five;
			break;
		case 0xFF5AA5:  //6   0xFF5AA5
			button = Six;
			break;
		case 0xFF42BD:  //7   0xFF42BD
			button = Seven;
			break;
		case 0xFF4AB5:  //8   0xFF4AB5
			button = Eight;
			break;
		case 0xFF52AD:  //9   0xFF52AD
			button = Nine;
			break;
	}
}

void set_LED()
{
	digitalWrite( LED_1, (LED_data & 0b00000001) == 0 ? LOW : HIGH );
	digitalWrite( LED_2, (LED_data & 0b00000010) == 0 ? LOW : HIGH );
	digitalWrite( LED_3, (LED_data & 0b00000100) == 0 ? LOW : HIGH );
	digitalWrite( LED_4, (LED_data & 0b00001000) == 0 ? LOW : HIGH );
	digitalWrite( LED_5, (LED_data & 0b00010000) == 0 ? LOW : HIGH );
	digitalWrite( LED_6, (LED_data & 0b00100000) == 0 ? LOW : HIGH );
	digitalWrite( LED_7, (LED_data & 0b01000000) == 0 ? LOW : HIGH );
	digitalWrite( LED_8, (LED_data & 0b10000000) == 0 ? LOW : HIGH );
}