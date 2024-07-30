/*
  Guppy.h - Library for ineracting with a Guppy robot.
  Created by Ben Green, July 29, 2024.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Guppy.h"

#define pinServo0 2
#define pinServo1 3
#define pinServo2 4
#define pinServo3 5
#define pinM0a 10
#define pinM0b 11
#define pinM1a 9
#define pinM1b 8
#define pinLED 20
#define pinVbatt 29

Guppy::Guppy()
{
  pinMode(pinServo0, OUTPUT);
  pinMode(pinServo1, OUTPUT);
  pinMode(pinServo2, OUTPUT);
  pinMode(pinServo3, OUTPUT);
  pinMode(pinM0a, OUTPUT);
  pinMode(pinM0b, OUTPUT);
  pinMode(pinM1a, OUTPUT);
  pinMode(pinM1b, OUTPUT);
  pinMode(pinLED, OUTPUT);
  pinMode(pinVbatt, INPUT);
  Serial.begin(115200);
}

void Guppy::m0Power(int power = 0)
{
  Serial.print("m0 power =: ");
  Serial.println(power);
  if (power == 0) {
    digitalWrite(pinM0a, LOW);
    digitalWrite(pinM0b, LOW);
  }
  else if (power > 0) {
    digitalWrite(pinM0b, LOW);
    analogWrite(pinM0a, power);
  }
  else if (power < 0) {
    digitalWrite(pinM0a, LOW);
    analogWrite(pinM0b, abs(power));
  }
}

void Guppy::m1Power(int power = 0)
{
  Serial.print("m1 power =: ");
  Serial.println(power);
  if (power == 0) {
    digitalWrite(pinM1a, LOW);
    digitalWrite(pinM1b, LOW);
  }
  else if (power > 0) {
    digitalWrite(pinM1b, LOW);
    analogWrite(pinM1a, power);
  }
  else if (power < 0) {
    digitalWrite(pinM1a, LOW);
    analogWrite(pinM1b, abs(power));
  }
}

void Guppy::motorDrive(int power0 = 0, int power1 = 0)
{
  m0Power(power0);
  m1Power(power1);
}
void Guppy::heartbeat(){
  digitalWrite(pinLED, HIGH);
  delay(50);
  digitalWrite(pinLED, LOW);
  delay(50);
  digitalWrite(pinLED, HIGH);
  delay(50);
  digitalWrite(pinLED, LOW);
}

float Guppy::getVbatt(){
  int batteryReading1 = analogRead(pinVbatt);
  float batteryVoltage1 = 2.0*((3.3/1024.0)*batteryReading1);
  delay(1);
  int batteryReading2 = analogRead(pinVbatt);
  float batteryVoltage2 = 2.0*((3.3/1024.0)*batteryReading2);
  delay(1);
  int batteryReading3 = analogRead(pinVbatt);
  float batteryVoltage3 = 2.0*((3.3/1024.0)*batteryReading3);
  float ave = (batteryVoltage1 + batteryVoltage2 + batteryVoltage3)/3.0;
  return ave;
}