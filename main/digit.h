#ifndef DIGIT_H
#define DIGIT_H

#include "display.h"

#include <Arduino.h>

class Digit {
  
  public:
    Digit(byte value, uint16_t xo, uint16_t yo, uint16_t color);
    void Draw(byte value);
    void Morph(byte newValue);
    void MorphDigits(byte fromDigit, byte toDigit);
    byte Value();
    void DrawColon(uint16_t c);
    
  private:
    byte _value;
    uint16_t _color;
    uint16_t xOffset;
    uint16_t yOffset;
    int animSpeed = 100;

    void drawPixel(uint16_t x, uint16_t y, uint16_t c);
    void drawFillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t c);
    void drawLine(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, uint16_t c);
    void drawSeg(byte seg);
    void clearSeg(byte seg);
};

#endif
