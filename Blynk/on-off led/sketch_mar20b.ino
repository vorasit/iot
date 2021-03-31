void setup() {
  // put your setup code here, to run once:
  pinMode(D2, INPUT_PULLUP);
  pinMode(D4, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  int buttonStates = digitalRead(D2);
  if(buttonStates == 0){
    digitalWrite(D4,HIGH);
    }
    else{
      digitalWrite(D4,LOW);
      }
    delay(100
    
    );
}
