#include <Guppy.h>

Guppy guppy;

void setup() {
  guppy.begin();
}

void loop() {
  float power = 5.0;
  guppy.motorDrive(power, power);
  delay(1000);
  guppy.motorDrive(-power, -power);
  delay(1000);
}
