#include "Inputs.h"
#include "AppState.h"
#include "Display.h"
#include "Debug.h"

constexpr uint8_t FLIP_LEFT = 44;
constexpr uint8_t FLIP_RIGHT = 45;
constexpr unsigned long TORN_BUTTON_DEBOUNCE_MS = 30;

void clearButtons();
void clearLeds();

int ledPins[] = {8,23,25,27,29,31};

uint8_t buttonToPress = 0;

int flipLeftLastValue = LOW;

int flipRightLastValue = LOW;

bool vibratorWorking=false;

uint8_t buttonsSteps = 0;

uint8_t buttonsStepsValue = HIGH;

int tornLastReading = HIGH;
int tornStableState = HIGH;
unsigned long tornLastChange = 0;

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

  displayController.clear();
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
  if (modus == catchDrop || modus == catchLine || modus == spaceInvaders) {
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
      displayController.setPoint(x, y, true);
      delay(50);
      displayController.clear();
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
  if (modus == catchDrop || modus == catchLine || modus == spaceInvaders) {
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
      displayController.nextIcon();
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
  if (modus == catchDrop || modus == catchLine || modus == spaceInvaders) {
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
  tornLastReading = digitalRead(46);
  tornStableState = tornLastReading;
  tornLastChange = millis();
}


void loopTorn() {
  int reading = digitalRead(46);
  if (reading != tornLastReading) {
    tornLastReading = reading;
    tornLastChange = millis();
  }

  if (millis() - tornLastChange < TORN_BUTTON_DEBOUNCE_MS ||
      tornLastReading == tornStableState) {
    return;
  }

  tornStableState = tornLastReading;
  DEBUG_PRINT("Torn val:");
  DEBUG_PRINTLN(tornStableState);
  if (modus == standaard) {
    displayController.mirrorIcon();
    return;
  }
  if (tornStableState != LOW ||
      (modus != catchDrop && modus != catchLine && modus != spaceInvaders)) {
    return;
  }

  if (modus == catchDrop) {
    modus = catchLine;
    DEBUG_PRINTLN("Modus to catch the line");
  } else if (modus == catchLine) {
    modus = spaceInvaders;
    DEBUG_PRINTLN("Modus to Space Invaders");
  } else {
    modus = catchDrop;
    DEBUG_PRINTLN("Modus to catch the drop");
  }

  displayController.clear();
  clearButtons();
  clearLeds();
  if (modus == catchLine) {
    digitalWrite(buttons[3].led, HIGH);
  }
}


void loopBlauwePot() {
  if (modus == catchDrop || modus == catchLine || modus == spaceInvaders) {
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
