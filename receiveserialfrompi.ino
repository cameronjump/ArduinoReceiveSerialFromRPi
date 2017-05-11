// Process serial data from raspberry pi to Arduino

// rgb pin values
int redPin = 3;
int greenPin = 5;
int bluePin = 6;

// default values depending on how your mosfets and light strips are setup 255 can be on or off
int red = 255;
int green = 255;
int blue = 255;


const byte numChars = 90;
char receivedChars[numChars];

boolean newData = false;

void setup() {
  Serial.begin(9600);
  //debugging statement
  //Serial.println("<Arduino is ready -->");
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin,OUTPUT);

  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);


}

void loop() {
  recvWithStartEndMarkers();
  processData();
  //environmentCheck();
}

void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  // if (Serial.available() > 0) {
  while (Serial.available() > 0 && newData == false) { // <<== NEW - get all bytes from buffer
    rc = Serial.read();

    if (recvInProgress == true) {
      if (rc != endMarker) {
        receivedChars[ndx] = rc;
        ndx++;
        if (ndx >= numChars) {
          ndx = numChars - 1;
        }
      }
      else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }

    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

//expected string format <1.RRR.GGG.BBB>
void processData() {
  if (newData == true) {
    newData = false;
    String string = String(receivedChars);
    String modifier = string.substring(0, 1);
    //debugging
    //Serial.println(string);
    Serial.println(modifier);
    if(modifier == "1") {
      //process string to r g b values
      red = string.substring(2,5).toInt();
      green = string.substring(6,9).toInt();
      blue = string.substring(10,13).toInt();
      //debugging
      //Serial.print("red: ");
      //Serial.print(red);
      //Serial.print(" green: ");
      //Serial.print(green);
      //Serial.print(" blue: ");
      //Serial.println(blue);
      lightControl();
    }
  }
}

void lightControl() {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

void environmentCheck() {
  //here you can send data via serial to raspberry pi
  //Serial.print(data)
}
