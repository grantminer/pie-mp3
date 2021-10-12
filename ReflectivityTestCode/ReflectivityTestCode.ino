#include <stdio.h>
#include <stdlib.h>

int IR1PIN = A0;
int IR2PIN = A1;
char outString;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(IR1PIN, INPUT);
  pinMode(IR2PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(analogRead(IR1PIN));
  Serial.print(",");
  Serial.println(analogRead(IR2PIN));
}
