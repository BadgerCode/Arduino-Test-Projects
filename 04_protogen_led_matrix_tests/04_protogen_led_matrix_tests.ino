//Dot-Matrix Display
//The dot-matrix will display 0 to F circularly
//Email:support@sunfounder.com
//Website:www.sunfounder.com
//2015.5.7
const int latchPin = 8;   //Pin connected to ST_CP of 74HC595
const int clockPin = 12;  //Pin connected to SH_CP of 74HC595
const int dataPin = 11;   //Pin connected to DS of 74HC595



const int numVisibleColumns = 8;

int eye_normal[] = { B11111111, B11111111, B11111111, B01111111, B01111111, B00111111, B00011111, B00000111 };

const int blinkFramesCount = 9;
int blinkFrames[blinkFramesCount][numVisibleColumns] = {
  { B11111111, B11111111, B11111111, B01111111, B01111111, B00111111, B00011111, B00000111 },
  { B00000000, B11111111, B11111111, B01111111, B01111111, B00111111, B00011111, B00000111 },
  { B00000000, B00000000, B11111111, B01111111, B01111111, B00111111, B00011111, B00000111 },
  { B00000000, B00000000, B00000000, B01111111, B01111111, B00111111, B00011111, B00000111 },
  { B00000000, B00000000, B00000000, B00000000, B01111111, B00111111, B00011111, B00000111 },
  { B00000000, B00000000, B00000000, B00000000, B00000000, B00111111, B00011111, B00000111 },
  { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00011111, B00000111 },
  { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000111 },
  { B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000 },
};

const int expressionFramesCount = 6;
int expressionFrames[expressionFramesCount][numVisibleColumns] = {
  { B00000110, B00001001, B00000000, B00000000, B11000000, B00000011, B00011100, B01100000 },  // Proto face
  { B00000001, B00000111, B00001110, B00000000, B11000000, B00001100, B00110011, B01000000 },  // Proto face angry
  { B00001000, B00001110, B00000111, B00000000, B11000000, B00001100, B00110011, B01000000 },  // Proto face sad
  { B00000010, B00000010, B00000010, B00000000, B11000000, B00001110, B00111111, B01100000 },  // Proto face surprised
  { B00000010, B00000111, B00000111, B00000010, B11000000, B00000011, B00011100, B01100000 },  // Proto face surprised 2
  { B00001010, B00011111, B00001110, B00000100, B11000000, B00001110, B00111100, B01100000 },  // Proto face heart
  // { B11000000, B11000000, B11100000, B01110000, B01111000, B00111100, B00011111, B00000111 },  // Curve
  // { B11111111, B11111111, B11111111, B01111111, B01111111, B00111111, B00011111, B00000111 },  // Standard eye left piece
  // { B10110111, B11011011, B11011110, B01111111, B01101011, B00111101, B10011110, B00100111 },  // Standard eye left piece glitch
  // { B11111111, B11111111, B11111111, B01111111, B01111111, B00111111, B00011111, B00000111 },  // Standard eye left piece
  // { B00011000, B00111100, B00111100, B01111110, B01111110, B00111100, B00111100, B00011000 },  // Oval eye open
  // { B00000000, B00000000, B00111100, B01111110, B01111110, B00111100, B00000000, B00000000 },  // Oval eye closed
  // { B00011000, B00111100, B00111100, B01111110, B01111110, B00111100, B00111100, B00011000 },  // Oval eye open
  // { B01000110, B11100111, B11111111, B11111111, B11111111, B01111110, B00111100, B00011000 },  // Heart
};




void setup() {
  //set pins to output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

  randomSeed(analogRead(0));
}





// State
int *currentFrame = expressionFrames[0];
unsigned long nextFrameMs = 0;
int animationDelayMs = 500;  // 16 = 60FPS

bool isBlinking = false;
int blinkDelayMs = 5000;
int blinkFrameIndex = 0;
unsigned long nextBlinkMs = millis() + blinkDelayMs;

int expressionFrameIndex = 0;
unsigned long nextExpressionMs = millis() + random(7000) + 3000;


void loop() {
  // Don't use delays, or this won't render properly
  writeFrame(currentFrame);

  unsigned long currentMs = millis();

  // Perform animations
  if (currentMs >= nextFrameMs) {
    nextFrameMs = currentMs + animationDelayMs;

    // Expressions test
    if (currentMs >= nextExpressionMs) {
      if (expressionFrameIndex == 0) {
        // Apply a new random expression
        expressionFrameIndex = random(expressionFramesCount - 1) + 1;
        currentFrame = expressionFrames[expressionFrameIndex];
      } else if (currentMs >= (nextExpressionMs + 1000)) {
        // Reset to neutral expression
        expressionFrameIndex = 0;
        nextExpressionMs = millis() + random(7000) + 3000;
        currentFrame = expressionFrames[expressionFrameIndex];
      }
    }


    // Blinking
    // if (nextBlinkMs <= currentMs) {
    //   isBlinking = true;
    //   nextBlinkMs = millis() + blinkDelayMs;
    //   blinkFrameIndex = 0;
    // }

    // if (isBlinking) {
    //   if (blinkFrameIndex >= blinkFramesCount) {
    //     isBlinking = false;
    //     currentFrame = eye_normal;
    //   } else {
    //     currentFrame = blinkFrames[blinkFrameIndex];
    //     blinkFrameIndex++;
    //   }
    // }
    // End of blinking
  }
}


void writeFrame(int columnsToWrite[]) {
  int matrixColumn = 0x01;

  for (int columnIndex = 0; columnIndex < numVisibleColumns; columnIndex++) {
    shiftOut(dataPin, clockPin, MSBFIRST, columnsToWrite[columnIndex]);  //control ROW of dot matrix
    shiftOut(dataPin, clockPin, LSBFIRST, ~matrixColumn);                //control COL of dot matrix
    //return the latch pin high to signal chip that it
    //no longer needs to listen for information
    digitalWrite(latchPin, HIGH);      //pull the latchPin to save the data
    digitalWrite(latchPin, LOW);       //ground latchPin and hold low for as long as you are transmitting
    matrixColumn = matrixColumn << 1;  // Move to next matrix column
    delay(1);                          //wait for a microsecond
  }
}