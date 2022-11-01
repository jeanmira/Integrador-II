#include <Arduino.h>

#define BeltRead_Pin 22

void setup() {
  pinMode(BeltRead_Pin, INPUT);
  Serial.begin(9600);
}

void loop() {
  
  Serial.println(digitalRead(BeltRead_Pin));
  delay(100);
}