void setup() {
  pinMode(16, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(16, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
  digitalWrite(16, LOW);    // turn the LED off by making the voltage LOW
  delay(100);                       // wait for a second
}
