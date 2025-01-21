#include "touch.h"
#include "display.h"
#include "weather.h"
#include "clock.h"
#include "setup.h"

#include <Arduino.h>
#include <RF24.h>
#include <SPI.h>

RF24 radio(NRF_CE_PIN, NRF_CSN_PIN);

unsigned long lastReceivedTime = 0;
unsigned long startTouchTime = 0;
unsigned long lastTouchTime = 0;
const byte address[6] = "00001";

void touch_init() {
  /* 
  // with NRF
  radio.begin(NRF_CE_PIN, NRF_CSN_PIN);  // initialise NRF24L01
  radio.setChannel(5);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.openReadingPipe(0, address);
  radio.startListening();
  */

  Serial1.begin(9600, SERIAL_8N1, D0, D1);
}

void updateTouch() { 
  static long lastTime = 0;
  static int lastX = -1, lastY = -1;
  static int deltaX = 0, deltaY = 0;

  if (Serial1.available() > 0) {
    String receivedMessage = Serial1.readStringUntil('\n');
    //Serial.println("Received: " + receivedMessage); //for debugging


    // Filter for "x:" or "y:"
    if (receivedMessage.startsWith("x:") || receivedMessage.startsWith("y:")) {
      int value = receivedMessage.substring(2).toInt();
      long currentTime = millis();

      if (receivedMessage.startsWith("x:")) {
        if (lastX != -1) {
          deltaX += value - lastX;
        }
        lastX = value;
      } else if (receivedMessage.startsWith("y:")) {
        if (lastY != -1) {
          deltaY += value - lastY;
        }
        lastY = value;
      }

      if (currentTime - lastTime > 900) {
        if (abs(deltaX) > 15) {
          if (deltaX > 0) {
            Serial.println("Swipe Right");
          } else {
            Serial.println("Swipe Left");
          }
          deltaX = 0; // Reset after detection
        }

        if (abs(deltaY) > 15) {
          Serial.println(deltaY);
          if (deltaY > 0) {
            Serial.println("Swipe Down");
            isClock = false;
            delay(100);
            emptydisplay();
            delay(100);
            isWeather = true;
          } else {
            Serial.println("Swipe Up");
            isWeather = false;
            delay(100);
            emptydisplay();
            delay(100);
            clockStartingUp = true;
            isClock = true;
          }
          deltaY = 0; // Reset after detection
        }

        lastTime = currentTime;
      }
    }
  }
}
