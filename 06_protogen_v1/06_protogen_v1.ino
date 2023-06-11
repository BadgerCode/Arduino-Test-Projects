#include "LedControl.h"


int TemperatureAnalogPin = 0;
int DistanceAnalogPin = 1;

int Brightness = 6;  // 0 - 15


int PIN_LEFT_DIN = 11;
int PIN_LEFT_CS = 3;
int PIN_LEFT_CLK = 13;
int LEFT_NUM_PANELS = 8;
LedControl LEFT_LEDs = LedControl(PIN_LEFT_DIN, PIN_LEFT_CLK, PIN_LEFT_CS, LEFT_NUM_PANELS);


int PIN_RIGHT_DIN = 7;
int PIN_RIGHT_CS = 6;
int PIN_RIGHT_CLK = 5;
int RIGHT_NUM_PANELS = 4;
LedControl RIGHT_LEDs = LedControl(PIN_RIGHT_DIN, PIN_RIGHT_CLK, PIN_RIGHT_CS, RIGHT_NUM_PANELS);



int PANEL_MOUTH_LEFT_1 = 3;
int PANEL_MOUTH_LEFT_2 = 2;
int PANEL_MOUTH_LEFT_3 = 1;
int PANEL_MOUTH_LEFT_4 = 0;
int PANEL_NOSE_LEFT = 4;
int PANEL_EYE_LEFT_1 = 6;
int PANEL_EYE_LEFT_2 = 7;





// Neutral animation
byte Nose_Normal[1][8] = {
  { B00000000, B00000000, B00000000, B00000000, B00000000, B01111000, B11000000, B10000000 }
};

byte Eye_Normal[2][8] = {
  { B00000000, B00000011, B00001111, B00111111, B01111111, B11111111, B11111110, B11100000 },
  { B00000000, B11100000, B11111000, B11111100, B11111110, B11111111, B00000000, B00000000 }
};

byte Eye_Blink[2][8] = {
  { B00000000, B00000000, B00000000, B00000111, B00001111, B00111111, B11100000, B00000000 },
  { B00000000, B00000000, B00000000, B11000000, B11100000, B11111111, B00000000, B00000000 }
};

byte Mouth_Normal[4][8] = {
  { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 },
  { B00000000, B00000000, B00000000, B10000000, B00110001, B00011111, B00000011, B00000000 },
  { B00000000, B00000000, B00000000, B00100000, B11111111, B11100111, B10000000, B00000000 },
  { B00000000, B00000000, B00000111, B01111100, B11110000, B11000000, B00000000, B00000000 }
};



// X_X animation
// // X
// byte Eye_Normal[2][8] = {
//   { B11000011, B11100111, B01111110, B00111100, B00111100, B01111110, B11100111, B11000011 },
//   { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 },
// };

// byte Eye_Blink[2][8] = {
//   { B00000000, B00000000, B00000000, B00111100, B00111100, B00000000, B00000000, B00000000 },
//   { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 },
// };

// // Flat mouth
// byte Mouth_Normal[4][8] = {
//   { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 },
//   { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 },
//   { B00000000, B00000000, B00000000, B00000000, B00000000, B11111111, B11111111, B00000000 },
//   { B00000000, B00000000, B00000000, B00000000, B00000000, B11111111, B11111111, B00000000 },
// };



// // UWU Animation
byte Mouth_UWU[4][8] = {
  { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 },
  { B00000000, B01100110, B01100110, B01100110, B01100110, B01111110, B01111110, B00000000 },
  { B00000000, B10000001, B10000001, B10000001, B01011010, B01011010, B00111100, B00000000 },
  { B00000000, B01100110, B01100110, B01100110, B01100110, B01111110, B01111110, B00000000 },
};



// Proto OS mouth (left)
// byte Mouth_Normal[4][8] = {
//   { B00000000, B00000000, B00000000, B00000001, B00000111, B00011110, B01111000, B11100000 },
//   { B00000000, B00000000, B00000000, B00000000, B11100000, B01111000, B00011110, B00000111 },
//   { B00000000, B00000000, B00000000, B00000111, B00011111, B01111000, B11100000, B10000000 },
//   { B00001000, B00011110, B01111011, B11100011, B11111111, B00000000, B00000000, B00000000 },
// };


byte Numbers_ASCII[10][8] = {
  { B00000000, B00111100, B01100110, B01101110, B01110110, B01100110, B01100110, B00111100 },
  { B00000000, B00011000, B00011000, B00111000, B00011000, B00011000, B00011000, B01111110 },
  { B00000000, B00111100, B01100110, B00000110, B00001100, B00110000, B01100000, B01111110 },
  { B00000000, B00111100, B01100110, B00000110, B00011100, B00000110, B01100110, B00111100 },
  { B00000000, B00001100, B00011100, B00101100, B01001100, B01111110, B00001100, B00001100 },
  { B00000000, B01111110, B01100000, B01111100, B00000110, B00000110, B01100110, B00111100 },
  { B00000000, B00111100, B01100110, B01100000, B01111100, B01100110, B01100110, B00111100 },
  { B00000000, B01111110, B01100110, B00001100, B00001100, B00011000, B00011000, B00011000 },
  { B00000000, B00111100, B01100110, B01100110, B00111100, B01100110, B01100110, B00111100 },
  { B00000000, B00111100, B01100110, B01100110, B00111110, B00000110, B01100110, B00111100 },
};





