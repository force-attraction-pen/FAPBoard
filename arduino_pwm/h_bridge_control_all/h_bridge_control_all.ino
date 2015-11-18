//Code by Reichenstein7 (thejamerson.com)

//Keyboard Controls:
//
// 1 -Motor 1 Left
// 2 -Motor 1 Stop
// 3 -Motor 1 Right
//
// 4 -Motor 2 Left
// 5 -Motor 2 Stop
// 6 -Motor 2 Right

// Declare L298N Dual H-Bridge Motor Controller directly since there is not a library to load.

// Motor 1
int dir1PinA = 2;
int dir2PinA = 3;
int speedPinA = 9; // Needs to be a PWM pin to be able to control motor speed

// Motor 2
int dir1PinB = 4;
int dir2PinB = 5;
int speedPinB = 10; // Needs to be a PWM pin to be able to control motor speed

void setup() {  // Setup runs once per reset
  // initialize serial communication @ 9600 baud:
  Serial.begin(9600);

  //Define L298N Dual H-Bridge Motor Controller Pins

  pinMode(dir1PinA, OUTPUT);
  pinMode(dir2PinA, OUTPUT);
  pinMode(speedPinA, OUTPUT);
  pinMode(dir1PinB, OUTPUT);
  pinMode(dir2PinB, OUTPUT);
  pinMode(speedPinB, OUTPUT);

}

void loop() {

  // Initialize the Serial interface:

  if (Serial.available() > 0) {
    int inByte = Serial.read();
    int speed; // Local variable

    switch (inByte) {
      //______________Motor 2______________

      case '4': // Motor 2 Forward
        analogWrite(speedPinA, 255);
        digitalWrite(dir1PinA, LOW);
        digitalWrite(dir2PinA, HIGH);
        Serial.println("Motor 2 Forward");
        Serial.println("   ");
        break;

      case '5': // Motor 2 Stop (Freespin)
        analogWrite(speedPinA, 0);
        digitalWrite(dir1PinA, LOW);
        digitalWrite(dir2PinA, HIGH);
        Serial.println("Motor 2 Stop");
        Serial.println("   ");
        break;

      default:
        // turn all the connections off if an unmapped key is pressed:
        for (int thisPin = 2; thisPin < 11; thisPin++) {
          digitalWrite(thisPin, LOW);
        }
    }
  }
}
