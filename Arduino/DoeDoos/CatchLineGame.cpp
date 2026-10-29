#include "Games.h"
#include "AppState.h"
#include "Display.h"
#include "Debug.h"
#include "Haptics.h"

namespace {
const unsigned long CATCH_LINE_BUTTON_DEBOUNCE_MS = 30;
const unsigned long CATCH_LINE_INTERVAL_MS = 200;
}

CatchLineGame catchLineGame;

CatchLineGame::CatchLineGame()
  : position_(0),
    movingRight_(true),
    points_(0),
    buttonState_(HIGH),
    lastButtonReading_(HIGH),
    lastButtonChange_(0),
    lastUpdate_(0) {}

void CatchLineGame::reset() {
  position_ = 0;
  movingRight_ = true;
  points_ = 0;
  buttonState_ = HIGH;
  lastButtonReading_ = HIGH;
  lastButtonChange_ = 0;
  lastUpdate_ = 0;
}

void CatchLineGame::update() {
  if (modus != catchLine) {
    return;
  }

  int buttonReading = digitalRead(buttons[3].input);
  if (buttonReading != lastButtonReading_) {
    lastButtonChange_ = millis();
    lastButtonReading_ = buttonReading;
  }
  bool buttonPressed = false;
  if (millis() - lastButtonChange_ >= CATCH_LINE_BUTTON_DEBOUNCE_MS &&
      buttonReading != buttonState_) {
    buttonState_ = buttonReading;
    buttonPressed = buttonState_ == LOW;
  }
  if (buttonPressed) {
    if (position_ == 4) {
      DEBUG_PRINTLN("Caught the line!");
      points_++;
      DEBUG_PRINT("Catch the line points: ");
      DEBUG_PRINTLN(points_);
      startHitVibration();
      displayController.displayScore(points_);
    } else {
      DEBUG_PRINTLN("Missed the line!");
      startGameOverVibration();
      displayController.displayGameOver(points_);
      reset();
      return;
    }
  }

  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate_ < CATCH_LINE_INTERVAL_MS) {
    return;
  }
  lastUpdate_ = currentMillis;

  draw();
}

void CatchLineGame::draw() {
  displayController.clear();
  if (position_ == 8) {
    movingRight_ = false;
  } else if (position_ == 0) {
    movingRight_ = true;
  }
  if (movingRight_) {
    position_++;
  } else {
    position_--;
  }

  if (position_ < 8) {
    displayController.setColumn(position_, 0xFF);
  }
  if (position_ > 0) {
    displayController.setColumn(position_ - 1, 0xFF);
  }
  displayController.setPoint(7, 3, true);
  displayController.setPoint(7, 4, true);
}
