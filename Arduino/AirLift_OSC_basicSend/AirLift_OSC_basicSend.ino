//////////WIFI/////////
#include <SPI.h>
#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>  /// https://github.com/CNMAT/OSC
#include <OSCBundle.h>  /// https://github.com/CNMAT/OSC

////WIFI CONFIG////////
int status = WL_IDLE_STATUS;
#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
const IPAddress outIp(SECRET_IP);     // remote IP of your computer
const unsigned int outPort = SECRET_OUTPORT;          // remote port to receive OSC
unsigned int localPort = 8888;      // local port to listen on

int keyIndex = 0;            // your network key Index number (needed only for WEP)
/**************************************************************************/

WiFiUDP Udp;

/**************************************************************************/

long sendCount = 0;
long frameCount = 0;
void sendBundleViaOSC();
int data = 0;
/**************************************************************************/

// Configure the pins used for the ESP32 connection
#if defined(TEENSYDUINO) 
  #define SPIWIFI       SPI  // The SPI port
  #define SPIWIFI_SS     5   // Chip select pin
  #define ESP32_RESETN   6   // Reset pin
  #define SPIWIFI_ACK    9   // a.k.a BUSY or READY pin
  #define ESP32_GPIO0   -1
  
#elif !defined(SPIWIFI_SS)  // if the wifi definition isnt in the board variant
  // Don't change the names of these #define's! they match the variant ones
  #define SPIWIFI     SPI
  #define SPIWIFI_SS    10  // Chip select pin
  #define SPIWIFI_ACK   7   // a.k.a BUSY or READY pin
  #define ESP32_RESETN  5   // Reset pin
  #define ESP32_GPIO0   -1  // Not connected
#endif

void setup() {
  // put your setup code here, to run once:
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
 WiFi.setPins(SPIWIFI_SS, SPIWIFI_ACK, ESP32_RESETN, ESP32_GPIO0, &SPIWIFI);
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
}

void loop() {
  // put your main code here, to run repeatedly:
  //increment sendcount

  sendCount ++;
  //if sendCount is over 1000 (1 second) send OSC info
  if (sendCount > 1000)
  {

    sendViaOSC();//send messages

    //sendBundleViaOSC()  //send bundle
    Serial.println("sendingdata");

  }
}

void sendViaOSC() {
  data++;
  //text
  OSCMessage msg("/string");
  msg.add("hello, osc.");
  msg.add((int32_t)data);

  //analog pin
  //  msg.add("/analogData");
  //  msg.add((int32_t)analogRead(A0));

  //digital pin
  //  msg.add("/d12");
  //  msg.add((int32_t)digitalRead(12));

  //send out 
  Udp.beginPacket(outIp, outPort);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty();
  sendCount = 0;
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
