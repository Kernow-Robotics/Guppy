/*
  Guppy.h - Library for ineracting with a Guppy robot.
  Created by Ben Green, July 29, 2024.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Guppy.h"

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define pinServo0 2
#define pinServo1 3
#define pinServo2 4
#define pinServo3 5
#define pinM0a 11
#define pinM0b 10
#define pinM1a 9
#define pinM1b 8
#define pinLED 20
#define pinVbatt 29
#define pinSPI_CE 19
#define pinSPI_MISO 0
#define pinSPI_CS 1
#define pinSPI_SCK 2
#define pinSPI_MOSI 3

RF24 radio(pinSPI_CE, pinSPI_CS);

// --------Motor class--------
Motor::Motor(int pinA, int pinB)
{
  _pinA = pinA;
  _pinB = pinB;
  pinMode(_pinA, OUTPUT);
  pinMode(_pinB, OUTPUT);
}

void Motor::_power(int power)
{
  if (power == 0)
  {
    digitalWrite(_pinA, LOW);
    digitalWrite(_pinB, LOW);
  }
  else if (power > 0)
  {
    digitalWrite(_pinB, LOW);
    analogWrite(_pinA, power);
  }
  else if (power < 0)
  {
    digitalWrite(_pinA, LOW);
    analogWrite(_pinB, abs(power));
  }
}

Guppy::Guppy()
{
  Motor m0(pinM0a, pinM0b);
  Motor m1(pinM1a, pinM1b);
  pinMode(pinServo0, OUTPUT);
  pinMode(pinServo1, OUTPUT);
  pinMode(pinServo2, OUTPUT);
  pinMode(pinServo3, OUTPUT);
  pinMode(pinLED, OUTPUT);
  pinMode(pinVbatt, INPUT);

  // Serial.begin(115200);
  // _vbatt = 2.0*((3.3/1024.0)*analogRead(pinVbatt));
}

void Guppy::begin()
{

  SPI.setRX(0); // MISO
  SPI.setCS(1);
  SPI.setSCK(2);
  SPI.setTX(3); // MOSI
  SPI.begin();

  Serial.begin(115200);
  // while (!Serial) {
  //   // Wait for serial port to connect. Needed for native USB
  // }
  Serial.println(F("Starting Guppy..."));

  heartbeat();

  _vbatt = 2.0 * ((3.3 / 1024.0) * analogRead(pinVbatt));
}

// --------Motor functions--------
void Guppy::motorDrive(int power0 = 0, int power1 = 0)
{
  m0.power(power0);
  m1.power(power1);
}

// --------Radio functions--------
void Guppy::initRadio()
{
  // Initialize the radio
  Serial.println(F("Initializing radio..."));
  if (!radio.begin(19, 1))
  {
    Serial.println(F("radio hardware is not responding!!"));
    while (1)
    {
      errorState();
    } // hold in infinite loop
  }
  else
  {
    Serial.println(F("radio initialized successfully"));
  }
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
}

void Guppy::startListening(uint8_t address[6])
{
  radio.openReadingPipe(0, address);
  radio.startListening();
}

void Guppy::stopListening()
{
  radio.stopListening();
}

void Guppy::send(String message, uint8_t address[6])
{
  // const byte demoaddresses[6] = "N3";
  // radio.openWritingPipe(demoaddresses);
  radio.openWritingPipe(address);
  // Serial.print("openning writing pipe");
  // Serial.println(address);
  char text[32];
  message.toCharArray(text, sizeof(text));
  radio.write(&text, sizeof(text));
  Serial.print("Sent: ");
  Serial.print(text);
  Serial.print(" to radio address: ");
  Serial.println((char *)address);
}

// --------LED functions--------
void Guppy::lightOn()
{
  digitalWrite(pinLED, HIGH);
}

void Guppy::lightOff()
{
  digitalWrite(pinLED, LOW);
}

void Guppy::heartbeat()
{
  lightOn();
  delay(50);
  lightOff();
  delay(50);
  lightOn();
  delay(50);
  lightOff();
}

void Guppy::errorState()
{
  lightOn();
  delay(100);
  lightOff();
  delay(100);
}

// --------Battery functions--------
float Guppy::updateVbatt()
{
  float batteryReading = 2.0 * ((3.3 / 1024.0) * analogRead(pinVbatt));
  _vbatt = (_vbatt * 10 + batteryReading) / 11;
  // int batteryReading1 = analogRead(pinVbatt);
  // float batteryVoltage1 = 2.0*((3.3/1024.0)*batteryReading1);
  // int batteryReading2 = analogRead(pinVbatt);
  // float batteryVoltage2 = 2.0*((3.3/1024.0)*batteryReading2);
  // int batteryReading3 = analogRead(pinVbatt);
  // float batteryVoltage3 = 2.0*((3.3/1024.0)*batteryReading3);
  // float ave = (batteryVoltage1 + batteryVoltage2 + batteryVoltage3)/3.0;
  // Serial.println(ave);
  return _vbatt;
}
