#ifndef TOUCH_H
#define TOUCH_H

#include <RF24.h>

// NRF pins
#define NRF_CE_PIN   A0   //A0
#define NRF_CSN_PIN  A1   //A1

extern bool isClock;
extern bool isWeather;

extern RF24 radio;
extern unsigned long lastReceivedTime;

void touch_init();
void updateTouch();

#endif
