
#define POT_PIN          15
#define BAUD             57600

void setup() {
  Serial.begin(BAUD);
}

void loop() {
  int rawval = analogRead(POT_PIN);
  Serial.println(rawval);
  
  delay(250);
}
