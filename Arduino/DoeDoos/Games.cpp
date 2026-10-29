#include "Games.h"
#include "AppState.h"
#include "Display.h"
#include "Debug.h"

namespace {
const unsigned long CATCH_DROP_BASE_INTERVAL_MS = 200;
const unsigned long CATCH_DROP_MIN_INTERVAL_MS = 1;
const unsigned long CATCH_LINE_BUTTON_DEBOUNCE_MS = 30;
const unsigned long CATCH_LINE_INTERVAL_MS = 200;
}

CatchDropGame catchDropGame;
CatchLineGame catchLineGame;

CatchDropGame::CatchDropGame()
  : points_(0),
    playerX_(0),
    lastPlayerX_(0),
    drops_{8,8,8,8,8,8,8,8},
    intervalMs_(CATCH_DROP_BASE_INTERVAL_MS),
    lastUpdate_(0),
    needsSpawn_(true) {}

void CatchDropGame::update() {
  if (modus != catchDrop) {
    return;
  }
  updateControl();
  updateDrops();
}

void CatchDropGame::updateControl() {
  uint16_t controlKnob = analogRead(GOLDEN_POT);
  playerX_ = controlKnob / 147;
  if (playerX_ == lastPlayerX_) {
    return;
  }
  DEBUG_PRINT("CatchDropBar: ");
  DEBUG_PRINTLN(playerX_);
  draw();
  lastPlayerX_ = playerX_;
}

void CatchDropGame::updateDrops() {
  unsigned long currentMillis = millis();
  if (currentMillis - lastUpdate_ < intervalMs_) {
    return;
  }
  lastUpdate_ = currentMillis;

  for (uint8_t i = 0; i < 8; i++) {
    if (drops_[i] < 8) {
      drops_[i]++;
      if (drops_[i] == 7) {
        needsSpawn_ = true;
        if (i == playerX_ || i == playerX_ + 1) {
          points_++;
          if (intervalMs_ > CATCH_DROP_MIN_INTERVAL_MS) {
            intervalMs_ -= 5;
          }
        } else {
          if (points_ > 0) {
            displayController.displayGameOver(points_);
          }
          intervalMs_ = CATCH_DROP_BASE_INTERVAL_MS;
          points_ = 0;
          digitalWrite(5, LOW);
          digitalWrite(6, LOW);
          digitalWrite(7, LOW);
        }
      }
    }
  }

  if (needsSpawn_) {
    needsSpawn_ = false;
    uint8_t newDropAt = random(0, 8);
    drops_[newDropAt] = 0;
  }
  draw();
}

void CatchDropGame::draw() {
  displayController.clear();
  displayController.setPoint(7, playerX_, true);
  displayController.setPoint(7, playerX_ + 1, true);
  for (uint8_t i = 0; i < 8; i++) {
    if (drops_[i] < 8) {
      displayController.setPoint(drops_[i], i, true);
    }
  }
}

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
      displayController.displayScore(points_);
    } else {
      DEBUG_PRINTLN("Missed the line!");
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
