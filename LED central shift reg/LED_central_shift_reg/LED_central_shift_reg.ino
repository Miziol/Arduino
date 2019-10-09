#include <IRremote.h>

//IR config
#define IR_SIG_pin 2
IRrecv IR(IR_SIG_pin);
decode_results results;

//serial register connection
#define SR_MR_pin 7
#define SR_SHCP_pin 6
#define SR_STCP_pin 5
#define SR_OE_pin 4
#define SR_DS_pin 3


//data
byte SR_data = 0xFF;
unsigned long IR_data;

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
  Serial.println(SR_data);
  
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
  IR_change_value();

}


void IR_change_value()
{
  switch(IR_data)
  {
    case 0xFFA25D:  //CH-   0xFFA25D
    case 0xFF629D:  //CH  0xFF629D
    case 0xFFE21D:  //CH+   0xFFE21D
    case 0xFF22DD:  //<<  0xFF22DD
    case 0xFF02FD:  //>>  0xFF02FD
    case 0xFFC23D:  //>||   0xFFC23D
    case 0xFFE01F:  //–   0xFFE01F
    case 0xFFA857:  //+   0xFFA857
    case 0xFF906F:  //EQ  0xFF906F
    case 0xFF9867:  //100+  0xFF9867
      SR_data = 0x70;
    case 0xFFB04F:  //200+  0xFFB04F
      SR_data = 0x00;
    case 0xFF6897:  //0   0xFF6897
      SR_data = 0xFF;
    case 0xFF30CF:  //1   0xFF30CF
      SR_data = (SR_data & 0b11111110) + ((SR_data + 1) & 0x01);
    case 0xFF18E7:  //2   0xFF18E7
      SR_data = (SR_data & 0b11111101) + ((SR_data + 2) & 0x02);
    case 0xFF7A85:  //3   0xFF7A85
    case 0xFF10EF:  //4   0xFF10EF
    case 0xFF38C7:  //5   0xFF38C7
    case 0xFF5AA5:  //6   0xFF5AA5
    case 0xFF42BD:  //7   0xFF42BD
    case 0xFF4AB5:  //8   0xFF4AB5
    case 0xFF52AD:  //9   0xFF52AD
      break;
  }
}
