#include <Arduino.h>
#include "AppState.h"
#include "Display.h"
#include "Games.h"
#include "Inputs.h"
#include "Nfc.h"
#include "Sketch.h"

void setup() {
  Serial.begin(115200);
  displayController.begin();
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
  if (displayController.isAnimating()) {
    Modus previousModus = modus;
    loopFlipButtons();
    loopTorn();
    loopEmergency();
    if (modus != previousModus) {
      displayController.cancelAnimation();
      displayController.clear();
    } else {
      displayController.updateAnimation();
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
  catchDropGame.update();
  catchLineGame.update();
  spaceInvadersGame.update();
  displayController.updateAnimation();
}
