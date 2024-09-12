#include <Guppy.h>

Guppy guppy;

uint8_t address[6] = "00001";

void setup() {
  guppy.begin();
  guppy.initRadio();
  guppy.startListening(address);
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