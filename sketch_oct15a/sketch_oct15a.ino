
#include <stdio.h>
#include <stdlib.h>

int IR1PIN = A0;
int IR2PIN = A1;
char outString;
int POT = 5; //potentiometer in A5
int pot_val = 0;
int adj_IR2 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(IR1PIN, INPUT);
  pinMode(IR2PIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  pot_val = 0%analogRead(POT); //sensor value offset by potentiometer, set to 167 based on black electrical tape w/data cable under support
  adj_IR2 = analogRead(IR2PIN) + pot_val;
  Serial.print(analogRead(IR1PIN));
  Serial.print(",");
  Serial.print(adj_IR2);
  Serial.print(",");
  Serial.println(pot_val);
}
