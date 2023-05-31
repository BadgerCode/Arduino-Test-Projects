/*
Schematic
Arduino ground -> LED ground
LED positive -> 560 ohm resistor
Gold side of resistor -> power cable
Power cable -> Digital output pin 13
*/


const int ledPin = 13;


void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(ledPin, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(ledPin, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);
}
