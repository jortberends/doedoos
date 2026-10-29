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

extern CatchDropGame catchDropGame;
extern CatchLineGame catchLineGame;
