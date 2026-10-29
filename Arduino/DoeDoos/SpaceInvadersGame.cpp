#include "Games.h"
#include "AppState.h"
#include "Display.h"
#include "Debug.h"

namespace {
const unsigned long SPACE_INVADERS_BUTTON_DEBOUNCE_MS = 30;
const unsigned long SPACE_INVADERS_BULLET_INTERVAL_MS = 100;
const unsigned long SPACE_INVADERS_INVADER_INTERVAL_MS = 400;
const unsigned long SPACE_INVADERS_DRAW_INTERVAL_MS = 50;
const uint8_t SPACE_INVADERS_COUNT = 3;
const uint8_t SPACE_INVADERS_MAX_LEFT_X = 8 - SPACE_INVADERS_COUNT;
}

SpaceInvadersGame spaceInvadersGame;

SpaceInvadersGame::SpaceInvadersGame()
  : wasActive_(false),
    gameOver_(false),
    points_(0),
    rocketX_(3),
    invaderLeftX_(0),
    invaderY_{0, 0, 0},
    invaderAlive_{true, true, true},
    invaderDirection_(1),
    bulletActive_(false),
    bulletX_(0),
    bulletY_(0),
    fireButtonState_(HIGH),
    lastFireButtonReading_(HIGH),
    lastFireButtonChange_(0),
    lastBulletUpdate_(0),
    lastInvaderUpdate_(0),
    lastDrawUpdate_(0),
    hasDrawn_(false) {}

void SpaceInvadersGame::reset() {
  gameOver_ = false;
  points_ = 0;
  rocketX_ = 3;
  invaderLeftX_ = random(0, SPACE_INVADERS_MAX_LEFT_X + 1);
  for (uint8_t i = 0; i < SPACE_INVADERS_COUNT; i++) {
    invaderY_[i] = 0;
    invaderAlive_[i] = true;
  }
  invaderDirection_ = random(0, 2) == 0 ? -1 : 1;
  bulletActive_ = false;
  bulletX_ = 0;
  bulletY_ = 0;
  fireButtonState_ = HIGH;
  lastFireButtonReading_ = HIGH;
  lastFireButtonChange_ = millis();
  lastBulletUpdate_ = millis();
  lastInvaderUpdate_ = millis();
  lastDrawUpdate_ = millis();
  hasDrawn_ = false;
}

void SpaceInvadersGame::update() {
  if (modus != spaceInvaders) {
    wasActive_ = false;
    return;
  }
  if (!wasActive_) {
    reset();
    wasActive_ = true;
  }
  if (gameOver_) {
    return;
  }

  unsigned long currentMillis = millis();
  updateRocket();
  updateShooting(currentMillis);
  updateInvader(currentMillis);
  checkCollision();
  if (!gameOver_) {
    draw();
  }
}

void SpaceInvadersGame::updateRocket() {
  rocketX_ = analogRead(GOLDEN_POT) / 128;
  if (rocketX_ > 7) {
    rocketX_ = 7;
  }
}

void SpaceInvadersGame::updateShooting(unsigned long currentMillis) {
  int reading = digitalRead(buttons[3].input);
  if (reading != lastFireButtonReading_) {
    lastFireButtonChange_ = currentMillis;
    lastFireButtonReading_ = reading;
  }
  if (currentMillis - lastFireButtonChange_ >= SPACE_INVADERS_BUTTON_DEBOUNCE_MS &&
      reading != fireButtonState_) {
    fireButtonState_ = reading;
    if (fireButtonState_ == LOW && !bulletActive_) {
      bulletActive_ = true;
      bulletX_ = rocketX_;
      bulletY_ = 6;
      lastBulletUpdate_ = currentMillis;
    }
  }

  if (!bulletActive_ ||
      currentMillis - lastBulletUpdate_ < SPACE_INVADERS_BULLET_INTERVAL_MS) {
    return;
  }
  lastBulletUpdate_ = currentMillis;
  if (bulletY_ == 0) {
    bulletActive_ = false;
    return;
  }
  bulletY_--;
}

void SpaceInvadersGame::updateInvader(unsigned long currentMillis) {
  if (currentMillis - lastInvaderUpdate_ < SPACE_INVADERS_INVADER_INTERVAL_MS) {
    return;
  }
  lastInvaderUpdate_ = currentMillis;

  int8_t nextX = static_cast<int8_t>(invaderLeftX_) + invaderDirection_;
  if (nextX < 0 || nextX > SPACE_INVADERS_MAX_LEFT_X) {
    invaderDirection_ = -invaderDirection_;
    for (uint8_t i = 0; i < SPACE_INVADERS_COUNT; i++) {
      if (invaderAlive_[i]) {
        invaderY_[i]++;
        if (invaderY_[i] == 7) {
          finishGame();
          return;
        }
      }
    }
  } else {
    invaderLeftX_ = static_cast<uint8_t>(nextX);
  }
}

void SpaceInvadersGame::checkCollision() {
  if (!bulletActive_) {
    return;
  }

  for (uint8_t i = 0; i < SPACE_INVADERS_COUNT; i++) {
    uint8_t invaderX = invaderLeftX_ + i;
    if (invaderAlive_[i] && bulletX_ == invaderX && bulletY_ == invaderY_[i]) {
      bulletActive_ = false;
      points_++;
      invaderY_[i] = 0;
      return;
    }
  }
}

void SpaceInvadersGame::draw() {
  unsigned long currentMillis = millis();
  if (hasDrawn_ &&
      currentMillis - lastDrawUpdate_ < SPACE_INVADERS_DRAW_INTERVAL_MS) {
    return;
  }
  lastDrawUpdate_ = currentMillis;
  hasDrawn_ = true;
  displayController.clear();
  displayController.setPoint(7, rocketX_, true);
  for (uint8_t i = 0; i < SPACE_INVADERS_COUNT; i++) {
    if (invaderAlive_[i]) {
      displayController.setPoint(invaderY_[i], invaderLeftX_ + i, true);
    }
  }
  if (bulletActive_) {
    displayController.setPoint(bulletY_, bulletX_, true);
  }
}

void SpaceInvadersGame::finishGame() {
  gameOver_ = true;
  bulletActive_ = false;
  displayController.displayGameOver(points_);
  reset();
}
