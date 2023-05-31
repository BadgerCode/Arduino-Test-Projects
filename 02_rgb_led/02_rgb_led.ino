//RGB LED//The RGB LED will appear red, green, and blue first, then red, orange, yellow, green, blue, indigo, and purple.
//Email:support@sunfounder.com
//Website:www.sunfounder.com
//2015.5.7

// MAKE SURE TO USE THE DIGITAL OUTPUT WITH A ~
// The other pins can't do analog signals (they're just on or off)
/*************************************************************************/
const int redPin = 11;    // R petal on RGB LED module connected to digital pin 11
const int greenPin = 10;  // G petal on RGB LED module connected to digital pin 10
const int bluePin = 9;   // B petal on RGB LED module connected to digital pin 9
/**************************************************************************/
void setup() {
  pinMode(redPin, OUTPUT);    // sets the redPin to be an output
  pinMode(greenPin, OUTPUT);  // sets the greenPin to be an output
  pinMode(bluePin, OUTPUT);   // sets the bluePin to be an output
}
/***************************************************************************/


unsigned char currentRed = 255;
unsigned char currentGreen = 0;
unsigned char currentBlue = 0;

int colourStage = 0;

int brightness = 0;  // how bright the LED is
int fadeAmount = 5;  // how many points to fade the LED by
void loop()  // run over and over again
{
  // // Basic colors:
  // color(255, 0, 0);  // turn the RGB LED red
  // delay(1000);       // delay for 1 second
  // color(0, 255, 0);  // turn the RGB LED green
  // delay(1000);       // delay for 1 second
  // color(0, 0, 255);  // turn the RGB LED blue
  // delay(1000);       // delay for 1 second
  // // Example blended colors:
  // color(255, 0, 252);  // turn the RGB LED red
  // delay(1000);         // delay for 1 second
  // color(237, 109, 0);  // turn the RGB LED orange
  // delay(1000);         // delay for 1 second
  // color(255, 215, 0);  // turn the RGB LED yellow
  // delay(1000);         // delay for 1 second
  // color(34, 139, 34);  // turn the RGB LED green
  // delay(1000);         // delay for 1 second
  // color(0, 112, 255);  // turn the RGB LED blue
  // delay(1000);         // delay for 1 second
  // color(0, 46, 90);    // turn the RGB LED indigo
  // delay(1000);         // delay for 1 second
  // color(128, 0, 128);  // turn the RGB LED purple
  // delay(1000);         // delay for 1 second


  // Rainbow
  color(currentRed, currentGreen, currentBlue);

  switch (colourStage) {
    case 0:
      currentGreen += 1;

      if (currentGreen >= 255) {
        currentGreen = 255;
        colourStage++;
      }
      break;
    case 1:
      currentRed -= 1;

      if (currentRed <= 0) {
        currentRed = 0;
        colourStage++;
      }
      break;
    case 2:
      currentBlue += 1;

      if (currentBlue >= 255) {
        currentBlue = 255;
        colourStage++;
      }
      break;
    case 3:
      currentGreen -= 1;

      if (currentGreen <= 0) {
        currentGreen = 0;
        colourStage++;
      }
      break;
    case 4:
      currentRed += 1;

      if (currentRed >= 255) {
        currentRed = 255;
        colourStage++;
      }
      break;
    case 5:
      currentBlue -= 1;

      if (currentBlue <= 0) {
        currentBlue = 0;
        colourStage++;
      }
      break;
  }

  if (colourStage > 5) {
    colourStage = 0;
  }

  delay(10);
}
/******************************************************/
void color(unsigned char red, unsigned char green, unsigned char blue)  // the color generating function
{
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}
/******************************************************/