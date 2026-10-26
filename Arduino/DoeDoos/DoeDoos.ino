#include <MD_MAX72xx.h>
#include <SPI.h>
#include <MFRC522.h>
#include <stdio.h>
#include <string.h>

#define DEBUG_ENABLED 0
#if DEBUG_ENABLED
  #define DEBUG_PRINT(...) Serial.print(__VA_ARGS__)
  #define DEBUG_PRINTLN(...) Serial.println(__VA_ARGS__)
#else
  #define DEBUG_PRINT(...)
  #define DEBUG_PRINTLN(...)
#endif

//  Matrix
#define MATRIX_HARDWARE_TYPE MD_MAX72XX::ICSTATION_HW
#define MATRIX_MAX_DEVICES 1
#define MATRIX_CS_PIN    36
MD_MAX72XX matrix = MD_MAX72XX(MATRIX_HARDWARE_TYPE, MATRIX_CS_PIN, MATRIX_MAX_DEVICES);

MFRC522 mfrc522(53, -1);  // Create MFRC522 instance

#define VIBRATOR 11
#define EMERGENCY 12

int ledPins[] = {8,23,25,27,29,31};
// int pots[] = {54,55,56,57,58};
#define GOLDEN_POT 58

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
  { 5, 41}, // Blue
  { 6, 42},
  { 7, 43},
  {33, 47},
  {35, 48},
  {37, 49}
 };

enum Modus {
  standaard,
  sketch,
  catchDrop,
  catchLine
};

Modus modus = standaard;

uint8_t buttonToPress = 0;

void setup() {
  Serial.begin(115200);
  matrix.begin();
  mfrc522.PCD_Init();
  setupLED();
  setupButtons();
  newRandomButtonToPress();
  setupTorn();
  setupEmergency();
  setupVibrator();
  setupFlipButtons();
}

void loop() {
  readButtons();
  readGoudenPot();
  loopBlauwePot();
  loopNFC();
  loopEmergency();
//  loopIcons();
  loopTorn();
  loopFlipButtons();
  loopOrangeRedPot();
  loopCatchDrop();
  loopCatchLine();
}

#define FLIP_LEFT 44
#define FLIP_RIGHT 45

void setupFlipButtons() {
  pinMode(FLIP_LEFT, INPUT_PULLUP);
  pinMode(FLIP_RIGHT, INPUT_PULLUP);
}

int flipLeftLastValue = LOW;
int flipRightLastValue = LOW;
void loopFlipButtons() {
  int left = digitalRead(FLIP_LEFT);
  int right = digitalRead(FLIP_RIGHT);
  if (left == flipLeftLastValue && right == flipRightLastValue) {
    return;
  }
  flipRightLastValue = right;
  flipLeftLastValue = left;

  matrix.clear();
  clearButtons();
  clearLeds();

  if (right == HIGH) {
    if (left == HIGH) {
      DEBUG_PRINTLN("Modus to catch the line");
      modus = catchLine;
      digitalWrite(buttons[3].led, HIGH);
    } else {
      DEBUG_PRINTLN("Modus to catch the drop");
      modus = catchDrop;
    }
  } else {
    if (left == HIGH) {
      DEBUG_PRINTLN("Modus to sketch");
      modus = sketch;
    } else {
      DEBUG_PRINTLN("Modus to default");
      modus = standaard;
    }
  }
}

void setupEmergency() {
  pinMode(EMERGENCY, INPUT_PULLUP);
}

void setupVibrator() {
  pinMode(VIBRATOR, OUTPUT);
}

