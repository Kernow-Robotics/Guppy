/*
  Guppy.h - Library for ineracting with a Guppy robot.
  Created by Ben Green, July 29, 2024.
  Released into the public domain.
*/
#ifndef Guppy_h
#define Guppy_h

#include "Arduino.h"
#include <RF24.h>
#include <nRF24L01.h>
#include <SPI.h>
#include <pico/multicore.h>
#include <Ticker.h>

class Motor
{
public:
  float setpoint;
  Motor(int pinA, int pinB);
  void power(float power);

private:
  int _pinA;
  int _pinB;
  float _currentPower;
  int _lastUpdateTime;
  float _updateIncrement = 1.0; // (volts per second) rate at which a motor's power can change
  void _rawPower(float power);
  void _updatePower();
};

class Guppy
{
public:
  Guppy();
  Motor m0;
  Motor m1;
  void begin();
  // --------Background services--------
  void startBackgroundServices();
  // --------Motor functions--------
  void motorDrive(float power0, float power1);
  // --------Radio functions--------
  void initRadio();
  void startListening(uint8_t address[6]);
  void stopListening();
  void send(String message, uint8_t address[6]);
  String receive();
  // --------LED functions--------
  void lightOn();
  void lightOff();
  void heartbeat();
  void errorState();
  // --------Battery functions--------
  float updateVbatt();

  static Guppy *instance;

private:
  float _vbatt;
  RF24 _radio;
  Ticker _timer;
  static void _beginCore1Wrapper();
  void _beginCore1();
  static void _updateWrapper();
  void _update();
};

#endif