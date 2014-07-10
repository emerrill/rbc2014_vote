#include <EEPROM.h>

#define ADDRESS_OFFSET   0x00
#define MAX_SLOT         7
#define BAUD             57600

void setup() {
  Serial.begin(BAUD);
  delay(1000);
  
  for (byte i = 0; i <= MAX_SLOT; i++) {
    Serial.print(i, DEC);
    Serial.print(": ");
    Serial.println(readEEPROMWord(i), DEC);
  }

}

void loop() {
  delay(100);
}

word readEEPROMWord(byte slot) {
  word addr = ADDRESS_OFFSET + (slot * 2);
  
  return word(EEPROM.read(addr + 1), EEPROM.read(addr));
}
