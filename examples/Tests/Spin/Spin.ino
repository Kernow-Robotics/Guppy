#include <Guppy.h>

Guppy guppy;

void setup() {
  guppy.begin();
  guppy.startBackgroundServices();
}

void loop() {
  float curentPower = 0.0;
  float maxpower = 5.0;
  while (curentPower < maxpower)
  {
    guppy.motorDrive(curentPower, -curentPower);
    curentPower += 0.1;
    delay(100);
  }
  delay(1000);
  while (curentPower > -maxpower)
  {
    guppy.motorDrive(curentPower, -curentPower);
    curentPower -= 0.1;
    delay(100);
  }
  delay(1000);
  while ( curentPower < 0.0)
  {
    guppy.motorDrive(curentPower, -curentPower);
    curentPower += 0.1;
    delay(100);
  }
}
