/*
arduino_pwm
 
 Demonstrates the control of analog output (PWM) pins of an Arduino board
 running the StandardFirmata firmware.  Moving the mouse horizontally across
 the sketch changes the output on pins 9 (value is proportional to the mouse
 X coordinate) and 11 (inversely porportional to mouse X).
 
 To use:
 * Using the Arduino software, upload the StandardFirmata example (located
 in Examples > Firmata > StandardFirmata) to your Arduino board.
 * Run this sketch and look at the list of serial ports printed in the
 message area below. Note the index of the port corresponding to your
 Arduino board (the numbering starts at 0).  (Unless your Arduino board
 happens to be at index 0 in the list, the sketch probably won't work.
 Stop it and proceed with the instructions.)
 * Modify the "arduino = new Arduino(...)" line below, changing the number
 in Arduino.list()[0] to the number corresponding to the serial port of
 your Arduino board.  Alternatively, you can replace Arduino.list()[0]
 with the name of the serial port, in double quotes, e.g. "COM5" on Windows
 or "/dev/tty.usbmodem621" on Mac.
 * Connect LEDs or other outputs to pins 9 and 11.
 * Run this sketch and move your mouse horizontally across the screen.
 
 For more information, see: http://playground.arduino.cc/Interfacing/Processing
 */

import processing.serial.*;
import cc.arduino.*;

Arduino arduino;

// The serial port
Serial myPort;

int width = 512;
int height = 200;

int dir1PinA = 2;
int dir2PinA = 3;
int speedPinA = 9; // Needs to be a PWM pin to be able to control motor speed

void setup() {
  size(512, 200);

  // Modify this line, by changing the "0" to the index of the serial
  // port corresponding to your Arduino board (as it appears in the list
  // printed by the line above).
  arduino = new Arduino(this, Arduino.list()[1], 57600);

  arduino.pinMode(dir1PinA, Arduino.OUTPUT);
  arduino.pinMode(dir2PinA, Arduino.OUTPUT);
  println(Arduino.list());
  //myPort = new Serial(this, Serial.list()[3], 9600);
  //println(Serial.list());

  // Alternatively, use the name of the serial port corresponding to your
  // Arduino (in double-quotes), as in the following line.
  //arduino = new Arduino(this, "/dev/tty.usbmodem621", 57600);
}

void draw() {
  background(constrain(mouseX / 2, 0, 255));

  //if (myPort.available() > 0) {
  //int inByte = myPort.read();

  int speed = constrain(mouseX / 2, 0, 255);
  textSize(32);
  fill(255,0,0);
  textMode(CENTER);
  text(speed, width/2, height/2);
  
  //int speed = 255;

  // Output analog values (PWM waves) to digital pin 9.
  // Note that only certain Arduino pins support analog output (PWM).
  

  if (speed < 20) 
  {
    arduino.digitalWrite(dir1PinA, Arduino.LOW);
    //arduino.digitalWrite(dir2PinA, Arduino.LOW);
    //arduino.analogWrite(speedPinA, 0);
    println("Stopped");
  } else if (speed >= 20 && speed < 180) 
  {
    int repelSpeed = int(map(speed, 20, 179, 0, 255));
    //arduino.analogWrite(speedPinA, repelSpeed);
    arduino.analogWrite(speedPinA, 255);
    arduino.digitalWrite(dir1PinA, Arduino.HIGH);
    arduino.digitalWrite(dir2PinA, Arduino.LOW);
    println("Repel Speed: " + repelSpeed); 
    
  } else 
  {
    int attractSpeed = int(map(speed, 180, 255, 0, 255));
    //arduino.analogWrite(speedPinA, attractSpeed);
    arduino.analogWrite(speedPinA, 255);
    arduino.digitalWrite(dir1PinA, Arduino.LOW);
    arduino.digitalWrite(dir2PinA, Arduino.HIGH);
    println("Attract Speed: " + attractSpeed);
  }


  // Turn on LED at pin 13
  arduino.digitalWrite(13, Arduino.HIGH);
  //}
}