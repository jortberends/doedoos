#pragma once

#include <Arduino.h>

#define DEBUG_ENABLED 0
#if DEBUG_ENABLED
  #define DEBUG_PRINT(value) Serial.print(value)
  #define DEBUG_PRINTLN(value) Serial.println(value)
  #define DEBUG_PRINT_NEWLINE() Serial.println()
  #define DEBUG_PRINT_FORMAT(value, format) Serial.print(value, format)
#else
  #define DEBUG_PRINT(value)
  #define DEBUG_PRINTLN(value)
  #define DEBUG_PRINT_NEWLINE()
  #define DEBUG_PRINT_FORMAT(value, format)
#endif
