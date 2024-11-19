#include <Guppy.h>

Guppy guppy;

uint8_t myAddress[6] = "00002";

void setup() {
  guppy.begin();
  guppy.startBackgroundServices();
  guppy.initRadio();
  guppy.startListening(myAddress);
}

void loop() {
  String message = "";
  message = guppy.receive();
  if (message == "light_on")
  {
    guppy.lightOn();
  }
  else if (message == "light_off")
  {
    guppy.lightOff();
  }
}