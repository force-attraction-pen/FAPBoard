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

int PIN_A = 2;
int PIN_B = 3;
int PIN_POWER = 9;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(PIN_POWER, OUTPUT);
  pinMode(PIN_A, OUTPUT);
  pinMode(PIN_B, OUTPUT);

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
    Serial.print(inByte);
    switch(inByte) {
      case 3:
        Serial.print(" repel\n");
        analogWrite(9,255);
        digitalWrite(2,HIGH);
        digitalWrite(3,LOW);
        break;
      case 2:
        Serial.print(" stopped\n");
        analogWrite(9,0);
        digitalWrite(2, LOW);
        digitalWrite(3, HIGH);
        break;
      case 1:
        Serial.print(" attract\n");
        analogWrite(9,255);
        digitalWrite(2,LOW);
        digitalWrite(3,HIGH);      
        break;
      
    }
  }
}

void establishContact() {
  while (Serial.available() <= 0) {
    Serial.print('A');   // send a capital A
    delay(300);
  }
}

