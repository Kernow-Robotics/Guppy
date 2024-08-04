#include <Guppy.h>

Guppy guppy;

uint8_t address[6] = "00001";

void setup() {
  guppy.begin();
  guppy.initRadio();
  guppy.startListening(address);
}

void loop() {
  if (guppy.radio.available()) {
    char text[32] = { 0 };
    guppy.radio.read(&text, sizeof(text));
    Serial.print("Received: ");
    Serial.println(text);
    String message = String(text);
    if (message == "light_on") {
      guppy.lightOn();
    }
    if (message == "light_off") {
      guppy.lightOff();
    }
  }
}