int redCol = 255;    // curently red 0-255
int greenCol = 0;    // curently green 0-255
int blueCol = 0;     // curently blue 0-255
int delayBlink = 1;  // milliseconds
int maxBright = 255; // 0-255 0-off 255-max

void setup() {
  pinMode(9,OUTPUT);  // red
  pinMode(10,OUTPUT); // green
  pinMode(11,OUTPUT); // blue
}

void loop() {
  while(greenCol<maxBright)
  {
    analogWrite(9,redCol);
    analogWrite(10,greenCol);
    greenCol++;
    if(redCol>0)
      redCol--;
    delay(delayBlink);
  }
  while(blueCol<maxBright)
  {
    analogWrite(10,greenCol);
    analogWrite(11,blueCol);
    blueCol++;
    if(greenCol>0)
      greenCol--;
    delay(delayBlink);
  }
  while(redCol<maxBright)
  {
    analogWrite(9,redCol);
    analogWrite(11,blueCol);
    redCol++;
    if(blueCol>0)
      blueCol--;
    delay(delayBlink);
  }
}
