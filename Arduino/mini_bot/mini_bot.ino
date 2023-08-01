/* Sweep

  Serial1.println("AT+CWJAP=SKYPNVF2,FD3c4LEPEAk");
  servo0.attach(18);  // attaches the servo on pin 9 to the servo object
  servo1.attach(19);  // attaches the servo on pin 9 to the servo object
  servo2.attach(3);  // attaches the servo on pin 9 to the servo object
  servo3.attach(2);  // attaches the servo on pin 9 to the servo object
*/

#include <Servo.h>

Servo servo0;  // create servo object to control a servo
Servo servo1;  // create servo object to control a servo
Servo servo2;  // create servo object to control a servo
Servo servo3;  // create servo object to control a servo
// twelve servo objects can be created on most boards


void sendCommand(const char* command, int timeout = 1000) {
  unsigned long startTime = millis(); // Get the current time
  Serial.print("Sending: ");
  Serial.println(command);
  Serial1.println(command);
  Serial.println();


  Serial.println("Recieved: ");

  while (Serial1.available() || (millis() - startTime) < timeout) {
    if (Serial1.available()) {
      char response = Serial1.read();
      Serial.write(response); // Print the response to Serial Monitor
    }
  }
  Serial.println();
  Serial.println("---------------------------");
  Serial.println();
}

void parseServoCommands(char* servoCommands) {
  char* command = strtok(servoCommands, ";");
  while (command != NULL) {

    char servoIndex = command[1]; // Extract the servo index (e.g., '0', '1', '2', '3')
    int servoPosition = atoi(&command[2]); // Extract the servo position
    // Limit the servo position to valid ranges (0 to 180 degrees)
    Serial.print(command);
    Serial.print("   ");
    Serial.print(servoIndex);
    Serial.print("   ");
    Serial.println(servoPosition);
    servoPosition = constrain(servoPosition, 0, 180);

    switch (servoIndex) {
      case '0':
        servo0.write(servoPosition);
        break;
      case '1':
        servo1.write(servoPosition);
        break;
      case '2':
        servo2.write(servoPosition);
        break;
      case '3':
        servo3.write(servoPosition);
        break;
      default:
        break;
    }

    command = strtok(NULL, ";");
  }
}

void parseCommand(const char* command) {
  Serial.println(command);
  // Check the command for servo control and angle
  if (command[0] == 'S') {
    int servoNum = command[1] - '0';
    int angle = atoi(&command[2]);

    // Set the angle of the specified servo
    switch (servoNum) {
      case 0:
        servo0.write(angle);
        break;
      case 1:
        servo1.write(angle);
        break;
      case 2:
        servo2.write(angle);
        break;
      case 3:
        servo3.write(angle);
        break;
      default:
        break;
    }
  }
}

void setup() {

  servo0.attach(18);  // attaches the servo on pin 9 to the servo object
  servo1.attach(19);  // attaches the servo on pin 9 to the servo object
  servo2.attach(3);  // attaches the servo on pin 9 to the servo object
  servo3.attach(2);  // attaches the servo on pin 9 to the servo object
  Serial.begin(9600); // Serial monitor for debugging
  Serial1.begin(115200); // ESP01 baud rate
  delay(2000);

  sendCommand("AT");
  sendCommand("AT+GMR");
  sendCommand("AT+RST", 10000);
  //  sendCommand("AT+RESTORE");


  // Set the Wi-Fi mode to client (station) mode
  sendCommand("AT+CWMODE?");
  sendCommand("AT+CWMODE=1");
  sendCommand("AT+CWMODE?");



  sendCommand("AT+CWLAP", 10000);


  // Connect to your local Wi-Fi network
  sendCommand("AT+CWJAP=\"SKYPNVF2\",\"FD3c4LEP3EAk\"", 10000);


  sendCommand("AT+CIPMUX=1");   // Enable multiple connections
  sendCommand("AT+CIPSERVER=1");   // Start the TCP server on port 8888
  sendCommand("AT+CIPDINFO=1");   // Start the TCP server on port 8888



  // Get the ESP01's IP address
  sendCommand("AT+CIFSR");

}



void loop() {
  // Check if there is any command received from the ESP01
  if (Serial1.available()) {
    String command = Serial1.readStringUntil('\n');
    command.trim();
    int colonIndex = command.indexOf(':');
    if (colonIndex != -1) {
      // Add 1 to the colon index to exclude the colon itself
      String resultString = command.substring(colonIndex + 1);
      char c[50];
      resultString.toCharArray(c, sizeof(c));
      Serial.println(c);
      parseServoCommands(c);
    }

    // Add your main loop logic here
  }
}
