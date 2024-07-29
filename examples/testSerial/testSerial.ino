#include "Guppy.h"

Guppy guppy;

void setup() {
  Serial.begin(115200);
}

void loop() {
  guppy.heartbeat();
  float vbatt = guppy.getVbatt();
  Serial.print("Battery Voltage: ");
  Serial.println(vbatt);
  delay(1000);
}