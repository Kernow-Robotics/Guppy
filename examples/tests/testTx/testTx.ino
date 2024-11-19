#include <Guppy.h>

Guppy guppy;

uint8_t myAddress[6] = "00001";

uint8_t addresses[][6] = { "00002", "00003", "00004" };

void setup() {
  guppy.begin();
  guppy.initRadio();
  guppy.startBackgroundServices();
}

void loop() {
  guppy.lightOn();
  for (int i = 0; i < 3; i++) {
    guppy.send("light_on", addresses[i]);
  }
  delay(1000);

  guppy.lightOff();
  for (int i = 0; i < 3; i++) {
    guppy.send("light_off", addresses[i]);
  }
  delay(1000);
}