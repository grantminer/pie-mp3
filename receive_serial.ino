#include <Adafruit_MotorShield.h>
#include <stdio.h>
#include <stdlib.h>

int IR1PIN = A0;
int IR2PIN = A1;
char outString;
char *strings[10];
char *token = NULL;
// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

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
  // turn on motor
  motorLeft->run(FORWARD);
  motorLeft->run(FORWARD);
}

void loop() {
  while(!Serial.available()) {} //wait for data
  // serial read section
  while (Serial.available()){
    if (Serial.available() >0){
      String dataString = Serial.readString();
    // prints the received data
    Serial.print("I received: ");
    Serial.println(dataString);
  }
  }
  //parse string
  data = dataString.c_str();
  byte index = 0;
  token = strtok(data, ",");  // new token from string, broken by commas
    while(token != NULL){
        strings[index] = token;
        index++;
        token = strtok(NULL, ",");  // takes a list of delimiters
    }
    for(int n = 0; n < index; n++){ // print the tokens
    Serial.println(strings[n]); //second and third should be motor speeds we want
   }
    motorLeft->setSpeed(strings[1]);
    motorRight->setSpeed(strings[2]);
    if (strings[1] > 0) {
       motorLeft->run(FORWARD);
    }
    else {
      motorLeft->run(BACKWARD);
    }
    if (strings[2] > 0) {
       motorRight->run(FORWARD);
    }
    else {
      motorRight->run(BACKWARD);
    }
    serialwrite(dataString, dataString);
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

void serialwrite(right_speed, left_speed) {
  Serial.println(analogRead(IR1PIN));  Serial.print(",");
  Serial.print(analogRead(IR2PIN));  Serial.print(",");
  Serial.print(right_speed);  Serial.print(",");
  Serial.print(left_speed);
}
