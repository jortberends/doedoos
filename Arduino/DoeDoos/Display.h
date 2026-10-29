#pragma once

#include <Arduino.h>
#include <MD_MAX72xx.h>

class DisplayController {
public:
  DisplayController();

  void begin();
  void clear();
  void setPoint(uint8_t row, uint8_t column, bool state);
  void setColumn(uint8_t column, uint8_t value);
  void loopIcons();
  void printLetter(uint8_t index);
  void nextIcon();
  void mirrorIcon();
  void updateAnimation();
  void displayGameOver(uint16_t points);
  void displayScore(uint16_t points);
  bool isAnimating() const;
  void cancelAnimation();

private:
  uint8_t gameOverGlyphColumn(char character, uint8_t column) const;
  void scrollGameOverColumn(uint8_t incomingColumn);
  void startAnimation(const char *text, uint16_t intervalMs);
  void printIcon(uint8_t index);

  MD_MAX72XX matrix_;
  bool animationRunning_;
  char animationText_[20];
  size_t animationTextLength_;
  size_t animationCharacter_;
  uint8_t animationGlyphColumn_;
  uint8_t animationTrailingColumns_;
  uint8_t animationColumns_[8];
  uint16_t animationIntervalMs_;
  unsigned long animationLastUpdate_;
  bool animationFirstFrame_;
  uint8_t currentIcon_;
  bool isMirroring_;
  uint8_t iconSteps_;
};

extern DisplayController displayController;
