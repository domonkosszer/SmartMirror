#ifndef DISPLAY_H
#define DISPLAY_H

#include "config.h"

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

// pin mapping for Arduino Nano ESP32
#define R1_PIN        4 //A3
#define G1_PIN        3 //A2
#define B1_PIN       11 //A4
#define R2_PIN       14 //A7
#define G2_PIN       13 //A6
#define B2_PIN       12 //A5
#define A_PIN        17 //D8
#define B_PIN        18 //D9
#define C_PIN         9 //D6
#define D_PIN        10 //D7
#define E_PIN        21 //D10
#define LAT_PIN       8 //D5
#define OE_PIN        7 //D4
#define CLK_PIN       6 //D3

#define NUM_LEDS PANEL_WIDTH*PANEL_HEIGHT

uint16_t colorWheel(uint8_t pos);
void display_init();

#endif
