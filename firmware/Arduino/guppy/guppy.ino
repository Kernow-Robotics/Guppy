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

bool debug = true;
String recieved;
String ipAddress;

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

void debugPrint(String message = "") {
  if (debug) {
    Serial.print(millis());
    //    Serial.print("    ");
    //    Serial.print(millis() - timenow);
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
    debugPrint(r);
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
      debugPrint("Data Message triggered");
      int colonIndex = message.indexOf(':');
      if (colonIndex != -1) {
        message = message.substring(colonIndex + 1);
        debugPrint(message);
        parseServoCommands(message);
      }
    }
    //----------------DATA PRESENT CHECK TCP
    //    if (message.substring(0, 4) == "+IPD") {
    //      debugPrint("Data present!");
    //      //        int colonIndex = r.indexOf(':');
    //      //        if (colonIndex != -1) {
    //      //          r = r.substring(colonIndex + 1);
    //      sendit("AT+CIPRECVDATA=0,20", 1000, true);
    //      //          parseServoCommands(r);
    //      //        }
    //  }
    //----------------DATA PRESENT CHECK UDP
    if (message.substring(0, 4) == "+IPD") {
      //      debugPrint("Data present!");
      int colonIndex = message.indexOf(':');
      if (colonIndex != -1) {
        message = message.substring(colonIndex + 1);
        //        sendit("AT+CIPRECVDATA=0,20", 1000, true);
        parseServoCommands(message);
        //        }
      }
    }
    //----------------LOCAL IP ADDRESS CHECK
    if (message.substring(0, 12) == "+CIFSR:STAIP") {
      int startQuote = message.indexOf("\"");  // Find the first double quote
      int endQuote = message.indexOf("\"", startQuote + 1);  // Find the second double quote

      if (startQuote != -1 && endQuote != -1) {
        // Extract the text between double quotes
        ipAddress = message.substring(startQuote + 1, endQuote);
      } else {
        // Return an empty string if no quotes are found
        ipAddress = "";
      }
    }

    //----------------MESSAGE TERMINATOR CHECK
    if (message == "OK" || message == "ERROR" || message == "ready" || message == "FAIL" || message == "SEND OK") {
      return false;
    }
    //--------------------------------
  }
  return true;
}

void sendit(String message, int timeout = 10000) {
  unsigned long startTime = millis(); // Get the current time
  //  Serial.println(message);
  Serial1.println(message);
  while ((millis() - startTime) < timeout) {
    recieved = getline();
    if (processline(recieved) == false) {
      break;
    }
    delay(1);
  }
  debugPrint("----------------");
}

void initWiFi(String ssid, String password) {
  Serial1.begin(115200); // ESP01 baud rate
  delay(500);
  debugPrint("CONNECTING TO WIFI");
  //---------------------------------WIFI INITIALISATION-----------------------------
//  sendit("AT");                                                 //
//  delay(1000);
//  sendit("AT+GMR");                                             //
//  delay(1000);
//  sendit("AT+RESTORE");                                  //
//  delay(1000);
  sendit("AT+CWMODE=1");                                        // Set the Wi-Fi mode to client (station) mode
  delay(1000);
  sendit("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"", 20000);
  //---------------------------------ICP/IP OR UDP INITIALISATION-----------------------------
  sendit("AT+CIPMUX=1");                                        // Enable multiple connections
  //  sendit("AT+CIPSTART=1,\"UDP\",\"0.0.0.0\",666,333,2");
//  sendit("AT+CIPSTART=4,\"UDP\",\"239.255.255.250\",1900,1900,0");
  sendit("AT+CIPSTART=4,\"UDP\",\"255.255.255.255\",12345,333,0");
  
  sendit("AT+CIPRECVMODE=1");                                   // Set passive recieve mode
  //---------------------------------PRINT IP ADDRESS-----------------------------
  sendit("AT+CIFSR");                                           // Checks the IP address of the esp
  //---------------------------------UNUSED COMMANDS-----------------------------
  //  sendit("AT+RST", 10000);
  //  sendit("AT+CWMODE?");
  //  sendit("AT+CWMODE?");
  //  sendit("AT+CWLAP", 10000);
  //  sendit("AT+CIPFWVER?",60000);
  //  sendit("AT+CIUPDATE", 60000);
  //  sendit("AT+CIPSERVER=1");    // Start tcp/ip server
  //  sendit("AT+CIPMUX=1");
  //  sendit("AT+CIPDINFO=1");
  //  sendit("AT+CIPSTO=0");    // Set timeout to 0 seconds
  //  sendit("AT+CIPTCPOPT=5,-1,1,0,0");
  //  sendit("AT+CIPTCPOPT?");
  //  sendit("AT+CIPMODE=1");
  //  sendit("AT+CIPSEND=64");   // Start the TCP server on default port
}

void sendSSDP() {
  // SSDP message
  //  debugPrint("first");
  String ssdpMessage = "NOTIFY * HTTP/1.1\r\n";
  ssdpMessage += "HOST: 239.255.255.250:1900\r\n";
  ssdpMessage += "CACHE-CONTROL: max-age=100\r\n";
  ssdpMessage += "LOCATION: http://192.168.0.15/description.xml\r\n";
  ssdpMessage += "SERVER: Arduino/1.0 UPnP/1.1\r\n";
  ssdpMessage += "NT: urn:schemas-upnp-org:device:Basic:1\r\n";
  ssdpMessage += "NTS: ssdp:alive\r\n";
  ssdpMessage += "USN: uuid:12345678-1234-5678-1234-56789abcdef0::urn:schemas-upnp-org:device:Basic:1\r\n";

  //  debugPrint("second");

  String initial = "AT+CIPSEND=4," + String(ssdpMessage.length() + 2); // Send the SSDP message
  //  debugPrint(initial);
  sendit(initial);
  //  debugPrint("third");

  String body = ssdpMessage;
  debugPrint(ssdpMessage);

  sendit(body);

  sendit("AT+CIPCLOSE");
  //  debugPrint("fourth");

}

void pingNetwork(int x) {
//  sendit("AT+CIPSTART=4,\"UDP\",\"255.255.255.255\",12345,333,0");
//  String announcement = "Hello from Guppy 1.3 " + ipAddress + " " + String(millis());
  String announcement = "Hello from Guppy 1.3 " + ipAddress + " " + String(x);
  sendit("AT+CIPSEND=4," + String(announcement.length() + 2));
  sendit(announcement);
//  sendit("AT+CIPCLOSE");
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
  initWiFi("SKYPNVF2", "FD3c4LEP3EAk");
//  initWiFi("Robotics Lab", "killallhumans");
//  initWiFi("Ben\'s iPhone", "aaabbbccc1");
  debugPrint("READY TO ROBOT");
  delay(3000);
  //  debug = false;

}

int startTime = 0;
int lastStartTime = 0;
int x = 1;
void loop() {
  
  pingNetwork(x);
  x++;
//  delay(100);

  //  sendSSDP();
  //  startTime = millis();
  //  delay(5);
  ////  Serial.println("loop");
  //  String incommingData=getline();
  //  bool x = processline(incommingData);

}
