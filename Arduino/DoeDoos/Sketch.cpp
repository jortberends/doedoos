#include "Sketch.h"
#include "AppState.h"
#include "Display.h"
#include "Inputs.h"
#include "Debug.h"

uint8_t tekening[8][8] = {
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0}
  };

void resetTekening() {
  for (uint8_t x=0;x<8;x++) {
    for (uint8_t y=0;y<8;y++) {
      tekening[x][y] = 0;
    }
  }
  matrix.clear();
}


void loopOrangeRedPot() {
  if (modus != sketch) {
    return;
  }
  int blue = digitalRead(buttons[3].input);
  if (blue == LOW) {
    DEBUG_PRINTLN("Reset tekening");
    resetTekening();
  }
  uint16_t red = analogRead(1);
  uint16_t orange = abs(analogRead(2)-1023);
  uint8_t x = red / 128;
  uint8_t y = orange / 128;
  tekening[x][y] = 1;
  for (uint8_t x=0;x<8;x++) {
    for (uint8_t y=0;y<8;y++) {
      if (tekening[x][y] == 1) {
        matrix.setPoint(x, y, true);
      }
    }
  }
}
