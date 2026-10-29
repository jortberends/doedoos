#include "Display.h"
#include "AppState.h"
#include "Debug.h"
#include <avr/pgmspace.h>
#include <stdio.h>
#include <string.h>

MD_MAX72XX matrix = MD_MAX72XX(MD_MAX72XX::ICSTATION_HW, 36, 1);

const uint8_t letters[9][8][8] PROGMEM = {
  { // E lin
    {0,1,1,1,1,1,1,0},
    {0,1,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0},
    {0,1,1,1,1,1,0,0},
    {0,1,0,0,0,0,0,0},
    {0,1,0,0,0,0,0,0},
    {0,1,1,1,1,1,1,0}
  }, { // O ma
    {0,0,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,0,1,1,1,1,0,0}
  }, { // J
    {0,1,1,1,1,1,1,0},
    {0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,1,0},
    {0,0,0,0,0,1,0,0},
    {0,1,1,1,1,0,0,0}
  }, { // M ama
    {1,0,0,0,0,0,0,1},
    {1,1,0,0,0,0,1,1},
    {1,0,1,0,0,1,0,1},
    {1,0,0,1,1,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1}
  }, { // R imme
    {0,1,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,1,1,1,1,0,0},
    {0,1,1,0,0,0,0,0},
    {0,1,0,1,0,0,0,0},
    {0,1,0,0,1,0,0,0},
    {0,1,0,0,0,1,0,0}
  }, { // K im
    {0,1,0,0,0,1,0,0},
    {0,1,0,0,1,0,0,0},
    {0,1,0,1,0,0,0,0},
    {0,1,1,0,0,0,0,0},
    {0,1,1,0,0,0,0,0},
    {0,1,0,1,0,0,0,0},
    {0,1,0,0,1,0,0,0},
    {0,1,0,0,0,1,0,0}
  }, { // B ram
    {0,1,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,1,1,1,1,0,0}
  }, { // S anne
    {0,0,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,0,0},
    {0,0,1,1,1,1,0,0},
    {0,0,0,0,0,0,1,0},
    {0,0,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,0,1,1,1,1,0,0}
  }, { // O pa
    {0,0,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,1,0,0,0,0,1,0},
    {0,0,1,1,1,1,0,0}
  }
};

