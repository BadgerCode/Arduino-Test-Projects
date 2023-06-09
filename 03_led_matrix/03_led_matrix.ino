//Dot-Matrix Display
//The dot-matrix will display 0 to F circularly
//Email:support@sunfounder.com
//Website:www.sunfounder.com
//2015.5.7
const int latchPin = 8;   //Pin connected to ST_CP of 74HC595
const int clockPin = 12;  //Pin connected to SH_CP of 74HC595
const int dataPin = 11;   //Pin connected to DS of 74HC595

int numCharacters = 18;
int columns[] = {
  // list of column values (each bit is one pixel in the column)
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /*” “,0*/
  0xFF, 0xC1, 0xBE, 0xBE, 0xBE, 0xC1, 0xFF, 0xFF, /*”0″,1*/
  0xFF, 0xDF, 0xDF, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, /*”1″,2*/
  0xFF, 0xDC, 0xBC, 0xBA, 0xB6, 0xCE, 0xFF, 0xFF, /*”2″,3*/
  0xFF, 0xDD, 0xBE, 0xB6, 0xB6, 0xC9, 0xFF, 0xFF, /*”3″,4*/
  0xFB, 0xF3, 0xEB, 0xDB, 0x80, 0xFB, 0xFF, 0xFF, /*”4″,5*/
  0xFF, 0x8D, 0xAE, 0xAE, 0xAE, 0xB1, 0xFF, 0xFF, /*”5″,6*/
  0xFF, 0xC1, 0x96, 0xB6, 0xB6, 0xD9, 0xFF, 0xFF, /*”6″,7*/
  0xFF, 0xBF, 0xBC, 0xB3, 0xAF, 0x9F, 0xFF, 0xFF, /*”7″,8*/
  0xFF, 0xC9, 0xB6, 0xB6, 0xB6, 0xC9, 0xFF, 0xFF, /*”8″,9*/
  0xFF, 0xCD, 0xB6, 0xB6, 0xB4, 0xC1, 0xFF, 0xFF, /*”9″,10*/
  0xFC, 0xF3, 0xCB, 0x9B, 0xEB, 0xF3, 0xFC, 0xFF, /*”A”,11*/
  0xFF, 0x80, 0xB6, 0xB6, 0xB6, 0xB6, 0xC9, 0xFF, /*”B”,12*/
  0xFF, 0xE3, 0xDD, 0xBE, 0xBE, 0xBE, 0xBE, 0xDD, /*”C”,13*/
  0xFF, 0x80, 0xBE, 0xBE, 0xBE, 0xBE, 0xDD, 0xE3, /*”D”,14*/
  0xFF, 0x80, 0xB6, 0xB6, 0xB6, 0xB6, 0xBE, 0xFF, /*”E”,15*/
  0xFF, 0x80, 0xB7, 0xB7, 0xB7, 0xB7, 0xBF, 0xFF, /*”F”,16*/
  0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, /*” “,17*/
};
int numVisibleColumns = 8;
int numColumns = numCharacters * numVisibleColumns;

int currentColumn = 0;

void setup() {
  //set pins to output
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}



void loop() {

  // Render the current set of columns 10 times (for some reason; without this, it flicked and letters aren't persistent)
  for (int t = 0; t < 10; t++) {
    int matrixColumn = 0x01;

    // Starting with the current column, render it and the next 7 columns
    for (int columnIndex = currentColumn; columnIndex < numVisibleColumns + currentColumn; columnIndex++) {
      shiftOut(dataPin, clockPin, MSBFIRST, ~columns[columnIndex]);  //control ROW of dot matrix
      shiftOut(dataPin, clockPin, MSBFIRST, ~matrixColumn);          //control COL of dot matrix
      //return the latch pin high to signal chip that it
      //no longer needs to listen for information
      digitalWrite(latchPin, HIGH);  //pull the latchPin to save the data
      //delay(1); //wait for a microsecond
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