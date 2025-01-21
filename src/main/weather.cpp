#include "config.h"
#include "setup.h"
#include "display.h"
#include "weather.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Weather variables
float temperature;
int humidity;
String weatherState;
unsigned long lastWeatherUpdate = 0;

void fetchWeather();
void displayWeather();

void fetchWeather() {
  const int maxRetries = 5; //Max retry attempts
  int retryCount = 0;
  bool success = false;

  while (retryCount < maxRetries && !success) {
    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String weatherURL = String(OPENWEATHER_URL) + CITY + "&units=metric&appid=" + API_KEY;

      http.begin(weatherURL);
      int httpResponseCode = http.GET();

      if (httpResponseCode == 200) {
        String payload = http.getString();
        Serial.println(payload);

        StaticJsonDocument<1024> doc;
        DeserializationError error = deserializeJson(doc, payload);

        if (error) {
          Serial.print(F("deserializeJson() failed: "));
          Serial.println(error.c_str());
          retryCount++;
          delay(2000); // 2 seconds until retry
        } else {
          temperature = doc["main"]["temp"].as<float>();
          humidity = doc["main"]["humidity"].as<int>();
          weatherState = doc["weather"][0]["description"].as<String>();

          Serial.println("Temperature: " + String(temperature) + " °C");
          Serial.println("Humidity: " + String(humidity) + " %");
          Serial.println("Weather State: " + weatherState);

          // Update display
          displayWeather();

          success = true; // Data successfully fetched and parsed
        }
      } else {
        Serial.println("Failed to fetch weather data. HTTP response code: " + String(httpResponseCode));
        retryCount++;
        delay(2000); // 2 seconds until retry
      }
      http.end();
    } else {
      retryCount++;
    }
  }

  if (!success) {
    Serial.println("Failed to fetch weather data after " + String(maxRetries) + " attempts.");
  }
}

void displayWeather() {
  int centerX = PANEL_WIDTH / 2; 
  int centerY = PANEL_HEIGHT / 2;

  dma_display->fillScreen(0);

  // location on the top (centred)
  String locationString = CITY;
  int locationWidth = locationString.length() * 6; 
  dma_display->setCursor(centerX - locationWidth / 2, 5); // Top center
  dma_display->setTextColor(dma_display->color565(255, 255, 255));
  dma_display->print(locationString);

  // temp and humidity on the same row
  String tempString = String(temperature, 1) + " C";
  int tempWidth = tempString.length() * 6;
  dma_display->setCursor(centerX - tempWidth / 2, centerY - 9);
  dma_display->print(tempString);

  String HumidityString = String(humidity) + " %";
  int humidityWidth = HumidityString.length() * 6;
  dma_display->setCursor(centerX - humidityWidth / 2, centerY + 1);
  dma_display->print(HumidityString);

  // weatherstate on the bottom center
  String weatherStateString = weatherState;
  int weatherStateWidth = weatherStateString.length() * 6;
  dma_display->setCursor(centerX - weatherStateWidth / 2, PANEL_HEIGHT - 16); // Bottom center
  dma_display->print(weatherStateString);
}

void updateWeather() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastWeatherUpdate >= WEATHER_UPDATE_INTERVAL) {
    fetchWeather();
    lastWeatherUpdate = currentMillis; // Ensure proper interval tracking
  }
}