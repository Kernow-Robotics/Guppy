#include <Guppy.h>

Guppy guppy;

uint8_t addresses[][6] = { "00001", "00002", "00003" };

void setup() {
  guppy.begin();
  guppy.initRadio();
}

void loop() {
  guppy.lightOn();
  for (int i = 0; i < 3; i++) {
    guppy.send("light_on", addresses[i]);
  }
  delay(200);

  guppy.lightOff();
  for (int i = 0; i < 3; i++) {
    guppy.send("light_off", addresses[i]);
  }
  delay(1000);
}