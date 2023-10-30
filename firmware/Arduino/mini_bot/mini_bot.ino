/* Sweep

  Serial1.println("AT+CWJAP=SKYPNVF2,FD3c4LEPEAk");
  servo0.attach(18);  // attaches the servo on pin 9 to the servo object
  servo1.attach(19);  // attaches the servo on pin 9 to the servo object
  servo2.attach(3);  // attaches the servo on pin 9 to the servo object
  servo3.attach(2);  // attaches the servo on pin 9 to the servo object
*/


int pinServo0 = 2;
int pinServo1 = 3;
int pinServo2 = 4;
int pinServo3 = 5;
int pinM0a = 8;
int pinM0b = 9;
int pinM1a = 10;
int pinM1b = 11;

#include <Servo.h>

Servo servo0;
Servo servo1;
Servo servo2;
Servo servo3;

String recieved;
int timenow = 0;
bool debug = false;

void m0Power(int power = 0) {
  Serial.print("m0 power =: ");
  Serial.println(power);
  if (power == 0) {
    digitalWrite(pinM0a, LOW);
    digitalWrite(pinM0b, LOW);
  }
  else if (power > 0) {
    digitalWrite(pinM0b, LOW);
    analogWrite(pinM0a, power);
  }
  else if (power < 0) {
    digitalWrite(pinM0a, LOW);
    analogWrite(pinM0b, abs(power));
  }
}

void m1Power(int power = 0) {
  Serial.print("m1 power =: ");
  Serial.println(power);
  if (power == 0) {
    digitalWrite(pinM1a, LOW);
    digitalWrite(pinM1b, LOW);
  }
  else if (power > 0) {
    digitalWrite(pinM1b, LOW);
    analogWrite(pinM1a, power);
  }
  else if (power < 0) {
    digitalWrite(pinM1a, LOW);
    analogWrite(pinM1b, abs(power));
  }
}

void motorDrive(int power0 = 0, int power1 = 0) {
  m0Power(power0);
  m1Power(power1);
}

void myprint(String message = "") {
  if (debug) {
    Serial.print(millis());
    Serial.print("    ");
    Serial.print(millis() - timenow);
    Serial.print("    ");
    Serial.println(message);
  }
}

String getline() {
  String r = "";
  if (Serial1.available()) {
    //    Serial.print("reading:");
    r = Serial1.readStringUntil('\n');
    r.trim();
    myprint(r);
  }
  return r;
}

void parseServoCommands(String servoCommands) {
  int pos = 0;
  while ((pos = servoCommands.indexOf(";")) != -1) {
    String token = servoCommands.substring(0, pos);
    char servoIndex = token[1]; // Extract the servo index (e.g., '0', '1', '2', '3')
    int servoPosition = token.substring(2).toInt(); // Extract the servo position
    switch (servoIndex) {
      case '0':
        m0Power(servoPosition);
        break;
      case '1':
        m1Power(servoPosition);
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
        myprint(message);
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
  myprint("----------------");
}

void setup() {
  debug = true;
  servo0.attach(pinServo0);  // attaches the servo on pin 9 to the servo object
  servo1.attach(pinServo1);  // attaches the servo on pin 9 to the servo object
  servo2.attach(pinServo2);  // attaches the servo on pin 9 to the servo object
  servo3.attach(pinServo3);  // attaches the servo on pin 9 to the servo object
  pinMode(pinM0a, OUTPUT);
  pinMode(pinM0b, OUTPUT);
  pinMode(pinM1a, OUTPUT);
  pinMode(pinM1b, OUTPUT);
  Serial.begin(115200); // Serial monitor for debugging
  Serial1.begin(115200); // ESP01 baud rate
  delay(2000);
  myprint("CONNECTING TO WIFI");
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
//  sendit("AT+CWJAP=\"SKYPNVF2\",\"FD3c4LEP3EAk\"", 10000);
  sendit("AT+CWJAP=\"Robotics Lab\",\"killallhumans\"", 10000);
  //  sendit("AT+CIPFWVER?",60000);
  //  sendit("AT+CIUPDATE",60000);

  //---------------------------------ICP/IP OR UDP INITIALISATION-----------------------------
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
  myprint("READY TO ROBOT");
  //  debug = false;
}

void loop() {
  delay(5);
  timenow = millis();
  bool x = processline(getline());
//motorDrive(255,255);
//delay(1000);
//motorDrive(0,0);
//delay(1000);
//motorDrive(-255,-255);
//delay(1000);
//motorDrive(0,0);
//delay(1000);

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
