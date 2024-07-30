#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Guppy.h>

// Create an RF24 object
RF24 radio(19, 1); // CE, CSN pins

// Address for communication
const byte address[6] = "00001";

void setup() {
  SPI.setRX(0);
  SPI.setCS(1);
  SPI.setSCK(2);
  SPI.setTX(3);
  Serial.begin(115200);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}
// Failed to parse "/Users/stitch/Documents/Arduino/libraries/Guppy/.vscode/c_cpp_properties.json": Unexpected token "
void loop() {
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    Serial.print("Received: ");
    Serial.println(text);
  }
}