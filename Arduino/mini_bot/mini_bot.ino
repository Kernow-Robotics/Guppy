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

String recieved;
int timenow = 0;
bool debug = false;


void myprint(String message = "") {
  Serial.print(millis());
  Serial.print("    ");
  Serial.print(millis() - timenow);
  Serial.print("    ");
  Serial.println(message);
}

String getline() {
  String r = "";
  if (Serial1.available()) {
    //    Serial.print("reading:");
    r = Serial1.readStringUntil('\n');
    r.trim();
    if (debug == true) {
      myprint(r);
    }
  }
  return r;
}

void parseServoCommands(String servoCommands) {
  int pos = 0;
  while ((pos = servoCommands.indexOf(";")) != -1) {
    String token = servoCommands.substring(0, pos);
    //    Serial.println(token);
    char servoIndex = token[1]; // Extract the servo index (e.g., '0', '1', '2', '3')
    int servoPosition = token.substring(2).toInt(); // Extract the servo position
    if (servoPosition > 180) {
      servoPosition = 180;
    }
    if (servoPosition < 0) {
      servoPosition = 0;
    }
    //    Serial.println(servoIndex);
    //    Serial.println(servoPosition);
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
    servoCommands = servoCommands.substring(pos + 1);
  }
}

bool processline(String message) {
  if (message.length() != 0) {
    //----------------DATA PACKET CHECK
    if (message.substring(0, 12) == "+CIPRECVDATA") {
      myprint("Data Message triggered");
      int colonIndex = message.indexOf(':');
      if (colonIndex != -1) {
        message = message.substring(colonIndex + 1);
        Serial.println(message);
        parseServoCommands(message);
      }
    }
    //----------------DATA PRESENT CHECK TCP
    //    if (message.substring(0, 4) == "+IPD") {
    //      myprint("Data present!");
    //      //        int colonIndex = r.indexOf(':');
    //      //        if (colonIndex != -1) {
    //      //          r = r.substring(colonIndex + 1);
    //      sendit("AT+CIPRECVDATA=0,20", 1000, true);
    //      //          parseServoCommands(r);
    //      //        }
    //  }
    //----------------DATA PRESENT CHECK UDP
    if (message.substring(0, 4) == "+IPD") {
      //      myprint("Data present!");
      int colonIndex = message.indexOf(':');
      if (colonIndex != -1) {
        message = message.substring(colonIndex + 1);
        //        sendit("AT+CIPRECVDATA=0,20", 1000, true);
        parseServoCommands(message);
        //        }
      }
    }
    //----------------MESSAGE TERMINATOR CHECK
    if (message == "OK" || message == "ERROR" || message == "ready") {
      return false;
    }
    //--------------------------------
  }
  return true;
}




//void parseCommand(const char* recieved) {
//  if (recieved[0] == 'S') {
//    int servoNum = recieved[1] - '0';
//    int angle = atoi(&recieved[2]);
//    switch (servoNum) {
//      case 0:
//        servo0.write(angle);
//        break;
//      case 1:
//        servo1.write(angle);
//        break;
//      case 2:
//        servo2.write(angle);
//        break;
//      case 3:
//        servo3.write(angle);
//        break;
//      default:
//        break;
//    }
//  }
//}

void sendit(const char* message, int timeout = 1000) {
  unsigned long startTime = millis(); // Get the current time
  //  Serial.println(message);
  Serial1.println(message);
  bool finished = false;
  while (((millis() - startTime) < timeout) && (finished == false)) {
    recieved = getline();
    if (processline(recieved) == false) {
      break;
    }
    delay(1);
  }
  if (debug == true) {
    myprint("----------------");
  }
}

void setup() {
  debug = false;
  servo0.attach(18);  // attaches the servo on pin 9 to the servo object
  servo1.attach(19);  // attaches the servo on pin 9 to the servo object
  servo2.attach(3);  // attaches the servo on pin 9 to the servo object
  servo3.attach(2);  // attaches the servo on pin 9 to the servo object
  Serial.begin(115200); // Serial monitor for debugging
  Serial1.begin(115200); // ESP01 baud rate
  delay(2000);
  if (debug == true) {
    myprint("CONNECTING TO WIFI");
  }
  //  sendit("+++");

  //
  //---------------------------------WIFI INITIALISATION-----------------------------
  sendit("AT");
  sendit("AT+GMR");
  //  sendit("AT+RST", 10000);
  sendit("AT+RESTORE", 10000);
  //  sendit("AT+CWMODE?");
  sendit("AT+CWMODE=1");// Set the Wi-Fi mode to client (station) mode
  //  sendit("AT+CWMODE?");
  //  sendit("AT+CWLAP", 10000);
  sendit("AT+CWJAP=\"SKYPNVF2\",\"FD3c4LEP3EAk\"", 10000);
  //  sendit("AT+CIPFWVER?",60000);
  //  sendit("AT+CIUPDATE",60000);

  //---------------------------------ICP/IP INITIALISATION-----------------------------
  sendit("AT+CIPMUX=1");   // Enable multiple connections
  sendit("AT+CIPSTART=1,\"UDP\",\"0.0.0.0\",333,333,2");

  //  sendit("AT+CIPSERVER=1");    // Start tcp/ip server
  //  sendit("AT+CIPMUX=1");

  //  sendit("AT+CIPDINFO=1");
  //  sendit("AT+CIPSTO=0");    // Set timeout to 0 seconds
  //  sendit("AT+CIPTCPOPT=5,-1,1,0,0");
  //  sendit("AT+CIPTCPOPT?");


  sendit("AT+CIPRECVMODE=1");    // Set passive recieve mode
  //  sendit("AT+CIPMODE=1");
  //  sendit("AT+CIPSEND=64");   // Start the TCP server on default port
  //---------------------------------PRINT IP ADDRESS-----------------------------

  sendit("AT+CIFSR");    // Checks the IP address of the esp
  if (debug) {
    myprint("DONE");
  }
  debug = false;
}

void loop() {
  delay(1);
  timenow = millis();
  //  myprint("looping");
  //  String x = getline(true);
  bool x = processline(getline());

  //  delay(100);
  //  if (Serial1.available()) {
  //    //        Serial.println("looping");
  //    String recieved = "";
  //    recieved = readline();
  //    if (recieved != "") {
  //      int colonIndex = recieved.indexOf(':');
  //      if (colonIndex != -1) {
  //        String resultString = recieved.substring(colonIndex + 1);
  //        char c[50];
  //        resultString.toCharArray(c, sizeof(c));
  //        parseServoCommands(c);
  //      }
  //    }
  //  }
  //  Serial.print("looping ");
  //  Serial.println(millis());
  //  delay(1);
}
