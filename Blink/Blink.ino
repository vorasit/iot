/*
  ESP8266 Blink by Simon Peter
  Blink the blue LED on the ESP-01 module
  This example code is in the public domain

  The blue LED on the ESP-01 module is connected to GPIO1
  (which is also the TXD pin; so we cannot use Serial.print() at the same time)

  Note that this sketch uses LED_BUILTIN to find the pin with the internal LED
*/

void setup() {
  pinMode(4, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  pinMode(5, OUTPUT); 
  pinMode(D0, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(4, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(1000);     
  digitalWrite(4, LOW);   // Turn the LED on (Note that LOW is the voltage level
  delay(1000);                      // Wait for a second
                  // Wait for a second
  digitalWrite(5, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(1000);
  digitalWrite(5, LOW);
  delay(1000);

  digitalWrite(D0, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(1000);
  digitalWrite(D0, LOW);
  delay(1000);
  
}
