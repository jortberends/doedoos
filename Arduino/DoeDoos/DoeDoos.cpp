#include <Arduino.h>
#include "AppState.h"
#include "Display.h"
#include "Games.h"
#include "Inputs.h"
#include "Nfc.h"
#include "Sketch.h"

void setup() {
  Serial.begin(115200);
  displayInit();
  setupNfc();
  setupLED();
  setupButtons();
  newRandomButtonToPress();
  setupTorn();
  setupEmergency();
  setupVibrator();
  setupFlipButtons();
}

void loop() {
  if (displayIsAnimating()) {
    Modus previousModus = modus;
    loopFlipButtons();
    loopEmergency();
    if (modus != previousModus) {
      cancelDisplayAnimation();
      matrix.clear();
    } else {
      updateDisplayAnimation();
    }
    return;
  }

  readButtons();
  readGoudenPot();
  loopBlauwePot();
  loopNFC();
  loopEmergency();
  loopTorn();
  loopFlipButtons();
  loopOrangeRedPot();
  loopCatchDrop();
  loopCatchLine();
  updateDisplayAnimation();
}
