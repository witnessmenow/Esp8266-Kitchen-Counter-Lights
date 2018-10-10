/*******************************************************************
    A project for controlling a 12V LED strip using an ESP8266

    Features:
    - LED strip can be toggled using the rotary encoder button
    - The LED strip brightness can be adjusted using a rotary encoder
    - LED strip can be controlled by Webpage
    - Can be controlled by Alexa

    Main Hardware:
    - Wemos D1 Mini (or any ESP8266)
    - 12V LED strip
    - IRLB8721 Mosfet
    - Rotary Encoder

    Written by Brian Lough
 *******************************************************************/

#include <ESP8266WiFi.h>

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <Encoder.h>
// For interfacing with the rotary encoder
// Available on the library manager (Search for "Encoder")
// https://github.com/PaulStoffregen/Encoder

#include "fauxmoESP.h"

#include "secret.h"


const char* ssid = SSID_MAIN;
const char* password = WIFI_PASSWORD;

// prototypes
boolean connectWifi();

#define LED_STRIP D2

// Rotary Encoder Pins
#define RE_SW D5
#define RE_DT D6
#define RE_CLK D7

ESP8266WebServer server(80);

Encoder myEnc(RE_DT, RE_CLK);

fauxmoESP fauxmo;

int pushButtonCoolDownDue = 0;
int brightness = 200;
boolean isLedsOn = false;

long oldPosition  = -999;

const char *webpage =
#include "webPage.h"
  ;

void handleRoot() {
  server.send(200, "text/html", webpage);
}

void handleBrightness() {

  Serial.println("Handle Brightness");
  for (uint8_t i=0; i<server.args(); i++){
    if(server.argName(i) == "level") {
      brightness = server.arg(i).toInt();
      Serial.print("Setting brtightness to: ");
      Serial.println(brightness);
    }
  }
  server.send(200, "text/html", "Set Brightness");
}

void setup() {

  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS Responder Started");
  }

  server.on("/", handleRoot);

  server.on("/toggle", []() {
    Serial.println("got toggle");
    toggleLed();
    server.send(200, "text/plain", "toggled");
  });

  server.on("/brightness", handleBrightness);

  pinMode(LED_STRIP, OUTPUT);
  pinMode(RE_SW, INPUT_PULLUP);

      // You can enable or disable the library at any moment
    // Disabling it will prevent the devices from being discovered and switched
    fauxmo.enable(true);

    // Add virtual devices
    fauxmo.addDevice("switch one");

    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state) {
        Serial.printf("[MAIN] Device #%d (%s) state: %s\n", device_id, device_name, state ? "ON" : "OFF");
        toggleLed();
    });

    // Callback to retrieve current state (for GetBinaryState queries)
    fauxmo.onGetState([](unsigned char device_id, const char * device_name) {
        return true;
    });

  server.begin();
}

void toggleLed() {
  if (isLedsOn) {
    isLedsOn = false;
    analogWrite(LED_STRIP, 0);
    digitalWrite(LED_STRIP, LOW);
  } else {
    isLedsOn = true;
    analogWrite(LED_STRIP, brightness);
  }
}

// the loop function runs over and over again forever
void loop() {
  fauxmo.handle();
  server.handleClient();

  int time = millis();
  if (time > pushButtonCoolDownDue) {
    if (digitalRead(RE_SW) == LOW) {
      toggleLed();
      pushButtonCoolDownDue = time + 500;
    }

    if (isLedsOn) {
      long newPosition = myEnc.read();
      if (newPosition > oldPosition) {
        brightness += 10;
        if (brightness > 1023)
        {
          brightness = 1020;
        }
      } else if (newPosition < oldPosition) {
        brightness -= 10;
        if (brightness < 10)
        {
          brightness = 10;
        }
      }

      oldPosition = newPosition;
      analogWrite(LED_STRIP, brightness);
    }
  }
}
