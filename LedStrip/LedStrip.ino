/*******************************************************************
    A project for controlling a 12V LED strip using an ESP8266

    Features:
    - LED strip can be toggled using the rotary encoder button
    - The LED strip brightness can be adjusted using a rotary encoder

    Main Hardware:
    - Wemos D1 Mini (or any ESP8266)
    - 12V LED strip
    - IRLB8721 Mosfet
    - Rotary Encoder

    Written by Brian Lough
    YoutTube: https://www.youtube.com/channel/UCezJOfu7OtqGzd5xrP3q6WA
 *******************************************************************/


// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <Encoder.h>
// For interfacing with the rotary encoder
// Available on the library manager (Search for "Encoder")
// https://github.com/PaulStoffregen/Encoder


#define LED_STRIP D2

// Rotary Encoder Pins
#define RE_SW D5
#define RE_DT D6
#define RE_CLK D7


Encoder myEnc(RE_DT, RE_CLK);

int pushButtonCoolDownDue = 0;
int brightness = 200;
boolean isLedsOn = false;

long oldPosition  = -999;

void setup() {

  Serial.begin(115200);

  pinMode(LED_STRIP, OUTPUT);
  pinMode(RE_SW, INPUT_PULLUP);
}

// the loop function runs over and over again forever
void loop() {

  int time = millis();
  if (time > pushButtonCoolDownDue) {
    if (digitalRead(RE_SW) == LOW) {
      if (isLedsOn) {
        isLedsOn = false;
        analogWrite(LED_STRIP, 0);
        digitalWrite(LED_STRIP, LOW);
      } else {
        isLedsOn = true;
        analogWrite(LED_STRIP, brightness);
      }

      pushButtonCoolDownDue = time + 500;
    }
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
      if (brightness < 0)
      {
        brightness = 0;
      }
    }

    oldPosition = newPosition;
    analogWrite(LED_STRIP, brightness);
  }
}
