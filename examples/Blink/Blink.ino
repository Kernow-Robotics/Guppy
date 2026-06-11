#include <Guppy.h>

Guppy guppy;

void setup() {
  guppy.begin();
  
  pinMode(20, OUTPUT);
}

void loop() {
  digitalWrite(20, HIGH);
  delay(1000);
  digitalWrite(20, LOW);
  delay(1000);
}
