#include "Nfc.h"
#include "AppState.h"
#include "Display.h"
#include "Debug.h"
#include <MFRC522.h>
#include <string.h>

MFRC522 mfrc522(53, -1);

uint8_t nfc[9][7] = {
  {0x04,0x1D,0x35,0xC2,0x83,0x64,0x81},
  {0x04,0x25,0x35,0xC2,0x83,0x64,0x81},
  {0x04,0x29,0x34,0xC2,0x83,0x64,0x81},
  {0x04,0x2D,0x35,0xC2,0x83,0x64,0x81},
  {0x04,0x21,0x34,0xC2,0x83,0x64,0x81},
  {0x04,0xFA,0x31,0xC2,0x83,0x64,0x80},
  {0x04,0x13,0x32,0xC2,0x83,0x64,0x81},
  {0x04,0x0B,0x32,0xC2,0x83,0x64,0x81},
  {0x04,0x04,0x31,0xC2,0x83,0x64,0x81}
  };

uint8_t lastCard[10] = {};

uint8_t loopNFCSteps = 0;

void loopNFC() {
  if (modus == catchDrop || modus == catchLine) {
    return;
  }
  loopNFCSteps++;
  if (loopNFCSteps < 15) {
    return;
  }
  loopNFCSteps = 0;
  if (!mfrc522.PICC_IsNewCardPresent()) {
    DEBUG_PRINTLN("No card present");
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    DEBUG_PRINTLN("Read error");
    return;
  }
  DEBUG_PRINTLN("Card read.");
  if (memcmp(mfrc522.uid.uidByte, lastCard, mfrc522.uid.size) == 0) {
    // Same NFC as previous
    DEBUG_PRINTLN("Same as previous");
    return;
  }
  memcpy(lastCard, mfrc522.uid.uidByte, mfrc522.uid.size);
  for (uint8_t i=0; i < (sizeof(nfc)/sizeof(nfc[0])); i++) {
    if (memcmp(lastCard, nfc[i], mfrc522.uid.size) == 0) {
      uint8_t fileIndex = i; // 0-based
      DEBUG_PRINT("Card: ");
      DEBUG_PRINTLN(fileIndex);
      printLetter(fileIndex);
      return;
    }
  }
  DEBUG_PRINTLN("Unknown NFC: ");
}


void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    DEBUG_PRINT(buffer[i] < 0x10 ? " 0" : " ");
    DEBUG_PRINT_FORMAT(buffer[i], HEX);
  }
}


void setupNfc() {
  mfrc522.PCD_Init();
}