//uint8_t vibratorSteps = 0;
//bool vibratorOn = false;
//bool vibratorWorking = false;
//void loopVibrator() {
//  vibratorSteps++;
//  if (vibratorSteps == 255) {
//    vibratorSteps = 0;
//    vibratorOn = !vibratorOn;
//  } else {
//    return;
//  }
//  if (!vibratorWorking) {
//    return;
//  }
//  if (vibratorOn) {
//    analogWrite(VIBRATOR, 255);
//  } else {
//    analogWrite(VIBRATOR, 0);
//  }
//}
bool vibratorWorking=false;
void loopEmergency() {
  if (modus == catchDrop || modus == catchLine) {
    return;
  }
  uint8_t val = digitalRead(EMERGENCY);
  if (val == HIGH) {
    DEBUG_PRINTLN("Emergency HIGH");
    vibratorWorking=true;
  } else {
    DEBUG_PRINTLN("Emergency LOW");
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
    DEBUG_PRINTLN();
    for (uint8_t y = 0; y<8; y++) {
      matrix.setPoint(x, y, true);
      delay(50);
      matrix.clear();
    }
  }
}

void setupLED() {
  for (uint8_t i = 0; i < (sizeof(ledPins)/sizeof(ledPins[0])); i++) {
    DEBUG_PRINT("Pinmode output for: ");
    DEBUG_PRINTLN(i);
    pinMode(ledPins[i], OUTPUT);
  }
}

void setupButtons() {
  for (uint8_t i=0; i < (sizeof(buttons)/sizeof(buttons[0])); i++) {
    pinMode(buttons[i].input, INPUT_PULLUP);
    pinMode(buttons[i].led, OUTPUT);
  }
}

void clearButtons() {
  for (uint8_t i=0; i < (sizeof(buttons)/sizeof(buttons[0])); i++) {
    digitalWrite(buttons[i].led, LOW);
  }
}

void clearLeds() {
  for (uint8_t i=0; i < (sizeof(ledPins)/sizeof(ledPins[0])); i++) {
    digitalWrite(ledPins[i], LOW);
  }
}

