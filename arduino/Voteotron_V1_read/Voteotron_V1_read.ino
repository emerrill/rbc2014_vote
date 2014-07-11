#include <EEPROM.h>

#define ADDRESS_OFFSET   0x00
#define MAX_SLOT         7
#define BAUD             57600

void setup() {
  Serial.begin(BAUD);
  delay(1000);
  
  Serial.print("Scores:");
  Serial.println();
  
  Serial.print("1.21: ");
  Serial.print(readEEPROMWord(0), DEC);
  Serial.println();
  
  Serial.print("Omnicorp: ");
  Serial.print(readEEPROMWord(1), DEC);
  Serial.println();
  
  Serial.print("MB Labs: ");
  Serial.print(readEEPROMWord(2), DEC);
  Serial.println();
  
  Serial.print("Detroitus: ");
  Serial.print(readEEPROMWord(3), DEC);
  Serial.println();
  
  Serial.print("MakerTwins: ");
  Serial.print(readEEPROMWord(4), DEC);
  Serial.println();
  
  Serial.print("i3: ");
  Serial.print(readEEPROMWord(5), DEC);
  Serial.println();
  
  Serial.print("Tactical: ");
  Serial.print(readEEPROMWord(6), DEC);
  Serial.println();

}

void loop() {
  delay(100);
}

word readEEPROMWord(byte slot) {
  word addr = ADDRESS_OFFSET + (slot * 2);
  
  return word(EEPROM.read(addr + 1), EEPROM.read(addr));
}
