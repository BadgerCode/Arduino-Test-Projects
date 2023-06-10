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

int Cat[8] = { B10001000, B11111000, B10101000, B01110001, B00100001, B01111001, B01111101, B10111110 };

void loop() {
  // Render cat
  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, Cat[i]);
  }

  // Skip address 0
  for (int address = 1; address < numPanels; address++) {
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