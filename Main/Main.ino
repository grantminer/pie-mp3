#include <Adafruit_MotorShield.h>
#include <stdio.h>
#include <stdlib.h>

int IR1PIN = A0;
int IR2PIN = A1;
char outString;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61);

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *motorLeft = AFMS.getMotor(1);
// You can also make another motor on port M2
Adafruit_DCMotor *motorRight = AFMS.getMotor(2);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  pinMode(IR1PIN, INPUT);
  pinMode(IR2PIN, INPUT);
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
  // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // Set the speed to start, from 0 (off) to 255 (max speed)
  motorLeft->setSpeed(100);
  motorRight->setSpeed(100);
  motorLeft->run(FORWARD);
  motorRight->run(FORWARD);
  // turn on motor
  motorLeft->run(RELEASE);
  motorRight->run(RELEASE);
}

void loop() {
  motorLeft->run(FORWARD);
  motorLeft->run(FORWARD);

  while (!leftSeesBlack() && !rightSeesBlack()) { //it's following the right edge of the line
    motorRight->run(FORWARD);
    motorLeft->run(FORWARD);
  if (leftSeesBlack()) { //stop left motor, keep right motor running to drive back to line
    motorLeft->run(RELEASE);
    motorRight->run(FORWARD);
  }
  if (rightSeesBlack()) {
    motorRight->run(RELEASE);
    motorLeft->run(FORWARD);
  }
  }
}

void move(right_speed, left_speed) {
  if (left_speed == 0 && right_speed == 0) {
    motorLeft->run(RELEASE);
    motorRight->run(RELEASE);
  }
  else {
  motorLeft->setSpeed(left_speed);
  motorRight->setSpeed(right_speed);
  motorLeft->run(FORWARD);
  motorRight->run(FORWARD);
  }
}


void serialread() {
  input = Serial.read;
  
}

void serialwrite(right_speed, left_speed) {
  Serial.println(analogRead(IR1PIN));  Serial.print(",");
  Serial.print(analogRead(IR2PIN));  Serial.print(",");
  Serial.print(right_speed);  Serial.print(",");
  Serial.print(left_speed);
}
