#ifndef WEATHER_H
#define WEATHER_H

#include <Arduino.h>

#define OPENWEATHER_URL "http://api.openweathermap.org/data/2.5/weather?q="
#define CITY "Basel"
#define API_KEY "d46fd88f142e7e527c71d423876dd23a"

void fetchWeather();
void displayIcon();
void displayWeather();
void updateWeather();

extern String weatherDescription;
extern float temperature;
extern unsigned long lastWeatherUpdate;

#define WEATHER_UPDATE_INTERVAL 1000 // 1 minute in milliseconds

#endif
