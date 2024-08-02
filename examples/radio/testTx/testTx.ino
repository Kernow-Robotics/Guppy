#include <SPI.h>
#include <RF24.h>
#include <Guppy.h>

Guppy guppy;

// CE and CSN pins for the NRF24L01 module
// #define CE_PIN 19
// #define CSN_PIN 1

// Create an RF24 object
RF24 radio(19, 1);

// Address for the communication
const byte addresses[][6] = { "N1", "N2", "N3" };

void radioSend(String message, const byte address[6]) {
  radio.openWritingPipe(address);
  char text[32];
  message.toCharArray(text, sizeof(text));
  // const char text[] = "Hello, world!";
  radio.write(&text, sizeof(text));
  Serial.print("Sent: ");
  Serial.println(text);
}

void setup() {
  SPI.setRX(0);  // MISO
  SPI.setCS(1);
  SPI.setSCK(2);
  SPI.setTX(3);  // MOSI
  Serial.begin(115200);
  radio.begin();
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
}

void loop() {
  // String message = Serial.readStringUntil('\n');
  // radioSend(message);
  guppy.lightOn();
  for (int i = 0; i < 3; i++) {
    radioSend("light_on", addresses[i]);
  }
  delay(200);
  guppy.lightOff();
  for (int i = 0; i < 3; i++) {
    radioSend("light_off", addresses[i]);
  }
  delay(1000);
}