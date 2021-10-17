#include <Adafruit_MotorShield.h>
#include <stdio.h>
#include <stdlib.h>

int IR1PIN = A0;
int IR2PIN = A1;
int left_speed = 100;;
int right_speed = 100;;
int threshold = 350;
bool lb;
bool rb;
int cruise = 100;
int slow = -30;
int catchup = 130;
int stop_speed = 0;
int ls;
int rs;
String param;
String val;
String value;

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
    if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
      Serial.println("Could not find Motor Shield. Check wiring.");
      while (1);
    }
    Serial.println("Motor Shield found.");
  
    // Set the speed to start, from 0 (off) to 255 (max speed)
    
    motorLeft->setSpeed(left_speed);
    motorRight->setSpeed(right_speed);
    // turn on motor
    motorLeft->run(FORWARD);
    motorRight->run(FORWARD);
    serialwrite(100, 100);
}

void loop() {

    param, val = newparams();

    if (param && val) {
      if (param == "cruise") {
        cruise = val.toInt();
      } else if (param == "slow") {
        slow = val.toInt();
      } else if (param == "catchup") {
        catchup = val.toInt();
      }
    }

    lb, rb = seesBlack();
    
    ls = NULL;
    rs = NULL;
    
    ls, rs = movementLogic(lb, rb, left_speed, right_speed, threshold, cruise, slow, catchup);

    if (ls && rs) {
      motorLeft->run(RELEASE);
      motorRight->run(RELEASE);
      motorLeft->setSpeed(abs(int(left_speed)));
      motorRight->setSpeed(abs(int(right_speed)));
      if (left_speed > 0) {
         motorLeft->run(FORWARD);
      }
      else {
        motorLeft->run(BACKWARD);
      }
      if (right_speed > 0) {
         motorRight->run(FORWARD);
      }
      else {
        motorRight->run(BACKWARD);
      }

      left_speed = ls; right_speed = rs;
      
      serialwrite(left_speed, right_speed);
    }
}

int movementLogic(bool lb, bool rb, int left_speed, int right_speed, int threshold, int cruise, int slow, int catchup) {
  
    if (!lb && !rb) {
      ls = cruise;
      rs = cruise;
    }

    if (lb && !rb) {
      ls = slow;
      rs = catchup;
    }

    if (!lb && rb) {
      ls = catchup;
      rs = slow;
    }

    if (lb && rb) {
      ls = stop_speed;
      rs = stop_speed;
    } else {
      ls = stop_speed;
      rs = stop_speed;
      Serial.println("Edge Case (lb:"+ String(lb) + ", rb:" + String(rb) + ", ls:" + String(left_speed) + ", rs" + String(right_speed));
    }

    if (!(ls == left_speed && rs == right_speed)) {
      return ls, rs;
    }
}

bool seesBlack() {
    int left_analog = analogRead(IR1PIN);
    int right_analog = analogRead(IR2PIN);
    
    if (left_analog > threshold) {
          bool lb = false;
    } else {
          bool lb = true;
    }
  
    if (right_analog > threshold) {
        bool rb = false;
    } else {
        bool rb = true;
    }
    
    return lb, rb;
}

void serialwrite(int left_speed, int right_speed) {
    Serial.print(left_speed);  Serial.print(",");
    Serial.println(right_speed);
}

String newparams() {
    if (Serial.available()>0) {
      String parameter = Serial.readStringUntil(":");
      String newVal = Serial.readStringUntil(";");

      return parameter, newVal;
    }
}
