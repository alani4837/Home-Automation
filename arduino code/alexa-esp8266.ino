#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <functional>

#include "switch.h"
#include "UpnpBroadcastResponder.h"
#include "CallbackFunction.h"


const char* ssid = "wifi name";
const char* password = "wifi password";

boolean wifiConnected = false;

UpnpBroadcastResponder upnpBroadcastResponder;

Switch *lightOne = NULL;
Switch *lightTwo = NULL;
Switch *lightThree = NULL;
Switch *lightFour = NULL;

int relayOne = 05;  // NodeMCU pin D1
int relayTwo = 04;  // NodeMCU pin D2
int relayThree = 00; // NodeMCU pin D3
int relayFour = 02;  // NodeMCU pin D4

void setup() {
  Serial.begin(115200);
  wifiConnected = connectWifi();
  Serial.print("WiFi Connected");

  if (wifiConnected) {
    upnpBroadcastResponder.beginUdpMulticast();

    lightOne = new Switch("Light One", 80, lightOneOn, lightOneOff);
    lightTwo = new Switch("Light Two", 81, lightTwoOn, lightTwoOff);
    lightThree= new Switch("Light Three", 82, lightThreeOn, lightThreeOff);
    lightFour = new Switch("Light Four", 83, lightFourOn, lightFourOff);

    //Serial.println("Adding switches upnp broadcast responder");
    upnpBroadcastResponder.addDevice(*lightOne);
    upnpBroadcastResponder.addDevice(*lightTwo);
    upnpBroadcastResponder.addDevice(*lightThree);
    upnpBroadcastResponder.addDevice(*lightFour);

    //Set relay pins to outputs
    pinMode(relayOne, OUTPUT);
    pinMode(relayTwo, OUTPUT);
    pinMode(relayThree, OUTPUT);
    pinMode(relayFour, OUTPUT);

    //Set each relay pin to HIGH
    digitalWrite(relayOne, HIGH);   // sets relayOne on
    delay(500);
    digitalWrite(relayTwo, HIGH);   // sets relayOne on
    delay(500);
    digitalWrite(relayThree, HIGH);   // sets relayOne on
    delay(500);
    digitalWrite(relayFour, HIGH);   // sets relayOne on
    delay(500);
  }
}

void loop()
{
  if (wifiConnected) 
  {
    upnpBroadcastResponder.serverLoop();
    lightOne->serverLoop();
    lightTwo->serverLoop();
    lightThree->serverLoop();
    lightFour->serverLoop();
  }
}

void lightOneOn() {
  Serial.print("Switch 1 turn on ...");
  digitalWrite(relayOne, LOW);   // sets relayOne on
}

void lightOneOff() {
  Serial.print("Switch 1 turn off ...");
  digitalWrite(relayOne, HIGH);   // sets relayOne off
}

void lightTwoOn() {
  Serial.print("Switch 2 turn on ...");
  digitalWrite(relayTwo, LOW);   // sets relayTwo on
}

void lightTwoOff() {
  Serial.print("Switch 2 turn off ...");
  digitalWrite(relayTwo, HIGH);   // sets relayTwo Off
}

void lightThreeOn() {
  Serial.print("Switch 1 turn on ...");
  digitalWrite(relayThree, LOW);   // sets relayOne on
}

void lightThreeOff() {
  Serial.print("Switch 1 turn off ...");
  digitalWrite(relayThree, HIGH);   // sets relayOne off
}

void lightFourOn() {
  Serial.print("Switch 2 turn on ...");
  digitalWrite(relayFour, LOW);   // sets relayTwo on
}

void lightFourOff() {
  Serial.print("Switch 2 turn off ...");
  digitalWrite(relayFour, HIGH);   // sets relayTwo Off
}

// connect to wifi – returns true if successful or false if not
boolean connectWifi() {
  boolean state = true;
  int i = 0;

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  Serial.println("Connecting to WiFi");

  // Wait for connection
  Serial.print("Connecting ...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (i > 10) {
      state = false;
      break;
    }
    i++;
  }

  if (state) 
  {
      Serial.println("");
      Serial.print("Connected to ");
      Serial.println(ssid);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
  }
  else 
  {
    Serial.println("");
    Serial.println("Connection failed.");
  }

  return state;
}
