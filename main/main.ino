#include <Ticker.h>

#include "config.h"
#include "main.h"
#include "common.h"
#include "display.h"
#include "clock.h"
#include "time.h"
#include "weather.h"
#include "touch.h"

Ticker displayTicker;
unsigned long prevEpoch;
unsigned long lastNTPUpdate;

bool isClock = true;
bool isWeather = false;

void setup(){
  Serial.begin(115200);
  delay(10);

  display_init();
  Serial.println("Display initialized.");

  touch_init();
  Serial.println("NRF initialized.");

  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");

  configTime(NTP_OFFSET, 0, NTP_ADDRESS);
  lastNTPUpdate = millis();

  displayTicker.attach_ms(30, displayUpdater);
}

uint8_t wheelval = 0;

void loop() {

  updateTouch();

  if (WiFi.status() != WL_CONNECTED) {
    WiFi.reconnect();
  }

  // Periodically refresh NTP time
  if ((millis() - lastNTPUpdate) > (1000 * NTP_INTERVAL)) {
    configTime(NTP_OFFSET, 0, NTP_ADDRESS);
    lastNTPUpdate = millis();
  }
}

void displayUpdater() {
  if (isClock) {
    if(!getLocalTime(&timeinfo)){
      return;
    }

    unsigned long epoch = mktime(&timeinfo);
    if (epoch != prevEpoch) {
      displayClock();
      prevEpoch = epoch;
    }
  } 
  if (isWeather) {
    displayWeather();
    updateWeather();
  }
}