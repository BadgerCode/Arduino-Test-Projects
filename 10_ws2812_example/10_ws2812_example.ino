#include <FastLED.h>

#define DATAPIN 9
#define NUMPANELS 4

int NumLEDs = NUMPANELS * 64;
CRGB* LED_Data;
int Brightness = 125;  // 0-255

void setup() {
  // put your setup code here, to run once:

  LED_Data = new CRGB[NumLEDs];
  FastLED.addLeds<NEOPIXEL, DATAPIN>(LED_Data, NumLEDs);
}

void loop() {
  // put your main code here, to run repeatedly:
  // SimpleExample();
  FaceExample();
}


void SimpleExample() {
  // Cycles through all LEDs, one at a time, changing colour slightly for each one

  // Reset LEDs
  for (int i = 0; i < NumLEDs; i++)
    LED_Data[i] = CRGB::Black;

  // Set data
  int index = ((millis() / 100) % NumLEDs);
  LED_Data[index] = CHSV((20 * index) % 255, 255, Brightness);

  // Render LEDs
  FastLED.show();
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

byte Eyes[2][8] = {
  { B00000000, B00000011, B00001111, B00111111, B01111111, B11111111, B11111110, B11100000 },  //
  { B00000000, B11100000, B11111000, B11111100, B11111110, B11111111, B00000000, B00000000 }
};


void FaceExample() {
  // Renders a protogen face

  // Set data
  int hue = ((millis() / 50) * 15) % 255;

  for (int i = 0; i < NumLEDs; i++) {
    int row = (i / 8) % 8;
    int col = i % 8;
    int eye = 1 - ((i / 64) % 2);
    bool enabled = (Eyes[eye][row] & (1 << (7 - col))) > 0;

    if (enabled)
      LED_Data[i] = CHSV(hue, 255, Brightness);
    else
      LED_Data[i] = CRGB::Black;
  }

  // Render LEDs
  FastLED.show();
}
