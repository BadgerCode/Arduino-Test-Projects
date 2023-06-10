#include "LedControl.h"

int DIN = 11;
int CS = 3;
int CLK = 13;
int numPanels = 8;

LedControl lc = LedControl(DIN, CLK, CS, numPanels);


void setup() {
  pinMode(CS, OUTPUT);

  for (int address = 0; address < numPanels; address++) {
    lc.shutdown(address, false);   // Disable power saving
    lc.setIntensity(address, 15);  // Set brightness 0-15
    lc.clearDisplay(address);      // Turn all LEDs off
  }
}

bool patternA = true;

unsigned long nextFlipPattern = millis() + 1000;

void loop() {
  for (int address = 0; address < numPanels; address++) {
    for (int row = 0; row < 8; row++) {
      if (patternA)
        lc.setRow(address, row, B01010101);
      else
        lc.setRow(address, row, B10101010);
    }
  }

  if (millis() >= nextFlipPattern) {
    patternA = !patternA;
    nextFlipPattern = millis() + 1000;
  }
}