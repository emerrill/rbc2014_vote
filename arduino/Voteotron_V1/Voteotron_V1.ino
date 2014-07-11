#include <EEPROM.h>


#include <Adafruit_NeoPixel.h>
#define NUM_PIX          7



#define ADDRESS_OFFSET   0x00
#define POT_PIN          14
#define VOTE_PIN         3
#define STRAND_PIN       13
#define BAUD             57600
#define VOTE_LOCKOUT_MS  2000




#define MAX_SLOT         6
byte slot_leds[] = {27,25,25,25,25,25,28};
byte slot_offset[MAX_SLOT + 1];
#define LED_COUNT        183
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, STRAND_PIN, NEO_GRB + NEO_KHZ800);

uint32_t slot_color[MAX_SLOT+1] = {strip.Color(255, 0,0),
                                   strip.Color(255, 255,0),
                                   strip.Color(255, 0, 255),
                                   strip.Color(255, 255, 255),
                                   strip.Color(0, 255,0),
                                   strip.Color(0, 255, 255),
                                   strip.Color(0, 0,255)};


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

word dance_step = 0;

unsigned long vote_time = 0;


void setup() {
  Serial.begin(BAUD);
  
  word sum = 0;
  for (int i = 0; i <= MAX_SLOT; i++) {
    slot_offset[i] = sum;
    sum += slot_leds[i];
  }
  
  digitalWrite(VOTE_PIN, HIGH);
  
  // Setup pin two to be the ground sync for the vote pin
  pinMode(2, OUTPUT);
  digitalWrite(2, LOW);
  
  strip.begin();
  strip.show();

  allSlotsOn();
  strip.show();
  delay(500);
  allOff();
  strip.show();
  delay(500);

  sendScores();
  
  delay(10);
  attachInterrupt(1, voteISR, FALLING);
  
}

void loop() {
  updatePotSlot();
  updateLED();
  vote();
  
  if (current_slot == 254) {
    ants(strip.Color(255, 255, 255));
    dance_step++;
  }
  
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
  strip.show();
}

void allOff() {
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
}

void allOn(uint32_t color) {
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, color);
  }
}

void allSlotsOn() {
  for (int i = 0; i <= MAX_SLOT; i++) {
    slotOn(i);
  }
}

void slotOn(byte slot) {
  for (int i = 0; i < slot_leds[slot]; i++) {
    strip.setPixelColor(slot_offset[slot] + i, slot_color[i]);
  }
}

// ********************************
// Dance
// ********************************
void ants(uint32_t color) {
  unsigned int ds = dance_step / 3;
  for (int i = 0; i < LED_COUNT; i++) {
    if ((ds + i) % 1 == 0) {
      strip.setPixelColor(i, color);
    } else {
      strip.setPixelColor(i, 0, 0, 0);
    }
  }
  
  strip.show();
}

// ********************************
// Voting
// ********************************
void vote() {
  if (vote_slot > MAX_SLOT) {
    return;
  }
  
  allOn(slot_color[vote_slot]);
  strip.show();
  Serial.println(vote_slot);
  incrementSlot(vote_slot);
  delay(500);
  allOff();
  slotOn(vote_slot);
  strip.show();
  delay(500);
  allOn(slot_color[vote_slot]);
  strip.show();
  delay(500);
  allOff();
  slotOn(vote_slot);
  strip.show();
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

void sendScores() {
  Serial.print(F("Scores:"));
  
  Serial.print(F("1.21: "));
  Serial.print(readEEPROMWord(0), DEC);
  Serial.print(F(", "));
  
  Serial.print(F("Omnicorp: "));
  Serial.print(readEEPROMWord(1), DEC);
  Serial.print(", ");
  
  Serial.print(F("MB Labs: "));
  Serial.print(readEEPROMWord(2), DEC);
  Serial.print(F(", "));
  
  Serial.print(F("Detroitus: "));
  Serial.print(readEEPROMWord(3), DEC);
  Serial.print(F(", "));
  
  Serial.print(F("MakerTwins: "));
  Serial.print(readEEPROMWord(4), DEC);
  Serial.print(F(", "));
  
  Serial.print(F("i3: "));
  Serial.print(readEEPROMWord(5), DEC);
  Serial.print(F(", "));
  
  Serial.print(F("Tactical: "));
  Serial.print(readEEPROMWord(6), DEC);
  
  Serial.println();
}
