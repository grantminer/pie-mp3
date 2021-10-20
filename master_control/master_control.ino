#include <Adafruit_MotorShield.h>
#include <stdio.h>
#include <stdlib.h>

int IR1PIN = A1;
int IR2PIN = A2;
int IR3PIN = A3;
int threshold = 500;
int lb;
int rb;
int cb;
int cruise = 50;
int slow = -50;
int catchup = 50;
int stop_speed = 0;
int ls = 40;
int rs = 40;
String param;
int val;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *motorLeft = AFMS.getMotor(2);
// You can also make another motor on port M2
Adafruit_DCMotor *motorRight = AFMS.getMotor(1);

void setup() {
    Serial.begin(9600);           // set up Serial library at 9600 bps
    pinMode(IR1PIN, INPUT);
    pinMode(IR2PIN, INPUT);
    pinMode(IR3PIN, INPUT);
    if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
      Serial.println("Could not find Motor Shield. Check wiring.");
      while (1);
    }
    Serial.println("Motor Shield found.");
  
    // Set the speed to start, from 0 (off) to 255 (max speed)
    
    motorLeft->setSpeed(ls);
    motorRight->setSpeed(rs);
    // turn on motor
    motorLeft->run(FORWARD);
    motorRight->run(FORWARD);
}

void loop() {

//    newparams();
//
//    if (val) {
//      threshold = val;
//      Serial.println(val);
//    }

    seesBlack();
    
    movementLogic(lb, rb, cb, threshold, cruise, slow, catchup);

    //Serial.println("Made it!");
    motorLeft->run(RELEASE);
    motorRight->run(RELEASE);

    if (ls > 0) {
       motorLeft->run(FORWARD);
    }
    else {
      motorLeft->run(BACKWARD);
    }
    if (rs > 0) {
       motorRight->run(FORWARD);
    }
    else {
      motorRight->run(BACKWARD);
    }
    
    motorLeft->setSpeed(abs(ls));
    motorRight->setSpeed(abs(rs));
    
}

int movementLogic(bool lb, bool rb, bool cb, int threshold, int cruise, int slow, int catchup) {
  
    if (!lb && !rb) {
      ls = cruise;
      rs = cruise;
    } else if (lb && !rb) {
      ls = slow;
      rs = catchup;
    } else if (!lb && rb) {
      ls = catchup;
      rs = slow;
    } else if (lb && rb) {
      ls = stop_speed;
      rs = stop_speed;
    } else {
      ls = stop_speed;
      rs = stop_speed;
      Serial.println("Edge Case (lb:"+ String(lb) + ", rb:" + String(rb) + ", ls:" + String(ls) + ", rs" + String(rs));
    } 

    if (!cb && !lb && !rb) {
      ls = cruise;
      rs = -1 * cruise;
    }
}

int seesBlack() {
  
    int left_analog = analogRead(IR1PIN);
    int right_analog = analogRead(IR2PIN);
    int center_analog = analogRead(IR3PIN);

    lb = 0;
    rb = 0;
    cb = 0;

    if (center_analog > threshold) {
        cb = 1;
    }
    
    if (left_analog > threshold) {
        lb = 1;
    }
  
    if (right_analog > threshold) {
        rb = 1;
    }

    Serial.print(left_analog); Serial.print(","); Serial.print(center_analog); Serial.print(","); Serial.print(right_analog); Serial.print(","); Serial.print(ls); Serial.print(","); Serial.println(rs);
}

String newparams() {
    val = 0;
    while (Serial.available()) {
      if (Serial.available() > 0) {
        val = int(Serial.read());
        Serial.println(val);
      }
    }

//    if (Serial.available()>0) {
//      char incoming = char(Serial.read());
//
//      Serial.println(incoming);
//    }
}