uint8_t buttonsSteps = 0;
uint8_t buttonsStepsValue = HIGH;
void readButtons() {
  if (modus == catchDrop || modus == catchLine) {
    return;
  }
  if (modus != standaard) {
    clearButtons();
    return;
  }
  bool aButtonIsBeingPressed = false;
  for (uint8_t i=0; i < (sizeof(buttons)/sizeof(buttons[0])); i++) {
    int val = digitalRead(buttons[i].input);
    if (val == LOW && i != buttonToPress) {
      aButtonIsBeingPressed = true;
    }
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
  
  DEBUG_PRINT("Vibrator ON - a button is ");
  if (aButtonIsBeingPressed) {
    DEBUG_PRINTLN("pressed");
    analogWrite(VIBRATOR, 255);
  } else {
    DEBUG_PRINTLN("NOT pressed");
    analogWrite(VIBRATOR, 0);
  }
  DEBUG_PRINTLN("=====================");
}

void readGoudenPot() {
  if (modus == catchDrop || modus == catchLine) {
    return;
  }
  int val = analogRead(GOLDEN_POT);
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
  DEBUG_PRINT("Gold Pot:");
  DEBUG_PRINTLN(val);
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
  DEBUG_PRINT("PrintLetter:");
  DEBUG_PRINTLN(index);
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
  DEBUG_PRINT("Print smiley: ");
  DEBUG_PRINTLN(index);
  matrix.clear();
  for (uint8_t x=0; x < (sizeof(icon[0])/sizeof(icon[0][0])); x++) {
    for (uint8_t y = 0; y < (sizeof(icon[0][0])/sizeof(icon[0][0][0])); y++) {
      if (icon[index][x][y] == 1) {
        matrix.setPoint(x, y, true);
      }
    }
  }
  currentIcon = index;
}

bool isMirroring = false;
void mirrorIcon() {
  isMirroring = !isMirroring;
  DEBUG_PRINT("Mirror icon: ");
  DEBUG_PRINTLN(isMirroring);
  matrix.clear();
  for (uint8_t x=0; x < (sizeof(icon[0])/sizeof(icon[0][0])); x++) {
    for (uint8_t y = 0; y < (sizeof(icon[0][0])/sizeof(icon[0][0][0])); y++) {
      if (icon[currentIcon][x][y] == 1) {
        uint8_t actualY = y;
        if (isMirroring) {
          actualY = abs(actualY-7);
        }
        matrix.setPoint(x, actualY, true);
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
    DEBUG_PRINT("New button to press: ");
    DEBUG_PRINTLN(buttons[buttonToPress].input);
  }
}

void setupTorn() {
  pinMode(46, INPUT_PULLUP);
}

int tornLastValue = HIGH;
void loopTorn() {
  if (modus != standaard) {
    return;
  }
  int val = digitalRead(46);
  DEBUG_PRINT("Torn val:");
  DEBUG_PRINTLN(val);
  if (val == tornLastValue) {
    return;
  }
  mirrorIcon();
  tornLastValue = val;
}

uint16_t blauwePotSteps = 0;
uint16_t blauwePotState = HIGH;
void loopBlauwePot() {
  if (modus == catchDrop || modus == catchLine) {
    return;
  }
  uint16_t val = analogRead(0);
  DEBUG_PRINT("Blauwe pot: ");
  DEBUG_PRINTLN(val);
  if (blauwePotSteps > (val/4)) {
    blauwePotState = !blauwePotState;
    blauwePotSteps = 0;
  } else {
    blauwePotSteps++;
  }
  digitalWrite(8, blauwePotState);
}

uint8_t tekening[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
  };
  
void resetTekening() {
  for (uint8_t x=0;x<8;x++) {
    for (uint8_t y=0;y<8;y++) {
      tekening[x][y] = 0;  
    }
  }
  matrix.clear();
}

// 0..1024
uint8_t catchDropPoints=0;
uint8_t xPos = 0;
uint8_t lastXPos = 0;
void loopCatchDropControl() {
  uint16_t controlKnob = analogRead(GOLDEN_POT);
  xPos = controlKnob / 147;
  if (xPos == lastXPos) {
    return;
  }
  // matrix 0,0 (x,y) rechts boven
  // x = verticaal
  // y = horizontaal
  DEBUG_PRINT("CatchDropBar: ");
  DEBUG_PRINTLN(xPos);
  drawCatchDrop();
  lastXPos = xPos;
}

void loopCatchDrop() {
  if (modus != catchDrop) {
    return;
  }
  loopCatchDropControl();
  loopCatchDropDrop();
}

uint8_t drops[] = {8,8,8,8,8,8,8,8};
const unsigned long CATCH_DROP_BASE_INTERVAL_MS = 200;
const unsigned long CATCH_DROP_MIN_INTERVAL_MS = 1;
unsigned long catchDropIntervalMs = CATCH_DROP_BASE_INTERVAL_MS;
unsigned long lastCatchDropUpdate = 0;
bool catchDropNeedSpawn = true;
void loopCatchDropDrop() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastCatchDropUpdate < catchDropIntervalMs) {
    return;
  }
  lastCatchDropUpdate = currentMillis;

  for (uint8_t i=0;i<8;i++) {
    if (drops[i] < 8) {
      drops[i] = drops[i]+1;
      if (drops[i] == 7) {
        catchDropNeedSpawn=true;
        if (i == xPos || i == xPos+1) {
          catchDropPoints++;
          if (catchDropIntervalMs > CATCH_DROP_MIN_INTERVAL_MS) {
            catchDropIntervalMs = catchDropIntervalMs-5;
          }
        } else {
          // lives meganisme
          if (catchDropPoints > 0) {
            displayGameOver(catchDropPoints);
          }
          catchDropIntervalMs = CATCH_DROP_BASE_INTERVAL_MS;
          catchDropPoints=0;
          digitalWrite(5, LOW);
          digitalWrite(6, LOW);
          digitalWrite(7, LOW);
        }
      }
    }
  }
  if (catchDropNeedSpawn) {
    catchDropNeedSpawn=false;
    uint8_t newDropAt = random(0,7);
    drops[newDropAt]=0;
  }
  drawCatchDrop();
}

void drawCatchDrop() {
  matrix.clear();
  matrix.setPoint(7,xPos,true);
  matrix.setPoint(7,xPos+1,true);
  for (uint8_t i=0;i<8;i++) {
    if (drops[i] < 8) {
      matrix.setPoint(drops[i],i,true);
    }
  }
}

// START: Catch the line
uint8_t xPosCatchLine = 0;
bool catchLineMovingRight = true;
uint16_t catchLinePoints = 0;
int catchLineButtonState = HIGH;
int lastCatchLineButtonReading = HIGH;
unsigned long lastCatchLineButtonChange = 0;
const unsigned long CATCH_LINE_BUTTON_DEBOUNCE_MS = 30;
const unsigned long CATCH_LINE_INTERVAL_MS = 200;
unsigned long lastCatchLineUpdate = 0;

void resetCatchLine() {
  xPosCatchLine = 0;
  catchLineMovingRight = true;
  catchLinePoints = 0;
  catchLineButtonState = HIGH;
  lastCatchLineButtonReading = HIGH;
  lastCatchLineButtonChange = 0;
  lastCatchLineUpdate = 0;
}

void loopCatchLine() {
  if (modus != catchLine) {
    return;
  }

  // Debounce the active-low button and count only the stable press edge.
  int buttonReading = digitalRead(buttons[3].input);
  if (buttonReading != lastCatchLineButtonReading) {
    lastCatchLineButtonChange = millis();
    lastCatchLineButtonReading = buttonReading;
  }
  bool catchLineButtonPressed = false;
  if (millis() - lastCatchLineButtonChange >= CATCH_LINE_BUTTON_DEBOUNCE_MS &&
      buttonReading != catchLineButtonState) {
    catchLineButtonState = buttonReading;
    catchLineButtonPressed = catchLineButtonState == LOW;
  }
  if (catchLineButtonPressed) {
    if (xPosCatchLine == 4) {
      DEBUG_PRINTLN("Caught the line!");
      catchLinePoints++;
      DEBUG_PRINT("Catch the line points: ");
      DEBUG_PRINTLN(catchLinePoints);
      displayScore(catchLinePoints);
    } else {
      DEBUG_PRINTLN("Missed the line!");
      displayGameOver(catchLinePoints);
      resetCatchLine();
      return;
    }
  }

  // Update the catch line position
  unsigned long currentMillis = millis();
  if (currentMillis - lastCatchLineUpdate < CATCH_LINE_INTERVAL_MS) {
    return;
  }
  lastCatchLineUpdate = currentMillis;

  matrix.clear();
  if (xPosCatchLine == 8) {
    catchLineMovingRight = false;
  } else if (xPosCatchLine == 0) {
    catchLineMovingRight = true;
  }
  if (catchLineMovingRight) {
    xPosCatchLine++;
  } else {
    xPosCatchLine--;
  }

  // Draw the new position of the catch line, but also keep the previous position lit for a trailing effect.
  if (xPosCatchLine < 8) {
    matrix.setColumn(xPosCatchLine, 0xFF); // Draw the new position
  }
  if (xPosCatchLine > 0) {
    matrix.setColumn(xPosCatchLine - 1, 0xFF);
  }

  // Draw the "catch line" at the bottom of the matrix
  matrix.setPoint(7, 3, true);
  matrix.setPoint(7, 4, true);
}

// END: Catch the line

uint8_t gameOverGlyphColumn(char character, uint8_t column) {
  static const uint8_t glyphG[5] = {0x7E, 0x41, 0x49, 0x49, 0x3A};
  static const uint8_t glyphA[5] = {0x7E, 0x11, 0x11, 0x11, 0x7E};
  static const uint8_t glyphM[5] = {0x7F, 0x06, 0x18, 0x06, 0x7F};
  static const uint8_t glyphE[5] = {0x7F, 0x49, 0x49, 0x49, 0x41};
  static const uint8_t glyphO[5] = {0x3E, 0x41, 0x41, 0x41, 0x3E};
  static const uint8_t glyphV[5] = {0x0F, 0x30, 0x40, 0x30, 0x0F};
  static const uint8_t glyphR[5] = {0x7F, 0x09, 0x19, 0x29, 0x46};
  static const uint8_t glyph0[5] = {0x3E, 0x51, 0x49, 0x45, 0x3E};
  static const uint8_t glyph1[5] = {0x00, 0x42, 0x7F, 0x40, 0x00};
  static const uint8_t glyph2[5] = {0x62, 0x51, 0x49, 0x49, 0x46};
  static const uint8_t glyph3[5] = {0x22, 0x41, 0x49, 0x49, 0x36};
  static const uint8_t glyph4[5] = {0x18, 0x14, 0x12, 0x7F, 0x10};
  static const uint8_t glyph5[5] = {0x2F, 0x49, 0x49, 0x49, 0x31};
  static const uint8_t glyph6[5] = {0x3E, 0x49, 0x49, 0x49, 0x32};
  static const uint8_t glyph7[5] = {0x01, 0x71, 0x09, 0x05, 0x03};
  static const uint8_t glyph8[5] = {0x36, 0x49, 0x49, 0x49, 0x36};
  static const uint8_t glyph9[5] = {0x26, 0x49, 0x49, 0x49, 0x3E};
  const uint8_t *glyph = nullptr;

  switch (character) {
    case 'G': glyph = glyphG; break;
    case 'A': glyph = glyphA; break;
    case 'M': glyph = glyphM; break;
    case 'E': glyph = glyphE; break;
    case 'O': glyph = glyphO; break;
    case 'V': glyph = glyphV; break;
    case 'R': glyph = glyphR; break;
    case '0': glyph = glyph0; break;
    case '1': glyph = glyph1; break;
    case '2': glyph = glyph2; break;
    case '3': glyph = glyph3; break;
    case '4': glyph = glyph4; break;
    case '5': glyph = glyph5; break;
    case '6': glyph = glyph6; break;
    case '7': glyph = glyph7; break;
    case '8': glyph = glyph8; break;
    case '9': glyph = glyph9; break;
    default: return 0;
  }
  return glyph[column];
}

void scrollGameOverColumn(uint8_t incomingColumn, uint8_t displayedColumns[8], uint16_t delayMs) {
  for (int8_t column = 7; column > 0; column--) {
    displayedColumns[column] = displayedColumns[column - 1];
  }
  displayedColumns[0] = incomingColumn;

  matrix.clear();
  for (uint8_t column = 0; column < 8; column++) {
    for (uint8_t row = 0; row < 7; row++) {
      if (displayedColumns[column] & (1 << row)) {
        matrix.setPoint(row, column, true);
      }
    }
  }
  delay(delayMs);
}

void displayGameOver(uint16_t points) {
  char message[20];
  snprintf(message, sizeof(message), "GAME OVER %u", points);

  uint8_t displayedColumns[8] = {};
  size_t messageLength = strlen(message);

  for (size_t characterIndex = 0; characterIndex < messageLength; characterIndex++) {
    for (uint8_t glyphColumn = 0; glyphColumn < 6; glyphColumn++) {
      uint8_t incomingColumn = glyphColumn < 5
        ? gameOverGlyphColumn(message[characterIndex], glyphColumn)
        : 0;
      scrollGameOverColumn(incomingColumn, displayedColumns, 100);
    }
  }

  for (uint8_t column = 0; column < 8; column++) {
    scrollGameOverColumn(0, displayedColumns, 100);
  }
}

void displayScore(uint16_t points) {
  char score[6];
  snprintf(score, sizeof(score), "%u", points);

  uint8_t displayedColumns[8] = {};
  size_t scoreLength = strlen(score);
  uint8_t totalColumns = scoreLength * 6 + 8;
  uint16_t scrollDelayMs = 1000 / totalColumns;

  for (size_t characterIndex = 0; characterIndex < scoreLength; characterIndex++) {
    for (uint8_t glyphColumn = 0; glyphColumn < 6; glyphColumn++) {
      uint8_t incomingColumn = glyphColumn < 5
        ? gameOverGlyphColumn(score[characterIndex], glyphColumn)
        : 0;
      scrollGameOverColumn(incomingColumn, displayedColumns, scrollDelayMs);
    }
  }

  for (uint8_t column = 0; column < 8; column++) {
    scrollGameOverColumn(0, displayedColumns, scrollDelayMs);
  }
}

void loopOrangeRedPot() {
  if (modus != sketch) {
    return;
  }
  int blue = digitalRead(buttons[3].input);
  if (blue == LOW) {
    DEBUG_PRINTLN("Reset tekening");
    resetTekening();
  }
  uint16_t red = analogRead(1);
  uint16_t orange = abs(analogRead(2)-1023);
  uint8_t x = red / 128;
  uint8_t y = orange / 128;
  tekening[x][y] = 1;
  for (uint8_t x=0;x<8;x++) {
    for (uint8_t y=0;y<8;y++) {
      if (tekening[x][y] == 1) {
        matrix.setPoint(x, y, true);
      }
    }
  }
}

uint8_t nfc[9][7] = {
  {0x04,0x1D,0x35,0xC2,0x83,0x64,0x81},
  {0x04,0x25,0x35,0xC2,0x83,0x64,0x81},
  {0x04,0x29,0x34,0xC2,0x83,0x64,0x81},
  {0x04,0x2D,0x35,0xC2,0x83,0x64,0x81},
  {0x04,0x21,0x34,0xC2,0x83,0x64,0x81},
  {0x04,0xFA,0x31,0xC2,0x83,0x64,0x80},
  {0x04,0x13,0x32,0xC2,0x83,0x64,0x81},
  {0x04,0x0B,0x32,0xC2,0x83,0x64,0x81},
  {0x04,0x04,0x31,0xC2,0x83,0x64,0x81}
  };
//uint8_t nfc_blauwedruppel[] = {0xCA, 0x26, 0x64, 0x25};
//uint8_t nfc_card[] = {0xF2,0xF6,0x7E,0x2C};
uint8_t lastCard[10] = {};
uint8_t loopNFCSteps = 0;
void loopNFC() {
  if (modus == catchDrop || modus == catchLine) {
    return;
  }
  loopNFCSteps++;
  if (loopNFCSteps < 15) {
    return;
  }
  loopNFCSteps = 0;
  if (!mfrc522.PICC_IsNewCardPresent()) {
    DEBUG_PRINTLN("No card present");
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    DEBUG_PRINTLN("Read error");
    return;
  }
  DEBUG_PRINTLN("Card read.");
  if (memcmp(mfrc522.uid.uidByte, lastCard, mfrc522.uid.size) == 0) {
    // Same NFC as previous
    DEBUG_PRINTLN("Same as previous");
    return;
  }
  memcpy(lastCard, mfrc522.uid.uidByte, mfrc522.uid.size);
  for (uint8_t i=0; i < (sizeof(nfc)/sizeof(nfc[0])); i++) {
    if (memcmp(lastCard, nfc[i], mfrc522.uid.size) == 0) {
      uint8_t fileIndex = i; // 0-based
      DEBUG_PRINT("Card: ");
      DEBUG_PRINTLN(fileIndex);
      printLetter(fileIndex);
      return;
    }
  }
  DEBUG_PRINTLN("Unknown NFC: ");
}

/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    DEBUG_PRINT(buffer[i] < 0x10 ? " 0" : " ");
    DEBUG_PRINT(buffer[i], HEX);
  }
}
