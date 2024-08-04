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

class Guppy
{
  public:
    Guppy();
    void begin();
    // --------Motor functions--------
    void m0Power(int power);
    void m1Power(int power);
    void motorDrive(int power0, int power1);
    // --------Radio functions--------
    RF24 radio;
    void initRadio();
    void startListening(uint8_t address[6]);
    void stopListening();
    void send(String message, uint8_t address[6]);
    // --------LED functions--------
    void lightOn();
    void lightOff();
    void heartbeat();
    void errorState();
    // --------Battery functions--------
    float updateVbatt();
  private:
    float _vbatt;
};
#endif