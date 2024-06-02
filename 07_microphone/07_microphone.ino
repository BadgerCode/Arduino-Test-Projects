// https://www.youtube.com/watch?v=bMs5J4bJOD0
// https://www.dfrobot.com/product-2357.html

int MicrophoneAnalogPin = 0;


void setup() {
  // Debug output on serial port; Used for Serial.println("blah blah");
  Serial.begin(9600);
}

unsigned long NextPrint = 0;

void loop() {
  unsigned long curTime = millis();

  if (curTime >= NextPrint) {
    int level = analogRead(MicrophoneAnalogPin);
    Serial.println(level);
    
    NextPrint = millis() + 100;
  }
}
