#include <Guppy.h>

Guppy guppy;

void setup() {
  Serial.begin(115200);
}

void loop() {
  float vbatt = guppy.updateVbatt();

  Serial.print("Minimum:");
  Serial.print(3.0);
  Serial.print(",");

  Serial.print("Maximum:");
  Serial.print(4.2);
  Serial.print(",");

  Serial.print("Current:");
  Serial.println(vbatt, 3);
  delay(1000);
}