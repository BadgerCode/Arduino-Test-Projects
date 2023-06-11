#include "LedControl.h"


int TemperatureAnalogPin = 0;



int DIN = 11;
int CS = 3;
int CLK = 13;
int numPanels = 8;

LedControl lc = LedControl(DIN, CLK, CS, numPanels);


int PANEL_MOUTH_LEFT_1 = 3;
int PANEL_MOUTH_LEFT_2 = 2;
int PANEL_MOUTH_LEFT_3 = 1;
int PANEL_MOUTH_LEFT_4 = 0;
int PANEL_NOSE_LEFT = 4;
int PANEL_EYE_LEFT_1 = 6;
int PANEL_EYE_LEFT_2 = 7;






byte Nose_Normal[1][8] = {
  { B00000000, B00000000, B00000000, B00000000, B00000000, B01111000, B11000000, B10000000 }
};

byte Eye_Normal[2][8] = {
  { B00000000, B00000011, B00001111, B00111111, B01111111, B11111111, B11111110, B11100000 },
  { B00000000, B11100000, B11111000, B11111100, B11111110, B11111111, B00000000, B00000000 }
};

byte Mouth_Normal[4][8] = {
  { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 },
  { B00000000, B00000000, B00000000, B10000000, B00110001, B00011111, B00000011, B00000000 },
  { B00000000, B00000000, B00000000, B00100000, B11111111, B11100111, B10000000, B00000000 },
  { B00000000, B00000000, B00000111, B01111100, B11110000, B11000000, B00000000, B00000000 }
};


byte Eye_Blink[2][8] = {
  { B00000000, B00000000, B00000000, B00000111, B00001111, B00111111, B11100000, B00000000 },
  { B00000000, B00000000, B00000000, B11000000, B11100000, B11111111, B00000000, B00000000 }
};




void setup() {
  pinMode(CS, OUTPUT);

  for (int address = 0; address < numPanels; address++) {
    lc.shutdown(address, false);   // Disable power saving
    lc.setIntensity(address, 15);  // Set brightness 0-15
    lc.clearDisplay(address);      // Turn all LEDs off
  }
}


int Face_OffsetX = 0;  // OffsetX doesn't work, because the LEDs from one panel don't wrap to the next panel
int Face_OffsetY = 0;
int Face_OffsetY_Dir = 1;
int OffsetDelay = 200;
unsigned long NextOffsetShift = millis() + OffsetDelay;

int MinBlinkDuration = 5000;
unsigned long NextBlink = millis() + random(3000) + MinBlinkDuration;
int BlinkDurationMs = 50;


void loop() {
  unsigned long curTime = millis();


  // Mouth
  renderPanel(PANEL_MOUTH_LEFT_1, Mouth_Normal[0], false, Face_OffsetX, Face_OffsetY);
  renderPanel(PANEL_MOUTH_LEFT_2, Mouth_Normal[1], false, Face_OffsetX, Face_OffsetY);
  renderPanel(PANEL_MOUTH_LEFT_3, Mouth_Normal[2], false, Face_OffsetX, Face_OffsetY);
  renderPanel(PANEL_MOUTH_LEFT_4, Mouth_Normal[3], false, Face_OffsetX, Face_OffsetY);


  // Nose
  // Reversed rows & columns (the panel is rotated 180 degrees currently)

  renderPanel(PANEL_NOSE_LEFT, Nose_Normal[0], true, 0, 0);

  // Eyes
  if (curTime < NextBlink) {
    // Reversed rows & columns (the panel is rotated 180 degrees currently)
    renderPanel(PANEL_EYE_LEFT_1, Eye_Normal[0], true, Face_OffsetX, Face_OffsetY);
    renderPanel(PANEL_EYE_LEFT_2, Eye_Normal[1], true, Face_OffsetX, Face_OffsetY);
  } else {
    // Reversed rows & columns (the panel is rotated 180 degrees currently)
    renderPanel(PANEL_EYE_LEFT_1, Eye_Blink[0], true, Face_OffsetX, Face_OffsetY);
    renderPanel(PANEL_EYE_LEFT_2, Eye_Blink[1], true, Face_OffsetX, Face_OffsetY);

    if (curTime >= NextBlink + BlinkDurationMs) {
      NextBlink = millis() + random(3000) + MinBlinkDuration;
    }
  }


  // Offset the face to do a basic animation
  if (curTime >= NextOffsetShift) {
    NextOffsetShift = curTime + OffsetDelay;

    Face_OffsetY += Face_OffsetY_Dir;

    if (Face_OffsetY >= 1) {
      Face_OffsetY_Dir = -1;
    } else if (Face_OffsetY <= -1) {
      Face_OffsetY_Dir = 1;
    }
  }
}





void renderPanel(int panelIndex, byte data[], bool isReversed, int offsetX, int offsetY) {
  for (int row = 0; row < 8; row++) {
    int rowIndex = row + offsetY;
    if (rowIndex < 0 || rowIndex >= 8) {
      lc.setRow(panelIndex, row, 0);
      continue;
    }

    byte columnData = 0;
    if (isReversed) {
      columnData = reverse(data[7 - rowIndex] << offsetX);
    } else {
      columnData = data[rowIndex] << offsetX;
    }

    lc.setRow(panelIndex, row, columnData);
  }
}

byte reverse(byte b) {
  b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
  b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
  b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
  return b;
}


/// Returns a two decimal number for the temperature in celcius
float getTemperature() {
  float voltage = analogRead(TemperatureAnalogPin) * 0.004882814;
  return (voltage - 0.5) * 100;
}
