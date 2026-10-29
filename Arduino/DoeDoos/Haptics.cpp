#include "Haptics.h"
#include "AppState.h"

namespace {
const unsigned long HIT_VIBRATION_DURATION_MS = 240;
const unsigned long GAME_OVER_VIBRATION_PHASES[] = {300, 300, 300, 300, 300};
const uint8_t GAME_OVER_VIBRATION_PHASE_COUNT =
  sizeof(GAME_OVER_VIBRATION_PHASES) / sizeof(GAME_OVER_VIBRATION_PHASES[0]);

enum class VibrationPattern : uint8_t {
  None,
  Hit,
  GameOver
};

VibrationPattern vibrationPattern = VibrationPattern::None;
uint8_t vibrationPhase = 0;
unsigned long vibrationPhaseStarted = 0;
}

void startHitVibration() {
  if (!hapticEnabled || vibrationPattern == VibrationPattern::GameOver) {
    return;
  }

  vibrationPattern = VibrationPattern::Hit;
  vibrationPhaseStarted = millis();
  analogWrite(VIBRATOR, 255);
}

void startGameOverVibration() {
  if (!hapticEnabled) {
    vibrationPattern = VibrationPattern::None;
    analogWrite(VIBRATOR, 0);
    return;
  }

  vibrationPattern = VibrationPattern::GameOver;
  vibrationPhase = 0;
  vibrationPhaseStarted = millis();
  analogWrite(VIBRATOR, 255);
}

void updateVibration() {
  if (!hapticEnabled) {
    vibrationPattern = VibrationPattern::None;
    analogWrite(VIBRATOR, 0);
    return;
  }

  if (vibrationPattern == VibrationPattern::None) {
    return;
  }

  unsigned long currentMillis = millis();
  if (vibrationPattern == VibrationPattern::Hit) {
    if (currentMillis - vibrationPhaseStarted >= HIT_VIBRATION_DURATION_MS) {
      vibrationPattern = VibrationPattern::None;
      analogWrite(VIBRATOR, 0);
    }
    return;
  }

  while (vibrationPattern == VibrationPattern::GameOver &&
         currentMillis - vibrationPhaseStarted >=
           GAME_OVER_VIBRATION_PHASES[vibrationPhase]) {
    vibrationPhaseStarted += GAME_OVER_VIBRATION_PHASES[vibrationPhase];
    vibrationPhase++;
    if (vibrationPhase == GAME_OVER_VIBRATION_PHASE_COUNT) {
      vibrationPattern = VibrationPattern::None;
      analogWrite(VIBRATOR, 0);
      return;
    }

    analogWrite(VIBRATOR, vibrationPhase % 2 == 0 ? 255 : 0);
  }
}
