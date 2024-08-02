#include <SPI.h>
#include <RF24.h>
#include <Guppy.h>

Guppy guppy;

// Create an RF24 object
RF24 radio(19, 1);

// Address for the communication
const byte address[6] = "N2"; // Change this for each node

void setup() {
  guppy.heartbeat();
  SPI.setRX(0);  // MISO
  SPI.setCS(1);
  SPI.setSCK(2);
  SPI.setTX(3);  // MOSI
  Serial.begin(115200);
  //   SPI.set
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();
}

void loop() {
  if (radio.available()) {
    char text[32] = { 0 };
    radio.read(&text, sizeof(text));
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