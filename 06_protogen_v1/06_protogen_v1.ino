#include "LedControl.h"


int ButtonPin = 0;
int TemperatureAnalogPin = 0;
int DistanceAnalogPin = 1;

int Brightness = 6;  // 0 - 15


// Left face
int PIN_LEFT_DIN = 11;
int PIN_LEFT_CS = 3;
int PIN_LEFT_CLK = 13;
int LEFT_NUM_PANELS = 7;

LedControl LEFT_LEDs = LedControl(PIN_LEFT_DIN, PIN_LEFT_CLK, PIN_LEFT_CS, LEFT_NUM_PANELS);



// Right face
int PIN_RIGHT_DIN = 7;
int PIN_RIGHT_CS = 6;
int PIN_RIGHT_CLK = 5;
int RIGHT_NUM_PANELS = 7;

LedControl RIGHT_LEDs = LedControl(PIN_RIGHT_DIN, PIN_RIGHT_CLK, PIN_RIGHT_CS, RIGHT_NUM_PANELS);



// Panel indexes (left and right have the same indexes)
int PANEL_MOUTH1 = 3;
int PANEL_MOUTH2 = 2;
int PANEL_MOUTH3 = 1;
int PANEL_MOUTH4 = 0;
int PANEL_NOSE = 4;
int PANEL_EYE1 = 5;
int PANEL_EYE2 = 6;



typedef byte EyeFrame[2][8];

struct FaceExpression {
  byte Nose[1][8];
  EyeFrame Eye;
  EyeFrame Eye_Blink;
  byte Mouth[4][8];
};


// Expressions
enum Expression {
  START,
  Neutral,
  X_X,
  END
};

// TODO: Reduce memory by re-using parts of other expressions
struct FaceExpression Face_Neutral = {
  .Nose = {
    { B01111000, B11000000, B10000000, B00000000, B00000000, B00000000, B00000000, B00000000 }  //
  },
  .Eye = {
    { B00000000, B00000011, B00001111, B00111111, B01111111, B11111111, B11111110, B11100000 },  //
    { B00000000, B11100000, B11111000, B11111100, B11111110, B11111111, B00000000, B00000000 }   //
  },
  .Eye_Blink = {
    { B00000000, B00000000, B00000000, B00000111, B00001111, B00111111, B11100000, B00000000 },  //
    { B00000000, B00000000, B00000000, B11000000, B11100000, B11111111, B00000000, B00000000 }   //
  },
  .Mouth = {
    { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 },  //
    { B00000000, B00000000, B00000000, B10000000, B00110001, B00011111, B00000011, B00000000 },  //
    { B00000000, B00000000, B00000000, B00100000, B11111111, B11100111, B10000000, B00000000 },  //
    { B00000000, B00000000, B00000111, B01111100, B11110000, B11000000, B00000000, B00000000 }   //
  }
};

struct FaceExpression Face_X_X = {
  .Nose = {
    { B01111000, B11000000, B10000000, B00000000, B00000000, B00000000, B00000000, B00000000 }  //
  },
  .Eye = {
    { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 },  //
    { B11000011, B11100111, B01111110, B00111100, B00111100, B01111110, B11100111, B11000011 },  //
  },
  .Eye_Blink = {
    { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 },  //
    { B00000000, B00000000, B00000000, B00111100, B00111100, B00000000, B00000000, B00000000 },  //
  },
  .Mouth = {
    { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 },  //
    { B00000000, B00000000, B00000000, B00000000, B00000000, B11111111, B11111111, B00000000 },  //
    { B00000000, B00000000, B00000000, B00000000, B00000000, B11111111, B11111111, B00000000 },  //
    { B00000000, B00000000, B00000000, B00000000, B00000000, B11111111, B11111111, B00000000 },  //
  }
};


// Override expression
struct FaceExpression Face_UWU = {
  .Nose = {
    { B01111000, B11000000, B10000000, B00000000, B00000000, B00000000, B00000000, B00000000 }  //
  },
  .Eye = {
    { B00000000, B00000011, B00001111, B00111111, B01111111, B11111111, B11111110, B11100000 },  //
    { B00000000, B11100000, B11111000, B11111100, B11111110, B11111111, B00000000, B00000000 }   //
  },
  .Eye_Blink = {
    { B00000000, B00000000, B00000000, B00000111, B00001111, B00111111, B11100000, B00000000 },  //
    { B00000000, B00000000, B00000000, B11000000, B11100000, B11111111, B00000000, B00000000 }   //
  },
  .Mouth = {
    { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 },  //
    { B00000000, B01100110, B01100110, B01100110, B01100110, B01111110, B01111110, B00000000 },  //
    { B00000000, B10000001, B10000001, B10000001, B01011010, B01011010, B00111100, B00000000 },  //
    { B00000000, B01100110, B01100110, B01100110, B01100110, B01111110, B01111110, B00000000 }   //
  }
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
  // Serial.begin(9600);  // Enable serial port
  // used for things like Serial.println("blah blah");

