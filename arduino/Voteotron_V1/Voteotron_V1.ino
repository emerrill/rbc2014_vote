#include <EEPROM.h>


#include <Adafruit_NeoPixel.h>
#define NUM_PIX          7



#define ADDRESS_OFFSET   0x00
#define POT_PIN          14
#define VOTE_PIN         3
#define STRAND_PIN       6
#define BAUD             57600
#define VOTE_LOCKOUT_MS  2000

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIX, STRAND_PIN, NEO_GRB + NEO_KHZ800);


#define MAX_SLOT         6
byte slot_led_pins[] = {4, 5, 6, 7, 8, 9, 10, 11};


#define SLOT_0_LOW       200
#define SLOT_0_HIGH      300
#define SLOT_1_LOW       300
#define SLOT_1_HIGH      400
#define SLOT_2_LOW       400
#define SLOT_2_HIGH      500
#define SLOT_3_LOW       500
#define SLOT_3_HIGH      600
#define SLOT_4_LOW       600
#define SLOT_4_HIGH      700
#define SLOT_5_LOW       700
#define SLOT_5_HIGH      800
#define SLOT_6_LOW       800
#define SLOT_6_HIGH      900

byte current_slot = 254;
byte vote_slot = 254;

unsigned long vote_time = 0;


void setup() {
  Serial.begin(BAUD);

  digitalWrite(VOTE_PIN, HIGH);
  delay(10);
  attachInterrupt(1, voteISR, FALLING);
  
}

void loop() {
  updatePotSlot();
  vote();
  
  delay(100);
}

// ********************************
// Slot Selection
// ********************************
byte updatePotSlot() {
  int rawval = analogRead(POT_PIN);

  if ((SLOT_0_LOW < rawval) && (rawval < SLOT_0_HIGH)) {
    current_slot = 0;
  } else if ((SLOT_1_LOW < rawval) && (rawval < SLOT_1_HIGH)) {
    current_slot = 1;
  } else if ((SLOT_2_LOW < rawval) && (rawval < SLOT_2_HIGH)) {
    current_slot = 2;
  } else if ((SLOT_3_LOW < rawval) && (rawval < SLOT_3_HIGH)) {
    current_slot = 3;
  } else if ((SLOT_4_LOW < rawval) && (rawval < SLOT_4_HIGH)) {
    current_slot = 4;
  } else if ((SLOT_5_LOW < rawval) && (rawval < SLOT_5_HIGH)) {
    current_slot = 5;
  } else if ((SLOT_6_LOW < rawval) && (rawval < SLOT_6_HIGH)) {
    current_slot = 6;
  } else {
    current_slot = 254;
  }
}

// ********************************
// LED Updates
// ********************************
void updateLED() {
  allOff();
  slotOn(current_slot);
}

void allOff () {
  /*for (int i = 0; i <= MAX_SLOT; i++) {
    digitalWrite(getSlotPin(i), LOW);
  }*/
}

void slotOn(byte slot) {
  /*digitalWrite(getSlotPin(slot), LOW);*/
}

// ********************************
// Voting
// ********************************
void vote() {
  if (vote_slot > MAX_SLOT) {
    return;
  }
  
  allOff();
  Serial.println(vote_slot);
  incrementSlot(vote_slot);
  delay(500);
  slotOn(vote_slot);
  delay(500);
  allOff();
  delay(500);
  slotOn(vote_slot);
  allOff();
  delay(500);
  
  vote_slot = 254;
  updatePotSlot();
  updateLED();
}

void voteISR() {
  if (vote_time > 0 && (millis() - vote_time) < VOTE_LOCKOUT_MS) {
    return;
  }
  if (current_slot > MAX_SLOT) {
    return;
  }
  
  vote_slot = current_slot;
  vote_time = millis();
}


// ********************************
// EEPROM
// ********************************

void writeEEPROMWord(byte slot, word val) {
  word addr = ADDRESS_OFFSET + (slot * 2);
  
  EEPROM.write(addr, lowByte(val));
  EEPROM.write(addr + 1, highByte(val));
}

word readEEPROMWord(byte slot) {
  word addr = ADDRESS_OFFSET + (slot * 2);
  
  return word(EEPROM.read(addr + 1), EEPROM.read(addr));
}

void incrementSlot(byte slot) {
  writeEEPROMWord(slot, (readEEPROMWord(slot) + 1));
}
