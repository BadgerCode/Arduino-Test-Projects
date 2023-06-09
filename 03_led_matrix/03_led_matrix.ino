//Dot-Matrix Display
//The dot-matrix will display 0 to F circularly
//Email:support@sunfounder.com
//Website:www.sunfounder.com
//2015.5.7
const int latchPin = 8;   //Pin connected to ST_CP of 74HC595
const int clockPin = 12;  //Pin connected to SH_CP of 74HC595
const int dataPin = 11;   //Pin connected to DS of 74HC595



/*
Letters

B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000,B00000000, // Empty
B00000000,B01111111,B01111111,B01001001,B01001001,B01111111,B00110110,B00000000, // B (Bold)
B00000000,B00011111,B00111111,B01100100,B01100100,B00111111,B00011111,B00000000, // A (Bold)
B00000000,B01111111,B01111111,B01000001,B01100011,B00111110,B00011100,B00000000, // D (Bold)
B00011100,B00111110,B01100011,B01000001,B01000101,B01100111,B00100111,B00000000, // G (Bold)
B00000000,B01111111,B01111111,B01001001,B01011101,B01000001,B01100011,B00000000, // E (Bold)
B00000000,B01111111,B01111111,B01001000,B01001100,B01111111,B00110011,B00000000, // R (Bold)
More letters here, but they're the wrong way around
https://gist.github.com/rothwerx/700f275d078b3483509f

Negated letters
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // Empty
0xFF, 0xC1, 0xBE, 0xBE, 0xBE, 0xC1, 0xFF, 0xFF, // 0
0xFF, 0xDF, 0xDF, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, // 1
0xFF, 0xDC, 0xBC, 0xBA, 0xB6, 0xCE, 0xFF, 0xFF, // 2
0xFF, 0xDD, 0xBE, 0xB6, 0xB6, 0xC9, 0xFF, 0xFF, // 3
0xFB, 0xF3, 0xEB, 0xDB, 0x80, 0xFB, 0xFF, 0xFF, // 4
0xFF, 0x8D, 0xAE, 0xAE, 0xAE, 0xB1, 0xFF, 0xFF, // 5
0xFF, 0xC1, 0x96, 0xB6, 0xB6, 0xD9, 0xFF, 0xFF, // 6
0xFF, 0xBF, 0xBC, 0xB3, 0xAF, 0x9F, 0xFF, 0xFF, // 7
0xFF, 0xC9, 0xB6, 0xB6, 0xB6, 0xC9, 0xFF, 0xFF, // 8
0xFF, 0xCD, 0xB6, 0xB6, 0xB4, 0xC1, 0xFF, 0xFF, // 9
0xFC, 0xF3, 0xCB, 0x9B, 0xEB, 0xF3, 0xFC, 0xFF, // A
0xFF, 0x80, 0xB6, 0xB6, 0xB6, 0xB6, 0xC9, 0xFF, // B
0xFF, 0xE3, 0xDD, 0xBE, 0xBE, 0xBE, 0xBE, 0xDD, // C
0xFF, 0x80, 0xBE, 0xBE, 0xBE, 0xBE, 0xDD, 0xE3, // D
0xFF, 0x80, 0xB6, 0xB6, 0xB6, 0xB6, 0xBE, 0xFF, // E
0xFF, 0x80, 0xB7, 0xB7, 0xB7, 0xB7, 0xBF, 0xFF, // F
*/


const int scrollDelay = 14;  // Controls how quickly the letters move
const int numVisibleColumns = 8;
const int numCharacters = 8;
int columns[] = {
  // list of column values (each bit is one pixel in the column)
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,  // Empty
  B00000000, B01111111, B01111111, B01001001, B01001001, B01111111, B00110110, B00000000,  // B (Bold)
  B00000000, B00011111, B00111111, B01100100, B01100100, B00111111, B00011111, B00000000,  // A (Bold)
  B00000000, B01111111, B01111111, B01000001, B01100011, B00111110, B00011100, B00000000,  // D (Bold)
  B00011100, B00111110, B01100011, B01000001, B01000101, B01100111, B00100111, B00000000,  // G (Bold)
  B00000000, B01111111, B01111111, B01001001, B01011101, B01000001, B01100011, B00000000,  // E (Bold)
  B00000000, B01111111, B01111111, B01001000, B01001100, B01111111, B00110011, B00000000,  // R (Bold)
  B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000   // Empty
};

int numColumns = numCharacters * numVisibleColumns;

int currentColumn = 0;

void setup() {
  //set pins to output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}


void loop() {

  // Render the current set of columns multiple times
  // For some reason; without this, it flickers and letters aren't persistent
  // This also controls how quickly the text moves
  for (int t = 0; t < scrollDelay; t++) {
    int matrixColumn = 0x01;

    // Starting with the current column, render it and the next 7 columns
    for (int columnIndex = currentColumn; columnIndex < numVisibleColumns + currentColumn; columnIndex++) {
      shiftOut(dataPin, clockPin, MSBFIRST, columns[columnIndex]);  //control ROW of dot matrix
      shiftOut(dataPin, clockPin, MSBFIRST, ~matrixColumn);         //control COL of dot matrix
      //return the latch pin high to signal chip that it
      //no longer needs to listen for information
      digitalWrite(latchPin, HIGH);      //pull the latchPin to save the data
      digitalWrite(latchPin, LOW);       //ground latchPin and hold low for as long as you are transmitting
      matrixColumn = matrixColumn << 1;  // Move to next matrix column
      delay(1);                          //wait for a microsecond
    }
  }

  currentColumn++;
  if (currentColumn >= (numColumns - numVisibleColumns)) {
    currentColumn = 0;
  }
}