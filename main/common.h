#ifndef COMMON_H
#define COMMON_H

#include "config.h"
#include "display.h"

#include <WiFi.h>
#include <NTPClient.h>

#define WIFI_SSID "DESKTOP-Q2FKEVH 1018"
#define WIFI_PASSWORD "-1r4809M"

extern int status;

//Time of last status update
extern unsigned long lastStatusSend;

// NTP
extern bool wifiFirstConnected;

extern bool syncEventTriggered; // True if a time event has been triggered

//RGB display
extern MatrixPanel_I2S_DMA *dma_display;

//Current time and date
extern struct tm timeinfo;

//Flags to trigger display section updates
extern bool clockStartingUp;

#endif
