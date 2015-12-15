/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://www.arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */
unsigned int inByte = 0;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  establishContact();  // send a byte to establish contact until receiver responds

  
}

// the loop function runs over and over again forever
void loop() {

  if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
    if (inByte < 20) {
      Serial.print("stopped");
      Serial.print("\n");
      digitalWrite(2, LOW);
    } else if (inByte >= 20 && inByte < 180) {
      Serial.print("repel");
      analogWrite(9,255);
      digitalWrite(2,HIGH);
      digitalWrite(3,LOW);
    } else {
      Serial.print("attract");
      analogWrite(2,inByte);
      digitalWrite(2, LOW);
      digitalWrite(3, HIGH);
      
    }
    //Serial.print(inByte);
    //Serial.print("\n");
    //analogWrite(9, inByte);
    //digitalWrite(2, LOW);
    //digitalWrite(3, HIGH);
    digitalWrite(13, HIGH); 
  }
    // turn the LED on (HIGH is the voltage level)
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print('A');   // send a capital A
    delay(300);
  }
}

