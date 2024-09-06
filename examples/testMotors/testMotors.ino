#include <Guppy.h>

Guppy guppy;

void setup() {

}

void loop() {
  guppy.motorDrive(5.0, 5.0);
  delay(1000);
  guppy.motorDrive(0, 0);
  delay(1000);
  guppy.motorDrive(-5.0, -5.0);
  delay(1000);
  guppy.motorDrive(0, 0);
  delay(1000);
}