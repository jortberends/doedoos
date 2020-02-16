#include <MD_MAX72xx.h>
#include <SPI.h>
#include <MFRC522.h>

//  Matrix
#define MATRIX_HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW
#define MATRIX_MAX_DEVICES 1
#define MATRIX_CS_PIN    36
MD_MAX72XX matrix = MD_MAX72XX(MATRIX_HARDWARE_TYPE, MATRIX_CS_PIN, MATRIX_MAX_DEVICES);

MFRC522 mfrc522(53, -1);  // Create MFRC522 instance

#define VIBRATOR 11
#define EMERGENCY 12

int ledPins[] = {8,23,25,27,29,31};
int pots[] = {54,55,56,57,58};

uint8_t letters[9][8][8] = {
  { // E lin
    {0,1,1,1,1,1,1,0},
    {0,1,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0},
    {0,1,1,1,1,1,0,0},
    {0,1,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,0}
  }, { // O ma
    {0,0,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,0,1,1,1,1,0,0}
  }, { // J
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,1,0},
    {0,0,0,0,0,1,0,0},
    {0,1,1,1,1,0,0,0}
  }, { // M ama
    {1,0,0,0,0,0,0,1},
    {1,1,0,0,0,0,1,1},
    {1,0,1,0,0,1,0,1},
    {1,0,0,1,1,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1}
  }, { // R imme
    {0,1,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,1,1,1,1,0,0},
    {0,1,1,0,0,0,0,0},
    {0,1,0,1,0,0,0,0},
    {0,1,0,0,1,0,0,0},
    {0,1,0,0,0,1,0,0}
  }, { // K im
    {0,1,0,0,0,1,0,0},
    {0,1,0,0,1,0,0,0},
    {0,1,0,1,0,0,0,0},
    {0,1,1,0,0,0,0,0},
    {0,1,1,0,0,0,0,0},
    {0,1,0,1,0,0,0,0},
    {0,1,0,0,1,0,0,0},
    {0,1,0,0,0,1,0,0}
  }, { // B ram
    {0,1,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,1,1,1,1,0,0}
  }, { // S anne
    {0,0,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,0,1,1,1,1,0,0}
  }, { // O pa
    {0,0,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,0,1,1,1,1,0,0}
  }
};

