#include <Guppy.h>

Guppy guppy;

void setup() {

}

void loop() {
  guppy.motorDrive(255, 255);
  delay(1000);
  guppy.motorDrive(0, 0);
  delay(1000);
  guppy.motorDrive(-255, -255);
  delay(1000);
  guppy.motorDrive(0, 0);
  delay(1000);
}