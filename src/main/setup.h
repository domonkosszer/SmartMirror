#ifndef SETUP_H
#define SETUP_H

#include "config.h"
#include "display.h"

#include <WiFi.h>
#include <NTPClient.h>

#define WIFI_SSID "YOUR_SSID"
#define WIFI_PASSWORD "YOUR_PASSWORD"

extern int status;

//Time of last status update
extern unsigned long lastStatusSend;

// NTP
extern bool wifiFirstConnected;

extern bool syncEventTriggered;

//RGB display
extern MatrixPanel_I2S_DMA *dma_display;

extern struct tm timeinfo;

//Flags to trigger display section updates
extern bool clockStartingUp;

#endif
