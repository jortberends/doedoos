#pragma once

#include <Arduino.h>

struct Button {
  uint8_t led;
  uint8_t input;
};

enum Modus {
  standaard,
  sketch,
  catchDrop,
  catchLine,
  spaceInvaders
};

constexpr uint8_t VIBRATOR = 11;
constexpr uint8_t EMERGENCY = 12;
constexpr uint8_t GOLDEN_POT = 58;

extern Button buttons[9];
extern Modus modus;
