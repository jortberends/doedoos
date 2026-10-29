#pragma once

#include <Arduino.h>
#include <MD_MAX72xx.h>

extern MD_MAX72XX matrix;

void displayInit();
void loopIcons();
void printLetter(uint8_t index);
void nextIcon();
void printIcon(uint8_t index);
void mirrorIcon();
void updateDisplayAnimation();
void displayGameOver(uint16_t points);
void displayScore(uint16_t points);
bool displayIsAnimating();
void cancelDisplayAnimation();
