#include "config.h"
#include "common.h"
#include "display.h"
#include "weather.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Weather variables
String weatherDescription;
float temperature;
unsigned long lastWeatherUpdate = 0;

// Function prototypes
void fetchWeather();
void displayWeather();

// Fetch weather data from OpenWeatherMap
void fetchWeather() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String weatherURL = String(OPENWEATHER_URL) + CITY + "&units=metric&appid=" + API_KEY;

    http.begin(weatherURL);
    int httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
      String payload = http.getString();
      Serial.println(payload);

      // Parse JSON
      JsonDocument doc;
      DeserializationError error = deserializeJson(doc, payload);

      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
      } else {
        weatherDescription = doc["weather"][0]["description"].as<String>();
        temperature = doc["main"]["temp"].as<float>();

        Serial.println("Weather: " + weatherDescription);
        Serial.println("Temperature: " + String(temperature) + " °C");

        lastWeatherUpdate = millis();
      }
    } else {
      Serial.println("Failed to fetch weather data. HTTP response code: " + String(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("WiFi not connected.");
  }
}

// Display weather on the RGB matrix
void displayWeather() {
  int centerX = PANEL_WIDTH / 2; // Horizontal center
  int centerY = PANEL_HEIGHT / 2; // Vertical center

  // Clear previous weather data
  dma_display->fillRect(0, centerY - 8, PANEL_WIDTH, 16, 0);

  // Display temperature (centered)
  String tempString = String(temperature, 1) + " C";
  int tempWidth = tempString.length() * 7; // Approximate width of the text
  dma_display->setCursor(centerX - tempWidth / 2, centerY - 8);
  dma_display->setTextColor(dma_display->color565(255, 255, 255));
  dma_display->print(tempString);

  // Display weather description (centered)
  String description = weatherDescription.substring(0, 10); // Truncate if necessary
  int descWidth = description.length() * 6;
  dma_display->setCursor(centerX - descWidth / 2, centerY);
  dma_display->print(description);
}


void updateWeather() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastWeatherUpdate >= WEATHER_UPDATE_INTERVAL) {
    fetchWeather();
    lastWeatherUpdate = currentMillis; // Ensure proper interval tracking
    displayWeather(); // Update display immediately after fetching
  }
}
