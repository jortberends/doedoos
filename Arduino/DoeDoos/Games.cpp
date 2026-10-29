#include "Games.h"
#include "AppState.h"
#include "Display.h"
#include "Debug.h"

void loopCatchDropDrop();
void drawCatchDrop();

uint8_t catchDropPoints=0;

uint8_t xPos = 0;

uint8_t lastXPos = 0;

uint8_t drops[] = {8,8,8,8,8,8,8,8};

const unsigned long CATCH_DROP_BASE_INTERVAL_MS = 200;

const unsigned long CATCH_DROP_MIN_INTERVAL_MS = 1;

unsigned long catchDropIntervalMs = CATCH_DROP_BASE_INTERVAL_MS;

unsigned long lastCatchDropUpdate = 0;

bool catchDropNeedSpawn = true;

uint8_t xPosCatchLine = 0;

bool catchLineMovingRight = true;

uint16_t catchLinePoints = 0;

int catchLineButtonState = HIGH;

int lastCatchLineButtonReading = HIGH;

unsigned long lastCatchLineButtonChange = 0;

const unsigned long CATCH_LINE_BUTTON_DEBOUNCE_MS = 30;

const unsigned long CATCH_LINE_INTERVAL_MS = 200;

unsigned long lastCatchLineUpdate = 0;

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
    uint8_t newDropAt = random(0,8);
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
