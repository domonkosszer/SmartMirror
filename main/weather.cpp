#include "config.h"
#include "common.h"
#include "display.h"
#include "weather.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Weather variables
float temperature;
int humidity;
unsigned long lastWeatherUpdate = 0;

// Function prototypes
void fetchWeather();
void displayWeather();
void displayIcon(const String &iconCode);

// Fetch weather data from OpenWeatherMap
void fetchWeather() {
  const int maxRetries = 5; // Maximum number of retry attempts
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
          delay(2000); // Wait 2 seconds before retrying
        } else {
          // Extract temperature and humidity
          temperature = doc["main"]["temp"].as<float>();
          humidity = doc["main"]["humidity"].as<int>();

          // Extract weather icon code
          String iconCode = doc["weather"][0]["icon"].as<String>();
          Serial.println("Icon Code: " + iconCode);

          Serial.println("Temperature: " + String(temperature) + " °C");
          Serial.println("Humidity: " + String(humidity) + " %");

          // Update display
          displayIcon(iconCode);
          displayWeather();

          success = true; // Data successfully fetched and parsed
        }
      } else {
        Serial.println("Failed to fetch weather data. HTTP response code: " + String(httpResponseCode));
        retryCount++;
        delay(2000); // Wait 2 seconds before retrying
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


// Display weather information on the RGB matrix
void displayWeather() {
  int centerX = PANEL_WIDTH / 2; // Horizontal center
  int centerY = PANEL_HEIGHT / 2; // Vertical center

  // Clear previous weather data
  dma_display->fillRect(0, centerY - 16, PANEL_WIDTH, 32, 0);

  // Display temperature (centered)
  String tempString = String(temperature, 1) + " C";
  int tempWidth = tempString.length() * 7; // Approximate width of the text
  dma_display->setCursor(centerX - tempWidth / 2, centerY - 8);
  dma_display->setTextColor(dma_display->color565(255, 255, 255));
  dma_display->print(tempString);

  // Display humidity (centered below temperature)
  String humidityString = String(humidity) + " %";
  int humidityWidth = humidityString.length() * 6;
  dma_display->setCursor(centerX - humidityWidth / 2, centerY + 8);
  dma_display->print(humidityString);
}

// Display weather icon based on icon code
void displayIcon(const String &iconCode) {
  const uint8_t clearSkyIcon[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  };

  const uint8_t rainIcon[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  };

  const uint8_t cloudsIcon[] = {
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  };

  const uint8_t *icon = nullptr;

  // Match icon code to corresponding bitmap
  if (iconCode == "01d" || iconCode == "01n") {
    icon = clearSkyIcon;
  } else if (iconCode.startsWith("02") || iconCode.startsWith("03") || iconCode.startsWith("04")) {
    icon = cloudsIcon;
  } else if (iconCode.startsWith("09") || iconCode.startsWith("10")) {
    icon = rainIcon;
  }

  // Display the icon if bitmap is available
  if (icon != nullptr) {
    dma_display->drawBitmap(10, 10, icon, 32, 32, dma_display->color565(255, 255, 255));
  } else {
    Serial.println("No icon available for code: " + iconCode);
  }
}

void updateWeather() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastWeatherUpdate >= WEATHER_UPDATE_INTERVAL) {
    fetchWeather();
    lastWeatherUpdate = currentMillis; // Ensure proper interval tracking
  }
}
