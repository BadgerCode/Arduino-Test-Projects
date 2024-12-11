#include <FastLED.h>

#define DATA_PIN 7
#define NUM_LEDS 15

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  // FastLED.setBrightness(255);
}



// 0. Single colour
void loop() {
  // Set all LEDs to blue
  fill_solid(leds, NUM_LEDS, CRGB(0, 0, 255));
  FastLED.show();
  delay(5000);
}



// 1. Simple alternating colours loop
// void loop() {
//   // Set all LEDs to red
//   fill_solid(leds, NUM_LEDS, CRGB::Red);
//   FastLED.show();
//   delay(5000);

//   // Set all LEDs to green
//   fill_solid(leds, NUM_LEDS, CRGB::Green);
//   FastLED.show();
//   delay(5000);

//   // Set all LEDs to blue
//   fill_solid(leds, NUM_LEDS, CRGB::Blue);
//   FastLED.show();
//   delay(5000);

//   fill_solid(leds, NUM_LEDS, CRGB::White);
//   FastLED.show();
//   delay(5000);
// }



// 2. Simple RGB hue rotation (all same colour)
// uint8_t hue = 0;
// void loop() {
//   fill_solid(leds, NUM_LEDS, CHSV(hue++, 255, 255));
//   FastLED.show();
//   delay(20);
// }


// 3. Spinning hue rotation
// uint8_t hue = 0;
// void loop() {
//   for (int i = 0; i < NUM_LEDS; i++) {
//     leds[i] = CHSV(hue, 255, 255);
//     hue += 15;
//   }

//   FastLED.show();
//   delay(40);
// }
