#include <Guppy.h>

Guppy guppy;

void setup() {
  guppy.begin();
  guppy.startBackgroundServices();
}

void loop() {
  guppy.motorDrive(5.0, 5.0);
  delay(5000);
  guppy.motorDrive(0, 0);
  delay(5000);
  guppy.motorDrive(-5.0, -5.0);
  delay(5000);
  guppy.motorDrive(0, 0);
  delay(5000);
}