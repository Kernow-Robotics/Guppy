#include <Guppy.h>

Guppy guppy;

void setup() {
  guppy.begin();
}

void loop() {
  // Get the voltage of the battery
  float vbatt = guppy.updateVbatt();

  // Convert the voltage into the state of charge (SOC) measured in percentage
  int stateOfCharge = guppy.voltageToSOC(vbatt);

  // Print this information to the user
  Serial.print("The battery voltage is ");
  Serial.print(vbatt);
  Serial.print("V which is ");
  Serial.print(stateOfCharge);
  Serial.print("% charged");
  Serial.println();
  delay(1000);
}