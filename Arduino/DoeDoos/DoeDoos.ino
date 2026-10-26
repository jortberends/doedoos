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

int flipLeftLastValue = HIGH;
int flipRightLastValue = HIGH;
void loopFlipButtons() {
  int left = digitalRead(FLIP_LEFT);
  int right = digitalRead(FLIP_RIGHT);
  if (left == flipLeftLastValue && right == flipRightLastValue) {
    return;
  }
  flipRightLastValue = right;
  flipLeftLastValue = left;

  if (right == HIGH) {
    if (left == HIGH) {
      Serial.println("Modus to catch the line");
      modus = catchLine;
    } else {
      Serial.println("Modus to catch the drop");
      modus = catchDrop;
    }
    matrix.clear();
    clearButtons();
  } else {
    if (left == HIGH) {
      Serial.println("Modus to sketch");
      modus = sketch;
    } else {
      Serial.println("Modus to default");
      modus = standaard;
    }
    matrix.clear();
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

void clearButtons() {
  for (uint8_t i=0; i < (sizeof(buttons)/sizeof(buttons[0])); i++) {
    digitalWrite(buttons[i].led, LOW);
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
  
  Serial.print("Vibrator ON - a button is ");
  if (aButtonIsBeingPressed) {
    Serial.println("pressed");
    analogWrite(VIBRATOR, 255);
  } else {
    Serial.println("NOT pressed");
    analogWrite(VIBRATOR, 0);
  }
  Serial.println("=====================");
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
  currentIcon = index;
}

bool isMirroring = false;
void mirrorIcon() {
  isMirroring = !isMirroring;
  Serial.print("Mirror icon: ");
  Serial.println(isMirroring);
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
    Serial.print("New button to press: ");
    Serial.println(buttons[buttonToPress].input);
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
  Serial.print("Torn val:");
  Serial.println(val);
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
  Serial.print("CatchDropBar: ");
  Serial.println(xPos);
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
uint16_t catchDropsStep = 0;
uint8_t catchDropSpeed = 60;
bool catchDropNeedSpawn = true;
void loopCatchDropDrop() {
  if (catchDropsStep < catchDropSpeed) {
    catchDropsStep++;
    return;
  }
  catchDropsStep=0;
  for (uint8_t i=0;i<8;i++) {
    if (drops[i] < 8) {
      drops[i] = drops[i]+1;
      if (drops[i] == 7) {
        catchDropNeedSpawn=true;
        if (i == xPos || i == xPos+1) {
          catchDropPoints++;
          if (catchDropPoints == 0) {
            digitalWrite(5, LOW);
            digitalWrite(6, LOW);
            digitalWrite(7, LOW);
          } else if (catchDropPoints == 1) {
            digitalWrite(5,HIGH);
          } else if (catchDropPoints == 2) {
            digitalWrite(6, HIGH);
          } else if (catchDropPoints == 3) {
            digitalWrite(7, HIGH);
          } else if (catchDropPoints == 4) {
            digitalWrite(5, LOW);
            digitalWrite(6, LOW);
            digitalWrite(7, LOW);
            catchDropPoints=0;
            catchDropSpeed=catchDropSpeed-3;
          }
        } else {
          // lives meganisme
          catchDropSpeed=60;
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
const unsigned long CATCH_LINE_INTERVAL_MS = 100;
unsigned long lastCatchLineUpdate = 0;

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
      Serial.println("Caught the line!");
      catchLinePoints++;
      Serial.print("Catch the line points: ");
      Serial.println(catchLinePoints);
    } else {
      Serial.println("Missed the line!");
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

void loopOrangeRedPot() {
  if (modus != sketch) {
    return;
  }
  int blue = digitalRead(buttons[3].input);
  if (blue == LOW) {
    Serial.println("Reset tekening");
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
  memcpy(lastCard, mfrc522.uid.uidByte, mfrc522.uid.size);
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
