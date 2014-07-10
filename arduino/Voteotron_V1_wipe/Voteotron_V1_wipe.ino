#include <EEPROM.h>

#define ADDRESS_OFFSET   0x00
#define MAX_SLOT         7

void setup() {
  delay(1000);
  
  for (byte i = 0; i <= MAX_SLOT; i++) {
    writeEEPROMWord(i, 0);
  }

}

void loop() {
  delay(100);
}

void writeEEPROMWord(byte slot, word val) {
  word addr = ADDRESS_OFFSET + (slot * 2);
  
  EEPROM.write(addr, lowByte(val));
  EEPROM.write(addr + 1, highByte(val));
}
