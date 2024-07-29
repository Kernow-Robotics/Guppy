/*
  Guppy.h - Library for ineracting with a Guppy robot.
  Created by Ben Green, July 29, 2024.
  Released into the public domain.
*/
#ifndef Guppy_h
#define Guppy_h

#include "Arduino.h"

class Guppy
{
  public:
    Guppy();
    void m0Power(int power);
    void m1Power(int power);
    void motorDrive(int power0, int power1);
    void heartbeat();
    float getVbatt();
  private:
    int _pin;
};
#endif