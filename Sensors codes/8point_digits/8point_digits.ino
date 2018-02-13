/*
    ___
   | A |            D1 A  B  D2 D3 C
  B|   |C           *  *  *  *  *  *
   |___|
   | H |            *  *  *  *  *  *
  D|   |G           D  E  F  G  H  D4
   |___| o               (kropki na wyświetlaczu znajdują się koło dolnego ciągu)
     E    F

     Wysoka wartosc na pinie odpowiedzialnym za laseczkę daje kolor
     Wysoka wartość na pinie odpowiedzialnym za cyfrę da jej całkwowite wymaanie
*/


int pinA = 9;
int pinB = 10;
int pinC = 13;
int pinD = 2;
int pinE = 3;
int pinF = 4;
int pinG = 5;
int pinH = 6;
int D1 = 8;
int D2 = 11;
int D3 = 12;
int D4 = 7;

void number(int number)
{
  switch(number)
  {
    case 1:
      digitalWrite(pinA, LOW);   
      digitalWrite(pinB, LOW);   
      digitalWrite(pinC, HIGH);   
      digitalWrite(pinD, LOW);   
      digitalWrite(pinE, LOW);   
      digitalWrite(pinG, HIGH);   
      digitalWrite(pinH, LOW);
      break;
    case 2:
      digitalWrite(pinA, HIGH);   
      digitalWrite(pinB, LOW);   
      digitalWrite(pinC, HIGH);   
      digitalWrite(pinD, HIGH);   
      digitalWrite(pinE, HIGH);   
      digitalWrite(pinG, LOW);   
      digitalWrite(pinH, HIGH);
      break;
    case 3:
      digitalWrite(pinA, HIGH);   
      digitalWrite(pinB, LOW);   
      digitalWrite(pinC, HIGH);   
      digitalWrite(pinD, LOW);   
      digitalWrite(pinE, HIGH);   
      digitalWrite(pinG, HIGH);   
      digitalWrite(pinH, HIGH);
      break;
    case 4:
      digitalWrite(pinA, LOW);   
      digitalWrite(pinB, HIGH);   
      digitalWrite(pinC, HIGH);   
      digitalWrite(pinD, LOW);   
      digitalWrite(pinE, LOW);   
      digitalWrite(pinG, HIGH);   
      digitalWrite(pinH, HIGH);
      break;
    case 5:
      digitalWrite(pinA, HIGH);   
      digitalWrite(pinB, HIGH);   
      digitalWrite(pinC, LOW);   
      digitalWrite(pinD, LOW);   
      digitalWrite(pinE, HIGH);   
      digitalWrite(pinG, HIGH);   
      digitalWrite(pinH, HIGH);
      break;
    case 6:
      digitalWrite(pinA, HIGH);   
      digitalWrite(pinB, HIGH);   
      digitalWrite(pinC, LOW);   
      digitalWrite(pinD, HIGH);   
      digitalWrite(pinE, HIGH);   
      digitalWrite(pinG, HIGH);   
      digitalWrite(pinH, HIGH);
      break;
    case 7:
      digitalWrite(pinA, HIGH);   
      digitalWrite(pinB, LOW);   
      digitalWrite(pinC, HIGH);   
      digitalWrite(pinD, LOW);   
      digitalWrite(pinE, LOW);   
      digitalWrite(pinG, HIGH);   
      digitalWrite(pinH, LOW);
      break;
    case 8:
      digitalWrite(pinA, HIGH);   
      digitalWrite(pinB, HIGH);   
      digitalWrite(pinC, HIGH);   
      digitalWrite(pinD, HIGH);   
      digitalWrite(pinE, HIGH);   
      digitalWrite(pinG, HIGH);   
      digitalWrite(pinH, HIGH);
      break;
    case 9:
      digitalWrite(pinA, HIGH);   
      digitalWrite(pinB, HIGH);   
      digitalWrite(pinC, HIGH);   
      digitalWrite(pinD, LOW);   
      digitalWrite(pinE, HIGH);   
      digitalWrite(pinG, HIGH);   
      digitalWrite(pinH, HIGH);
      break;
    case 0:
      digitalWrite(pinA, HIGH);   
      digitalWrite(pinB, HIGH);   
      digitalWrite(pinC, HIGH);   
      digitalWrite(pinD, HIGH);   
      digitalWrite(pinE, HIGH);   
      digitalWrite(pinG, HIGH);   
      digitalWrite(pinH, LOW);
  }
}

void setup() {               
  // initialize the digital pins as outputs.
  pinMode(pinA, OUTPUT);     
  pinMode(pinB, OUTPUT);     
  pinMode(pinC, OUTPUT);     
  pinMode(pinD, OUTPUT);     
  pinMode(pinE, OUTPUT);     
  pinMode(pinF, OUTPUT);     
  pinMode(pinG, OUTPUT);   
  pinMode(D1, OUTPUT);  
  pinMode(D2, OUTPUT);  
  pinMode(D3, OUTPUT);  
  pinMode(D4, OUTPUT);
  pinMode(11, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  digitalWrite(D1, LOW);
  digitalWrite(D2, LOW);
  digitalWrite(D3, LOW);
  digitalWrite(D4, LOW); 
  
  for( int i = 0; i < 10; i++ )
  {
    Serial.println(i);
    number(i);
    delay(1000);
  }
}
