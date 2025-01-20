#include <Ticker.h>
#include <esp_task_wdt.h>

#include "config.h"
#include "main.h"
#include "common.h"
#include "display.h"
#include "clock.h"
#include "time.h"
#include "weather.h"
#include "touch.h"

#define EVERY_SECOND     1000
#define EVERY_MINUTE     EVERY_SECOND * 60
#define EVERY_10_MINUTES EVERY_MINUTE * 10

Ticker displayTicker;
unsigned long prevEpoch;
unsigned long lastNTPUpdate;

void setup(){
  display_init();

  Serial.begin(115200);
  delay(10);

  Serial1.begin(9600, SERIAL_8N1, D0, D1);

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

  //logStatusMessage(WiFi.localIP().toString());

  displayTicker.attach_ms(30, displayUpdater);

  fetchWeather();

  touch_init();
  Serial.println("NRF initialized.");

}

uint8_t wheelval = 0;

void loop() {

  if (Serial1.available() > 0) {  // Check if data is available from Arduino Nano
    String receivedMessage = Serial1.readStringUntil('\n');  // Read the message from Nano
    Serial.println("Received: " + receivedMessage);  // Print the message to Serial Monitor
  }

  if (WiFi.status() != WL_CONNECTED) {
    WiFi.reconnect();
  }

  // Periodically refresh NTP time
  if ((millis() - lastNTPUpdate) > (1000 * NTP_INTERVAL)) {
    configTime(NTP_OFFSET, 0, NTP_ADDRESS);
    lastNTPUpdate = millis();
  }

  displayWeather();
  updateWeather();

  touch_update();
}

void displayUpdater() {
  if(!getLocalTime(&timeinfo)){
    return;
  }

  unsigned long epoch = mktime(&timeinfo);
  if (epoch != prevEpoch) {
    //displayClock();
    prevEpoch = epoch;
  }
}