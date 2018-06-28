int fr;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  fr = 34;
}

void loop() {
  // put your main code here, to run repeatedly:
  analogRead(A0);
  tone(7, fr);
  Serial.print(analogRead(A0));
  Serial.print("\n");
  fr++;
  
}
