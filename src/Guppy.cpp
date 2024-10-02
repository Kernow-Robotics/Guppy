/*
  Guppy.cpp - Library for ineracting with a Guppy robot.
  Created by Ben Green, July 29, 2024.
  Released into the public domain.
*/

#include "Arduino.h"
#include "Guppy.h"

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <pico/multicore.h>

// --------PIN DECLARATIONS--------
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

// --------MOTOR CLASS--------
Motor::Motor(int pinA, int pinB)
{
  _pinA = pinA;
  _pinB = pinB;
  pinMode(_pinA, OUTPUT);
  pinMode(_pinB, OUTPUT);
  _lastUpdateTime = millis();
}

void Motor::power(float power)
{
  if (power > 5.0)
  {
    power = 5.0;
  }
  else if (power < -5.0)
  {
    power = -5.0;
  }
  setpoint = power;
}

void Motor::_rawPower(float power)
{
  if (power > 5.0)
  {
    power = 5.0;
  }
  else if (power < -5.0)
  {
    power = -5.0;
  }

  int pwmPower = power * (255 / 5.0);
  // int pwmPower = int(map(power, -5.0, 5.0, -255, 255));

  if (pwmPower == 0)
  {
    digitalWrite(_pinA, LOW);
    digitalWrite(_pinB, LOW);
  }
  else if (pwmPower > 0)
  {
    digitalWrite(_pinB, LOW);
    analogWrite(_pinA, pwmPower);
  }
  else if (pwmPower < 0)
  {
    digitalWrite(_pinA, LOW);
    analogWrite(_pinB, abs(pwmPower));
  }
}

void Motor::_updatePower()
{
  float diff = (millis() - _lastUpdateTime) / 1000.0; // time since last update in seconds
  if (_currentPower < setpoint)
  {
    _currentPower += _updateIncrement * diff;
    if (_currentPower > setpoint)
    {
      _currentPower = setpoint;
    }
  }
  else if (_currentPower > setpoint)
  {
    _currentPower -= _updateIncrement * diff;
    if (_currentPower < setpoint)
    {
      _currentPower = setpoint;
    }
  }
  _rawPower(_currentPower);
  _lastUpdateTime = millis();

  Serial.print("diff = ");
  Serial.println(diff);
  Serial.print("currentPower = ");
  Serial.println(_currentPower);
}

// --------GUPPY CLASS--------
Guppy *Guppy::instance = nullptr;

Guppy::Guppy() : m0(pinM0a, pinM0b),
                 m1(pinM1a, pinM1b),
                 _radio(pinSPI_CE, pinSPI_CS),
                 _timer()
{
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

  SPI.setRX(pinSPI_MISO); // MISO
  SPI.setCS(pinSPI_CS);
  SPI.setSCK(pinSPI_SCK);
  SPI.setTX(pinSPI_MOSI); // MOSI
  SPI.begin();

  Serial.begin(115200);
  // while (!Serial) {
  //   // Wait for serial port to connect. Needed for native USB
  // }

  Serial.println(F("Starting Guppy..."));

  heartbeat();

  _vbatt = 2.0 * ((3.3 / 1024.0) * analogRead(pinVbatt));
}
// --------Background services--------
void Guppy::startBackgroundServices()
{
  instance = this;
  multicore_launch_core1(_beginCore1Wrapper);
}

void Guppy::_beginCore1Wrapper()
{
  instance->_beginCore1();
}

void Guppy::_beginCore1()
{
  _timer.attach(0.02, _updateWrapper);
  while (true)
  {
    // Core 1 stays busy here
    tight_loop_contents(); // Keeps Core 1 running without blocking
  }
}

void Guppy::_updateWrapper()
{
  instance->_update();
}

void Guppy::_update()
{
  Serial.print("updating...");
  Serial.print("vbatt = ");
  Serial.print(_vbatt);
  Serial.print(" ");
  Serial.println(millis());
  m0._updatePower();
  m1._updatePower();
}

// --------Motor functions--------
void Guppy::motorDrive(float power0, float power1)
{
  m0.power(power0);
  m1.power(power1);
}

// --------Radio functions--------
void Guppy::initRadio()
{
  // Initialize the radio
  Serial.println(F("Initializing radio..."));
  if (!_radio.begin(19, 1))
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
  _radio.setPALevel(RF24_PA_MAX);
  _radio.setDataRate(RF24_250KBPS);
  _radio.stopListening();
}

void Guppy::startListening(uint8_t address[6])
{
  _radio.openReadingPipe(0, address);
  _radio.startListening();
}

void Guppy::stopListening()
{
  _radio.stopListening();
}

void Guppy::send(String message, uint8_t address[6])
{
  _radio.openWritingPipe(address);
  char text[32];
  message.toCharArray(text, sizeof(text));
  _radio.write(&text, sizeof(text));
  // Serial.print("Sent: ");
  // Serial.print(text);
  // Serial.print(" to radio address: ");
  // Serial.println((char *)address);
}

String Guppy::receive()
{
  if (_radio.available())
  {
    char text[32] = {0};
    _radio.read(&text, sizeof(text));
    Serial.print("Received: ");
    Serial.println(text);
    String message = String(text);
    return message;
  }
  else
  {
    return "";
  }
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

// Function to convert voltage to SOC
float Guppy::voltage_to_soc(float voltage)
{
  // Check for values outside the table range
  float voltage_soc_table[][2] = {
      {4.20, 100},
      {4.15, 95},
      {4.10, 90},
      {4.05, 85},
      {4.00, 80},
      {3.95, 75},
      {3.90, 70},
      {3.85, 65},
      {3.80, 60},
      {3.75, 55},
      {3.70, 50},
      {3.65, 45},
      {3.60, 40},
      {3.55, 35},
      {3.50, 30},
      {3.45, 25},
      {3.40, 20},
      {3.35, 15},
      {3.30, 10},
      {3.25, 5},
      {3.20, 0}};
  if (voltage >= 4.2)
  {
    return 100.0;
  }
  else if (voltage <= 3.2)
  {
    return 0.0;
  }

  // Loop through the voltage-SOC table and interpolate between values
  for (int i = 0; i < (sizeof(voltage_soc_table) / sizeof(voltage_soc_table[0])) - 1; i++)
  {
    float v_high = voltage_soc_table[i][0];
    float soc_high = voltage_soc_table[i][1];
    float v_low = voltage_soc_table[i + 1][0];
    float soc_low = voltage_soc_table[i + 1][1];

    if (voltage <= v_high && voltage >= v_low)
    {
      // Linear interpolation between the two points
      return soc_low + (soc_high - soc_low) * (voltage - v_low) / (v_high - v_low);
    }
  }

  // If voltage is out of range, return -1 (should never happen due to prior checks)
  return -1.0;
}