const uint8_t icon[7][8][8] PROGMEM = {
  { // Happy
    {0,0,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {1,0,1,0,0,1,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,1,0,0,1,0,1},
    {1,0,0,1,1,0,0,1},
    {0,1,0,0,0,0,1,0},
    {0,0,1,1,1,1,0,0}
  },
//  { // Sad
//    {0,0,1,1,1,1,0,0},
//    {0,1,0,0,0,0,1,0},
//    {1,0,1,0,0,1,0,1},
//    {1,0,0,0,0,0,0,1},
//    {1,0,0,1,1,0,0,1},
//    {1,0,1,0,0,1,0,1},
//    {0,1,0,0,0,0,1,0},
//    {0,0,1,1,1,1,0,0}
//  },
  { // Kat
    {1,0,0,0,1,0,0,0},
    {1,1,1,1,1,0,0,0},
    {1,0,1,0,1,0,0,0},
    {0,1,1,1,0,0,0,1},
    {0,0,1,0,0,0,0,1},
    {0,1,1,1,1,0,0,1},
    {0,1,1,1,1,1,0,1},
    {1,0,1,1,1,1,1,0}
  },
  { // Hond
    {0,0,1,0,0,0,0,0},
    {1,1,1,0,0,0,0,0},
    {1,1,1,0,0,0,0,1},
    {0,0,1,1,1,1,1,0},
    {0,0,1,1,1,1,1,0},
    {0,0,1,1,1,1,1,0},
    {0,0,1,0,0,0,1,0},
    {0,1,1,0,0,1,1,0}
  },
  { // Vogel
    {0,1,1,1,0,0,0,0},
    {1,1,0,1,0,0,0,0},
    {0,1,1,1,0,0,0,0},
    {0,1,1,1,1,0,0,0},
    {0,1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1,0},
    {0,1,1,1,1,0,0,0},
    {0,0,1,0,0,0,0,0}
  },
  { // Vis
    {0,0,0,0,0,0,0,0},
    {0,0,0,1,1,0,0,0},
    {0,0,1,0,0,1,0,1},
    {0,1,0,1,0,0,1,1},
    {1,0,0,0,0,0,0,1},
    {0,1,0,0,0,0,1,1},
    {0,0,1,0,0,1,0,1},
    {0,0,0,1,1,0,0,0}
  },
  { // Klok
    {0,0,1,1,1,1,0,0},
    {0,1,0,0,0,0,1,0},
    {1,0,0,1,0,0,0,1},
    {1,0,0,1,0,0,0,1},
    {1,0,0,1,1,1,0,1},
    {1,0,0,0,0,0,0,1},
    {0,1,0,0,0,0,1,0},
    {0,0,1,1,1,1,0,0}
  },
  { // Huis
    {0,0,0,1,1,0,0,0},
    {0,0,1,0,0,1,0,0},
    {0,1,0,0,0,0,1,0},
    {1,0,0,0,1,1,0,1},
    {1,0,0,0,1,1,0,1},
    {1,0,1,1,0,0,0,1},
    {1,0,1,1,0,0,0,1},
    {1,1,1,1,1,1,1,1}
  }
};

bool displayAnimationRunning = false;

char displayAnimationText[20];

size_t displayAnimationTextLength = 0;

size_t displayAnimationCharacter = 0;

uint8_t displayAnimationGlyphColumn = 0;

uint8_t displayAnimationTrailingColumns = 0;

uint8_t displayAnimationColumns[8] = {};

uint16_t displayAnimationIntervalMs = 0;

unsigned long displayAnimationLastUpdate = 0;

bool displayAnimationFirstFrame = true;
uint8_t currentIcon = 0;
bool isMirroring = false;
uint8_t iconSteps = 0;

void loopIcons() {
  if (iconSteps == 255) {
    nextIcon();
    iconSteps = 0;
  } else {
    iconSteps++;
  }
}


void printLetter(uint8_t index) {
  DEBUG_PRINT("PrintLetter:");
  DEBUG_PRINTLN(index);
  matrix.clear();
  for (uint8_t x=0; x < (sizeof(letters[0])/sizeof(letters[0][0])); x++) {
    for (uint8_t y = 0; y < (sizeof(letters[0][0])/sizeof(letters[0][0][0])); y++) {
      if (pgm_read_byte(&letters[index][x][abs(y-7)]) == 1) {
        matrix.setPoint(x, y, true);
      }
    }
  }
}


void nextIcon() {
  currentIcon++;
  if (currentIcon == (sizeof(icon)/sizeof(icon[0]))) {
    currentIcon = 0;
  }
  printIcon(currentIcon);
}


void printIcon(uint8_t index) {
  DEBUG_PRINT("Print smiley: ");
  DEBUG_PRINTLN(index);
  matrix.clear();
  for (uint8_t x=0; x < (sizeof(icon[0])/sizeof(icon[0][0])); x++) {
    for (uint8_t y = 0; y < (sizeof(icon[0][0])/sizeof(icon[0][0][0])); y++) {
      if (pgm_read_byte(&icon[index][x][y]) == 1) {
        matrix.setPoint(x, y, true);
      }
    }
  }
  currentIcon = index;
}


void mirrorIcon() {
  isMirroring = !isMirroring;
  DEBUG_PRINT("Mirror icon: ");
  DEBUG_PRINTLN(isMirroring);
  matrix.clear();
  for (uint8_t x=0; x < (sizeof(icon[0])/sizeof(icon[0][0])); x++) {
    for (uint8_t y = 0; y < (sizeof(icon[0][0])/sizeof(icon[0][0][0])); y++) {
      if (pgm_read_byte(&icon[currentIcon][x][y]) == 1) {
        uint8_t actualY = y;
        if (isMirroring) {
          actualY = abs(actualY-7);
        }
        matrix.setPoint(x, actualY, true);
      }
    }
  }
}


uint8_t gameOverGlyphColumn(char character, uint8_t column) {
  static const uint8_t glyphG[5] = {0x7E, 0x41, 0x49, 0x49, 0x3A};
  static const uint8_t glyphA[5] = {0x7E, 0x11, 0x11, 0x11, 0x7E};
  static const uint8_t glyphM[5] = {0x7F, 0x06, 0x18, 0x06, 0x7F};
  static const uint8_t glyphE[5] = {0x7F, 0x49, 0x49, 0x49, 0x41};
  static const uint8_t glyphO[5] = {0x3E, 0x41, 0x41, 0x41, 0x3E};
  static const uint8_t glyphV[5] = {0x0F, 0x30, 0x40, 0x30, 0x0F};
  static const uint8_t glyphR[5] = {0x7F, 0x09, 0x19, 0x29, 0x46};
  static const uint8_t glyph0[5] = {0x3E, 0x51, 0x49, 0x45, 0x3E};
  static const uint8_t glyph1[5] = {0x00, 0x42, 0x7F, 0x40, 0x00};
  static const uint8_t glyph2[5] = {0x62, 0x51, 0x49, 0x49, 0x46};
  static const uint8_t glyph3[5] = {0x22, 0x41, 0x49, 0x49, 0x36};
  static const uint8_t glyph4[5] = {0x18, 0x14, 0x12, 0x7F, 0x10};
  static const uint8_t glyph5[5] = {0x2F, 0x49, 0x49, 0x49, 0x31};
  static const uint8_t glyph6[5] = {0x3E, 0x49, 0x49, 0x49, 0x32};
  static const uint8_t glyph7[5] = {0x01, 0x71, 0x09, 0x05, 0x03};
  static const uint8_t glyph8[5] = {0x36, 0x49, 0x49, 0x49, 0x36};
  static const uint8_t glyph9[5] = {0x26, 0x49, 0x49, 0x49, 0x3E};
  const uint8_t *glyph = nullptr;

  switch (character) {
    case 'G': glyph = glyphG; break;
    case 'A': glyph = glyphA; break;
    case 'M': glyph = glyphM; break;
    case 'E': glyph = glyphE; break;
    case 'O': glyph = glyphO; break;
    case 'V': glyph = glyphV; break;
    case 'R': glyph = glyphR; break;
    case '0': glyph = glyph0; break;
    case '1': glyph = glyph1; break;
    case '2': glyph = glyph2; break;
    case '3': glyph = glyph3; break;
    case '4': glyph = glyph4; break;
    case '5': glyph = glyph5; break;
    case '6': glyph = glyph6; break;
    case '7': glyph = glyph7; break;
    case '8': glyph = glyph8; break;
    case '9': glyph = glyph9; break;
    default: return 0;
  }
  return glyph[column];
}


void scrollGameOverColumn(uint8_t incomingColumn, uint8_t displayedColumns[8]) {
  for (int8_t column = 7; column > 0; column--) {
    displayedColumns[column] = displayedColumns[column - 1];
  }
  displayedColumns[0] = incomingColumn;

  matrix.clear();
  for (uint8_t column = 0; column < 8; column++) {
    for (uint8_t row = 0; row < 7; row++) {
      if (displayedColumns[column] & (1 << row)) {
        matrix.setPoint(row, column, true);
      }
    }
  }
}


void startDisplayAnimation(const char *text, uint16_t intervalMs) {
  snprintf(displayAnimationText, sizeof(displayAnimationText), "%s", text);
  displayAnimationTextLength = strlen(displayAnimationText);
  displayAnimationCharacter = 0;
  displayAnimationGlyphColumn = 0;
  displayAnimationTrailingColumns = 0;
  memset(displayAnimationColumns, 0, sizeof(displayAnimationColumns));
  displayAnimationIntervalMs = intervalMs;
  displayAnimationLastUpdate = millis();
  displayAnimationFirstFrame = true;
  displayAnimationRunning = true;
}


void updateDisplayAnimation() {
  if (!displayAnimationRunning) {
    return;
  }

  unsigned long currentMillis = millis();
  if (!displayAnimationFirstFrame &&
      currentMillis - displayAnimationLastUpdate < displayAnimationIntervalMs) {
    return;
  }
  displayAnimationFirstFrame = false;
  displayAnimationLastUpdate = currentMillis;

  uint8_t incomingColumn = 0;
  if (displayAnimationCharacter < displayAnimationTextLength) {
    if (displayAnimationGlyphColumn < 5) {
      incomingColumn = gameOverGlyphColumn(
        displayAnimationText[displayAnimationCharacter],
        displayAnimationGlyphColumn
      );
    }
    displayAnimationGlyphColumn++;
    if (displayAnimationGlyphColumn == 6) {
      displayAnimationGlyphColumn = 0;
      displayAnimationCharacter++;
    }
  } else {
    displayAnimationTrailingColumns++;
  }

  scrollGameOverColumn(incomingColumn, displayAnimationColumns);
  if (displayAnimationTrailingColumns == 8) {
    displayAnimationRunning = false;
  }
}


void displayGameOver(uint16_t points) {
  char message[20];
  snprintf(message, sizeof(message), "GAME OVER %u", points);
  startDisplayAnimation(message, 100);
}


void displayScore(uint16_t points) {
  char score[6];
  snprintf(score, sizeof(score), "%u", points);

  size_t scoreLength = strlen(score);
  uint8_t totalColumns = scoreLength * 6 + 8;
  uint16_t scrollDelayMs = 1000 / totalColumns;
  startDisplayAnimation(score, scrollDelayMs);
}


bool displayIsAnimating() {
  return displayAnimationRunning;
}

void cancelDisplayAnimation() {
  displayAnimationRunning = false;
}

void displayInit() {
  matrix.begin();
}
