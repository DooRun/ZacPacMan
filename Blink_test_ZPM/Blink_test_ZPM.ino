/*
  Blink
*/


// the setup function runs once when you press reset or power the board
void setup() {
  
  pinMode(LED_B, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  digitalWrite(LED_G, HIGH);
  delay(1000);
  digitalWrite(LED_G, LOW);
  delay(1000);

  // initialize digital pin LED_BUILTIN as an output.
  //pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_B, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(2000);                      // wait for a second
  Serial.println("16:0");
  digitalWrite(LED_B, LOW);   // turn the LED off by making the voltage LOW
  delay(2000);                      // wait for a second
  Serial.println("16:1");
}
