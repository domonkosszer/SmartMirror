#ifndef TOUCH_H
#define TOUCH_H

#include <RF24.h>

#define HSPI_SCK     48  D13
#define HSPI_MISO    38  D12
#define HSPI_MOSI    47  D11
// Define the NRF pins
#define NRF_CE_PIN   A0   //A0
#define NRF_CSN_PIN  A1   //A1

// Define touch variables
extern RF24 radio;
extern unsigned long lastReceivedTime;

// Function prototypes
void touch_init();
void touch_update();

#endif
