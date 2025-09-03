#include "MAX7219Control.h"

#define FACE_DATAPIN 12
#define FACE_CSPIN 11
#define FACE_CLKPIN 10
#define FACE_NUMPANELS 12


MAX7219Control* LedController;

void setup() {
  // put your setup code here, to run once:

  LedController = new MAX7219Control(FACE_DATAPIN, FACE_CSPIN, FACE_CLKPIN, FACE_NUMPANELS);
  LedController->Initialise();
}

void loop() {
  // put your main code here, to run repeatedly:

  SimpleExample();
  // FaceExample();
}


void SimpleExample() {
  // Renders a diagonal line across each panel

  for (int panel = 0; panel < FACE_NUMPANELS; panel++) {
    LedController->SetRow(panel, 0, B10000000);
    LedController->SetRow(panel, 1, B01000000);
    LedController->SetRow(panel, 2, B00100000);
    LedController->SetRow(panel, 3, B00010000);
    LedController->SetRow(panel, 4, B00001000);
    LedController->SetRow(panel, 5, B00000100);
    LedController->SetRow(panel, 6, B00000010);
    LedController->SetRow(panel, 7, B00000001);
  }

  LedController->RenderDisplays();
}


byte Protogen[8] = {
  B00000110,
  B00001001,
  B00000000,
  B00000000,
  B11000000,
  B00000011,
  B00011100,
  B01100000,
};

void FaceExample() {
  // Renders a protogen face and makes it move up and down

  unsigned long curTime = millis();
  int offset = (curTime / 1000) % 2 == 0 ? -1 : 0;  // Bump the face down on even seconds- 2, 4, 6, 8

  for (int panel = 0; panel < FACE_NUMPANELS; panel++) {
    for (int row = 0; row < 8; row++) {
      int offsetRow = row + offset;

      // If the offset pushes the row outside the array, just show an empty row
      if (offsetRow < 0 || offsetRow > 7) {
        LedController->SetRow(panel, row, 0);
      } else {
        // Show the corresponding row from the protogen face
        LedController->SetRow(panel, row, Protogen[offsetRow]);
      }
    }
  }

  LedController->RenderDisplays();
}
