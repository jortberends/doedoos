#include "Inputs.h"
#include "AppState.h"
#include "Display.h"
#include "Debug.h"

constexpr uint8_t FLIP_LEFT = 44;
constexpr uint8_t FLIP_RIGHT = 45;

void clearButtons();
void clearLeds();

int ledPins[] = {8,23,25,27,29,31};

uint8_t buttonToPress = 0;

int flipLeftLastValue = LOW;

int flipRightLastValue = LOW;

bool vibratorWorking=false;

uint8_t buttonsSteps = 0;

uint8_t buttonsStepsValue = HIGH;

int tornLastValue = HIGH;

uint16_t blauwePotSteps = 0;

uint16_t blauwePotState = HIGH;

void setupFlipButtons() {
  pinMode(FLIP_LEFT, INPUT_PULLUP);
  pinMode(FLIP_RIGHT, INPUT_PULLUP);
}


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


void loopEmergency() {
  if (modus == catchDrop || modus == catchLine) {
    if (vibratorWorking) {
      vibratorWorking=false;
      analogWrite(VIBRATOR, 0);
    }
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
    DEBUG_PRINT_NEWLINE();
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
