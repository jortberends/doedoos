#include "Games.h"
#include "AppState.h"
#include "Display.h"
#include "Debug.h"

namespace {
const unsigned long CATCH_DROP_BASE_INTERVAL_MS = 200;
const unsigned long CATCH_DROP_MIN_INTERVAL_MS = 1;
}

CatchDropGame catchDropGame;

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
