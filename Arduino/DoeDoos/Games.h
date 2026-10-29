#pragma once

#include <Arduino.h>

class CatchDropGame {
public:
  CatchDropGame();
  void update();

private:
  void updateControl();
  void updateDrops();
  void draw();

  uint8_t points_;
  uint8_t playerX_;
  uint8_t lastPlayerX_;
  uint8_t drops_[8];
  unsigned long intervalMs_;
  unsigned long lastUpdate_;
  bool needsSpawn_;
};

class CatchLineGame {
public:
  CatchLineGame();
  void update();

private:
  void reset();
  void draw();

  uint8_t position_;
  bool movingRight_;
  uint16_t points_;
  int buttonState_;
  int lastButtonReading_;
  unsigned long lastButtonChange_;
  unsigned long lastUpdate_;
};

class SpaceInvadersGame {
public:
  SpaceInvadersGame();
  void update();

private:
  void reset();
  void updateRocket();
  void updateShooting(unsigned long currentMillis);
  void updateInvader(unsigned long currentMillis);
  void checkCollision();
  void draw();
  void finishGame();

  bool wasActive_;
  bool gameOver_;
  uint16_t points_;
  uint8_t rocketX_;
  uint8_t invaderLeftX_;
  uint8_t invaderY_[3];
  bool invaderAlive_[3];
  int8_t invaderDirection_;
  bool bulletActive_;
  uint8_t bulletX_;
  uint8_t bulletY_;
  int fireButtonState_;
  int lastFireButtonReading_;
  unsigned long lastFireButtonChange_;
  unsigned long lastBulletUpdate_;
  unsigned long lastInvaderUpdate_;
  unsigned long lastDrawUpdate_;
  bool hasDrawn_;
};

extern CatchDropGame catchDropGame;
extern CatchLineGame catchLineGame;
extern SpaceInvadersGame spaceInvadersGame;