void setup() {
  // Serial.begin(9600); // Enable serial port
  // used for things like Serial.println("blah blah");

  pinMode(PIN_LEFT_CS, OUTPUT);
  pinMode(PIN_RIGHT_CS, OUTPUT);

  for (int address = 0; address < LEFT_NUM_PANELS; address++) {
    LEFT_LEDs.shutdown(address, false);           // Disable power saving
    LEFT_LEDs.setIntensity(address, Brightness);  // Set brightness 0-15
    LEFT_LEDs.clearDisplay(address);              // Turn all LEDs off
  }

  for (int address = 0; address < RIGHT_NUM_PANELS; address++) {
    RIGHT_LEDs.shutdown(address, false);           // Disable power saving
    RIGHT_LEDs.setIntensity(address, Brightness);  // Set brightness 0-15
    RIGHT_LEDs.clearDisplay(address);              // Turn all LEDs off
  }
}


int Face_OffsetX = 0;  // OffsetX doesn't work, because the LEDs from one panel don't wrap to the next panel
int Face_OffsetY = 0;
int Face_OffsetY_Dir = 1;
int OffsetDelay = 200;
unsigned long NextOffsetShift = millis() + OffsetDelay;

int MinBlinkWait = 5000;
unsigned long NextBlink = millis() + random(3000) + MinBlinkWait;
int BlinkDurationMs = 50;

unsigned long NextTempReading = millis() + 500;


unsigned long NextDistanceReading = millis() + 200;

void loop() {
  unsigned long curTime = millis();

  // Temperature test
  if (curTime >= NextTempReading) {
    float temperature = getTemperature();
    int digitOneIndex = temperature / 10;
    int digitTwoIndex = (int)temperature % 10;
    renderPanel(RIGHT_LEDs, 0, Numbers_ASCII[digitTwoIndex], false, false, 0, 0);
    renderPanel(RIGHT_LEDs, 1, Numbers_ASCII[digitOneIndex], false, false, 0, 0);

    NextTempReading = millis() + 500;
  }


  // Touch sensor
  int distance = getDistance();

  // Mouth
  if (distance < 300) {
    renderPanel(LEFT_LEDs, PANEL_MOUTH_LEFT_1, Mouth_Normal[0], false, false, Face_OffsetX, Face_OffsetY);
    renderPanel(LEFT_LEDs, PANEL_MOUTH_LEFT_2, Mouth_Normal[1], false, false, Face_OffsetX, Face_OffsetY);
    renderPanel(LEFT_LEDs, PANEL_MOUTH_LEFT_3, Mouth_Normal[2], false, false, Face_OffsetX, Face_OffsetY);
    renderPanel(LEFT_LEDs, PANEL_MOUTH_LEFT_4, Mouth_Normal[3], false, false, Face_OffsetX, Face_OffsetY);
  }
  else {
    renderPanel(LEFT_LEDs, PANEL_MOUTH_LEFT_1, Mouth_UWU[0], false, false, Face_OffsetX, Face_OffsetY);
    renderPanel(LEFT_LEDs, PANEL_MOUTH_LEFT_2, Mouth_UWU[1], false, false, Face_OffsetX, Face_OffsetY);
    renderPanel(LEFT_LEDs, PANEL_MOUTH_LEFT_3, Mouth_UWU[2], false, false, Face_OffsetX, Face_OffsetY);
    renderPanel(LEFT_LEDs, PANEL_MOUTH_LEFT_4, Mouth_UWU[3], false, false, Face_OffsetX, Face_OffsetY);
  }


  // Nose
  // Reversed rows & columns (the panel is rotated 180 degrees currently)
  renderPanel(LEFT_LEDs, PANEL_NOSE_LEFT, Nose_Normal[0], true, false, 0, 0);


  // Eyes
  if (curTime < NextBlink) {
    // Reversed rows & columns (the panel is rotated 180 degrees currently)
    renderPanel(LEFT_LEDs, PANEL_EYE_LEFT_1, Eye_Normal[0], true, false, Face_OffsetX, Face_OffsetY);
    renderPanel(LEFT_LEDs, PANEL_EYE_LEFT_2, Eye_Normal[1], true, false, Face_OffsetX, Face_OffsetY);
  } else {
    // Reversed rows & columns (the panel is rotated 180 degrees currently)
    renderPanel(LEFT_LEDs, PANEL_EYE_LEFT_1, Eye_Blink[0], true, false, Face_OffsetX, Face_OffsetY);
    renderPanel(LEFT_LEDs, PANEL_EYE_LEFT_2, Eye_Blink[1], true, false, Face_OffsetX, Face_OffsetY);

    if (curTime >= NextBlink + BlinkDurationMs) {
      NextBlink = millis() + random(3000) + MinBlinkWait;
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





void renderPanel(LedControl faceLEDs, int panelIndex, byte data[], bool isReversed, bool flipRowsAndColumns, int offsetX, int offsetY) {
  for (int row = 0; row < 8; row++) {
    int rowIndex = row + offsetY;
    if (rowIndex < 0 || rowIndex >= 8) {
      if (flipRowsAndColumns == false) {
        faceLEDs.setRow(panelIndex, row, 0);
      } else {
        faceLEDs.setColumn(panelIndex, row, 0);
      }
      continue;
    }

    byte columnData = 0;
    if (isReversed) {
      columnData = reverse(data[7 - rowIndex] << offsetX);
    } else {
      columnData = data[rowIndex] << offsetX;
    }

    if (flipRowsAndColumns == false) {
      faceLEDs.setRow(panelIndex, row, columnData);
    } else {
      faceLEDs.setColumn(panelIndex, row, columnData);
    }
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

int getDistance() {
  return analogRead(DistanceAnalogPin);
}