  pinMode(ButtonPin, INPUT);
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


enum Expression CurrentExpression = Neutral;


int Face_OffsetX = 0;  // OffsetX doesn't work, because the LEDs from one panel don't wrap to the next panel
int Face_OffsetY = 0;
int Face_OffsetY_Dir = 1;
int OffsetDelay = 200;
unsigned long NextOffsetShift = millis() + OffsetDelay;

int MinBlinkWait = 2000;
unsigned long NextBlink = millis() + random(100) + MinBlinkWait;
int BlinkDurationMs = 100;

bool ButtonIsDown = false;
int ButtonCheckDelayMs = 10;
unsigned long NextButtonCheck = millis() + ButtonCheckDelayMs;


unsigned long NextPrint = millis() + 100;


void loop() {
  unsigned long curTime = millis();

  // Check if it's time to stop blinking
  if (curTime >= NextBlink + BlinkDurationMs) {
    NextBlink = millis() + random(100) + MinBlinkWait;
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

  // Check button to change expression
  bool buttonHasBeenPressed = isButtonDown();
  if (buttonHasBeenPressed && ButtonIsDown == false) {
    // Button has just been pressed this tick
    CurrentExpression = CurrentExpression + 1;
    if(CurrentExpression == END) {
      CurrentExpression = START + 1;
    }
  }
  ButtonIsDown = buttonHasBeenPressed;


  // Touch sensor
  int distance = getDistance();
  bool touchNearby = (distance < 750);


  // Determine expression
  struct FaceExpression* facialExpression = &Face_Neutral;

  switch (CurrentExpression) {
    case X_X:
      facialExpression = &Face_X_X;
      break;
    case Neutral:
    default:
      facialExpression = &Face_Neutral;
      break;
  }

  if (touchNearby) {
    // Allow touch sensor to override expression
    facialExpression = &Face_UWU;
  }


  // Mouth
  renderLeftAndRightPanel(PANEL_MOUTH1, (*facialExpression).Mouth[0], false, Face_OffsetX, Face_OffsetY);
  renderLeftAndRightPanel(PANEL_MOUTH2, (*facialExpression).Mouth[1], false, Face_OffsetX, Face_OffsetY);
  renderLeftAndRightPanel(PANEL_MOUTH3, (*facialExpression).Mouth[2], false, Face_OffsetX, Face_OffsetY);
  renderLeftAndRightPanel(PANEL_MOUTH4, (*facialExpression).Mouth[3], false, Face_OffsetX, Face_OffsetY);

  // Nose
  renderLeftAndRightPanel(PANEL_NOSE, (*facialExpression).Nose[0], false, 0, 0);


  // Eyes
  bool shouldBlink = (curTime >= NextBlink);
  EyeFrame* eyes = shouldBlink ? &((*facialExpression).Eye_Blink) : &((*facialExpression).Eye);
  renderLeftAndRightPanel(PANEL_EYE1, (*eyes)[0], true, Face_OffsetX, Face_OffsetY);
  renderLeftAndRightPanel(PANEL_EYE2, (*eyes)[1], true, Face_OffsetX, Face_OffsetY);




  // Debug print code
  // Make sure to enable Serial.begin(9600); in setup
  // if (curTime >= NextPrint) {
  //   NextPrint = millis() + 20;
  //   Serial.println((curTime >= NextBlink));
  // }
}



void renderLeftAndRightPanel(int panelIndex, byte data[], bool isReversed, int offsetX, int offsetY) {
  renderPanel(LEFT_LEDs, panelIndex, data, isReversed, isReversed, offsetX, offsetY);
  renderPanel(RIGHT_LEDs, panelIndex, data, isReversed, !isReversed, offsetX, offsetY * -1);
}


void renderPanel(LedControl faceLEDs, int panelIndex, byte data[], bool isReversed, bool isUpsideDown, int offsetX, int offsetY) {
  for (int row = 0; row < 8; row++) {
    int rowIndex = row + offsetY;
    if (rowIndex < 0 || rowIndex >= 8) {
      faceLEDs.setRow(panelIndex, row, 0);
      continue;
    }

    byte rowData = 0;
    int rowDataIndex = isUpsideDown ? (7 - rowIndex) : rowIndex;
    if (isReversed) {
      rowData = reverse(data[rowDataIndex] << offsetX);
    } else {
      rowData = data[rowDataIndex] << offsetX;
    }

    faceLEDs.setRow(panelIndex, row, rowData);
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
  return 1023 - analogRead(DistanceAnalogPin);  // Higher value = further away
}

bool isButtonDown() {
  return digitalRead(ButtonPin) == 0;  // Invert it, so that it's 0 when off and 1 when on
}