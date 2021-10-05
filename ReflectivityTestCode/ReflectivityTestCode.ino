#include <stdio.h>
#include <stdlib.h>

int IR1PIN = 9;
int IR2PIN = 10;
char outString;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(IR1PIN, INPUT);
  pinMode(IR2PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(digitalRead(IR1PIN));
  Serial.print(",");
  Serial.println(digitalRead(IR2PIN));
}