uint8_t icon[7][8][8] = {
  { // Happy
    {0,0,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {1,0,1,0,0,1,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,1,0,0,1,0,1},
    {1,0,0,1,1,0,0,1},
    {0,1,0,0,0,0,1,0},
    {0,0,1,1,1,1,0,0}
  },
//  { // Sad
//    {0,0,1,1,1,1,0,0},
//    {0,1,0,0,0,0,1,0},
//    {1,0,1,0,0,1,0,1},
//    {1,0,0,0,0,0,0,1},
//    {1,0,0,1,1,0,0,1},
//    {1,0,1,0,0,1,0,1},
//    {0,1,0,0,0,0,1,0},
//    {0,0,1,1,1,1,0,0}
//  },
  { // Kat
    {1,0,0,0,1,0,0,0},
    {1,1,1,1,1,0,0,0},
    {1,0,1,0,1,0,0,0},
    {0,1,1,1,0,0,0,1},
    {0,0,1,0,0,0,0,1},
    {0,1,1,1,1,0,0,1},
    {0,1,1,1,1,1,0,1},
    {1,0,1,1,1,1,1,0}
  },
  { // Hond
    {0,0,1,0,0,0,0,0},
    {1,1,1,0,0,0,0,0},
    {1,1,1,0,0,0,0,1},
    {0,0,1,1,1,1,1,0},
    {0,0,1,1,1,1,1,0},
    {0,0,1,1,1,1,1,0},
    {0,0,1,0,0,0,1,0},
    {0,1,1,0,0,1,1,0}
  },
  { // Vogel
    {0,1,1,1,0,0,0,0},
    {1,1,0,1,0,0,0,0},
    {0,1,1,1,0,0,0,0},
    {0,1,1,1,1,0,0,0},
    {0,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,0,0,0},
    {0,0,1,0,0,0,0,0}
  },
  { // Vis
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,1,0,0,1,0,1},
    {0,1,0,1,0,0,1,1},
    {1,0,0,0,0,0,0,1},
    {0,1,0,0,0,0,1,1},
    {0,0,1,0,0,1,0,1},
    {0,0,0,1,1,0,0,0}
  },
  { // Klok
    {0,0,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {1,0,0,1,0,0,0,1},
    {1,0,0,1,0,0,0,1},
    {1,0,0,1,1,1,0,1},
    {1,0,0,0,0,0,0,1},
    {0,1,0,0,0,0,1,0},
    {0,0,1,1,1,1,0,0}
  },
  { // Huis
    {0,0,0,1,1,0,0,0},
    {0,0,1,0,0,1,0,0},
    {0,1,0,0,0,0,1,0},
    {1,0,0,0,1,1,0,1},
    {1,0,0,0,1,1,0,1},
    {1,0,1,1,0,0,0,1},
    {1,0,1,1,0,0,0,1},
    {1,1,1,1,1,1,1,1}
  }
};

struct Button {
  uint8_t led;
  uint8_t input;
};

Button buttons[] = {
  { 2, 38},
  { 3, 39},
  { 4, 40},
  { 5, 41},
  { 6, 42},
  { 7, 43},
  {33, 47},
  {35, 48},
  {37, 49}
 };

uint8_t buttonToPress = 0;

void setup() {
  Serial.begin(115200);
  matrix.begin();
  mfrc522.PCD_Init();
  setupLED();
  setupButtons();
  newRandomButtonToPress();
  //setupTorn();
  setupEmergency();
  setupVibrator();
}

void loop() {
  readButtons();
  readGoudenPot();
  loopBlauwePot();
  loopNFC();
  loopVibrator();
  loopEmergency();
//  loopIcons();
  //loopTorn();
}

void setupEmergency() {
  pinMode(EMERGENCY, INPUT_PULLUP);
}

void setupVibrator() {
  pinMode(VIBRATOR, OUTPUT);
}

uint8_t vibratorSteps = 0;
bool vibratorOn = false;
bool vibratorWorking = false;
void loopVibrator() {
  vibratorSteps++;
  if (vibratorSteps == 255) {
    vibratorSteps = 0;
    vibratorOn = !vibratorOn;
  }
  if (!vibratorWorking) {
    return;
  }
  if (vibratorOn) {
    analogWrite(VIBRATOR, 128);
  } else {
    analogWrite(VIBRATOR, 0);
  }
}

void loopEmergency() {
  uint8_t val = digitalRead(EMERGENCY);
  if (val == HIGH) {
    Serial.println("Emergency HIGH");
    vibratorWorking=true;
  } else {
    Serial.println("Emergency LOW");
    vibratorWorking=false;
    analogWrite(VIBRATOR, 0);
  }
}

void testLED() {
  for (uint8_t pin = 0; pin < (sizeof(ledPins)/sizeof(ledPins[0])); pin++) {
    digitalWrite(ledPins[pin], HIGH);
    delay(50);
    digitalWrite(ledPins[pin], LOW);
  }
  for (uint8_t x = 0; x<8; x++) {
    Serial.println();
    for (uint8_t y = 0; y<8; y++) {
      matrix.setPoint(x, y, true);
      delay(50);
      matrix.clear();
    }
  }
}

void setupLED() {
  for (uint8_t i = 0; i < (sizeof(ledPins)/sizeof(ledPins[0])); i++) {
    Serial.print("Pinmode output for: ");
    Serial.println(i);
    pinMode(ledPins[i], OUTPUT);
  }
}

void setupButtons() {
  for (uint8_t i=0; i < (sizeof(buttons)/sizeof(buttons[0])); i++) {
    pinMode(buttons[i].input, INPUT_PULLUP);
    pinMode(buttons[i].led, OUTPUT);
  }
}

uint8_t buttonsSteps = 0;
uint8_t buttonsStepsValue = HIGH;
void readButtons() {
  for (uint8_t i=0; i < (sizeof(buttons)/sizeof(buttons[0])); i++) {
    int val = digitalRead(buttons[i].input);
    if (val == LOW && i == buttonToPress) {
      newRandomButtonToPress();
      nextIcon();
      buttonsSteps = 0;
      buttonsStepsValue = HIGH;
    } else if (i == buttonToPress && val == HIGH) {
      if (buttonsSteps > 128) {
        buttonsSteps = 0;
        buttonsStepsValue = !buttonsStepsValue;
      } else {
        buttonsSteps++;
      }
      digitalWrite(buttons[i].led, buttonsStepsValue);
    } else {
      digitalWrite(buttons[i].led, !val);
    }
  }
  Serial.println("=====================");
}

void readGoudenPot() {
  int val = analogRead(58);
  if (val > 102) {
    digitalWrite(31, HIGH);
  } else {
    digitalWrite(31, LOW);
  }
  if (val > 306) {
    digitalWrite(29, HIGH);
  } else {
    digitalWrite(29, LOW);
  }
  if (val > 510) {
    digitalWrite(27, HIGH);
  } else {
    digitalWrite(27, LOW);
  }
  if (val > 714) {
    digitalWrite(25, HIGH);
  } else {
    digitalWrite(25, LOW);
  }
  if (val > 916) {
    digitalWrite(23, HIGH);
  } else {
    digitalWrite(23, LOW);
  }
  Serial.print("Gold Pot:");
  Serial.println(val);
}


uint8_t iconSteps = 0;
void loopIcons() {
  if (iconSteps == 255) {
    nextIcon();
    iconSteps = 0;
  } else {
    iconSteps++;
  }
}

void printLetter(uint8_t index) {
  Serial.print("PrintLetter:");
  Serial.println(index);
  matrix.clear();
  for (uint8_t x=0; x < (sizeof(letters[0])/sizeof(letters[0][0])); x++) {
    for (uint8_t y = 0; y < (sizeof(letters[0][0])/sizeof(letters[0][0][0])); y++) {
      if (letters[index][x][abs(y-7)] == 1) {
        matrix.setPoint(x, y, true);
      }
    }
  }
}

uint8_t currentIcon = 0;
void nextIcon() {
  currentIcon++;
  if (currentIcon == (sizeof(icon)/sizeof(icon[0]))) {
    currentIcon = 0;
  }
  printIcon(currentIcon);
}

void printIcon(uint8_t index) {
  Serial.print("Print smiley: ");
  Serial.println(index);
    matrix.clear();
    for (uint8_t x=0; x < (sizeof(icon[0])/sizeof(icon[0][0])); x++) {
      for (uint8_t y = 0; y < (sizeof(icon[0][0])/sizeof(icon[0][0][0])); y++) {
        if (icon[index][x][y] == 1) {
          matrix.setPoint(x, y, true);
        }
      }
    }
}

void newRandomButtonToPress() {
  uint8_t newRandomButton = random(0, sizeof(buttons)/sizeof(buttons[0]));
  if (newRandomButton == buttonToPress) {
    newRandomButtonToPress();
  } else {
    buttonToPress = newRandomButton;
    Serial.print("New button to press: ");
    Serial.println(buttons[buttonToPress].input);
  }
}

void setupTorn() {
  pinMode(INPUT_PULLUP, 46);
}

int tornLastValue = HIGH;
void loopTorn() {
  int val = digitalRead(46);
  Serial.print("Torn val:");
  Serial.println(val);
  if (val == tornLastValue) {
    return;
  }
  if (val == HIGH) {
    printIcon(0);
  } else {
    printIcon(1);
  }
  tornLastValue = val;
}

uint16_t blauwePotSteps = 0;
uint16_t blauwePotState = HIGH;
void loopBlauwePot() {
  uint16_t val = analogRead(0);
  Serial.print("Blauwe pot: ");
  Serial.println(val);
  if (blauwePotSteps > (val/4)) {
    blauwePotState = !blauwePotState;
    blauwePotSteps = 0;
  } else {
    blauwePotSteps++;
  }
  digitalWrite(8, blauwePotState);
}

uint8_t nfc[10][7] = {
  {0x04,0x1D,0x35,0xC2,0x83,0x64,0x81},
  {0x04,0x25,0x35,0xC2,0x83,0x64,0x81},
  {0x04,0x29,0x34,0xC2,0x83,0x64,0x81},
  {0x04,0x2D,0x35,0xC2,0x83,0x64,0x81},
  {0x04,0x21,0x34,0xC2,0x83,0x64,0x81},
  {0x04,0xFA,0x31,0xC2,0x83,0x64,0x80},
  {0x04,0x13,0x32,0xC2,0x83,0x64,0x81},
  {0x04,0x0B,0x32,0xC2,0x83,0x64,0x81},
  {0x04,0x04,0x31,0xC2,0x83,0x64,0x81},
  
  };
//uint8_t nfc_blauwedruppel[] = {0xCA, 0x26, 0x64, 0x25};
//uint8_t nfc_card[] = {0xF2,0xF6,0x7E,0x2C};
uint8_t lastCard[10] = {};
uint8_t loopNFCSteps = 0;
void loopNFC() {
  loopNFCSteps++;
  if (loopNFCSteps < 25) {
    return;
  }
  loopNFCSteps = 0;
  if (!mfrc522.PICC_IsNewCardPresent()) {
    Serial.println("No card present");
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    Serial.println("Read error");
    return;
  }
  Serial.println("Card read.");
  if (memcmp(mfrc522.uid.uidByte, lastCard, mfrc522.uid.size) == 0) {
    // Same NFC as previous
    Serial.println("Same as previous");
    return;
  }
  strcpy(lastCard, mfrc522.uid.uidByte);
  for (uint8_t i=0; i < (sizeof(nfc)/sizeof(nfc[0])); i++) {
    if (memcmp(lastCard, nfc[i], mfrc522.uid.size) == 0) {
      uint8_t fileIndex = i; // 0-based
      Serial.print("Card: ");
      Serial.println(fileIndex);
      printLetter(fileIndex);
      return;
    }
  }
  Serial.println("Unknown NFC: ");
}

/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
