#include "setup.h"
#include "digit.h"

const byte sA = 0;
const byte sB = 1;
const byte sC = 2;
const byte sD = 3;
const byte sE = 4;
const byte sF = 5;
const byte sG = 6;
const int segHeight = CLOCK_SEGMENT_HEIGHT;
const int segWidth = CLOCK_SEGMENT_WIDTH;
const uint16_t height = PANEL_HEIGHT - 1;
const uint16_t width = PANEL_WIDTH - 1;

byte digitBits[] = {
    0b11111100, // 0 ABCDEF-
    0b01100000, // 1 -BC----
    0b11011010, // 2 AB-DE-G
    0b11110010, // 3 ABCD--G
    0b01100110, // 4 -BC--FG
    0b10110110, // 5 A-CD-FG
    0b10111110, // 6 A-CDEFG
    0b11100000, // 7 ABC----
    0b11111110, // 8 ABCDEFG
    0b11110110  // 9 ABCD_FG
};

uint16_t black;

Digit::Digit(byte value, uint16_t xo, uint16_t yo, uint16_t color) {
    _value = value;
    xOffset = xo;
    yOffset = yo;
    _color = color;
}

byte Digit::Value() {
    return _value;
}

void Digit::drawPixel(uint16_t x, uint16_t y, uint16_t c) {
    dma_display->drawPixel(xOffset + x, height - (y + yOffset), c);
}

void Digit::drawLine(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, uint16_t c) {
    dma_display->drawLine(xOffset + x, height - (y + yOffset), xOffset + x2, height - (y2 + yOffset), c);
}

void Digit::drawFillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t c) {
    dma_display->fillRect(xOffset + x, height - (y + yOffset), w, h, c);
}

void Digit::DrawColon(uint16_t c) {
  drawFillRect(-2-(int)(CLOCK_SEGMENT_SPACING/2), segHeight-1, 2,2, c);
  drawFillRect(-2-(int)(CLOCK_SEGMENT_SPACING/2), segHeight+1+3, 2,2, c);
}

void Digit::drawSeg(byte seg) {
    switch (seg) {
        case sA: drawLine(1, segHeight * 2 + 2, segWidth, segHeight * 2 + 2, _color); break;
        case sB: drawLine(segWidth + 1, segHeight * 2 + 1, segWidth + 1, segHeight + 2, _color); break;
        case sC: drawLine(segWidth + 1, 1, segWidth + 1, segHeight, _color); break;
        case sD: drawLine(1, 0, segWidth, 0, _color); break;
        case sE: drawLine(0, 1, 0, segHeight, _color); break;
        case sF: drawLine(0, segHeight * 2 + 1, 0, segHeight + 2, _color); break;
        case sG: drawLine(1, segHeight + 1, segWidth, segHeight + 1, _color); break;
    }
}

void Digit::clearSeg(byte seg) {
    switch (seg) {
        case sA: drawLine(1, segHeight * 2 + 2, segWidth, segHeight * 2 + 2, black); break;
        case sB: drawLine(segWidth + 1, segHeight * 2 + 1, segWidth + 1, segHeight + 2, black); break;
        case sC: drawLine(segWidth + 1, 1, segWidth + 1, segHeight, black); break;
        case sD: drawLine(1, 0, segWidth, 0, black); break;
        case sE: drawLine(0, 1, 0, segHeight, black); break;
        case sF: drawLine(0, segHeight * 2 + 1, 0, segHeight + 2, black); break;
        case sG: drawLine(1, segHeight + 1, segWidth, segHeight + 1, black); break;
    }
}

void Digit::Draw(byte value) {
    byte pattern = digitBits[value];
    for (int seg = 0; seg < 7; ++seg) {
        if (bitRead(pattern, 7 - seg)) {
            drawSeg(seg);
        }
    }
    _value = value;
}

void Digit::MorphDigits(byte fromDigit, byte toDigit) {
  byte fromPattern = digitBits[fromDigit];
  byte toPattern = digitBits[toDigit];

  for (int seg = 0; seg < 7; ++seg) {
      bool fromActive = bitRead(fromPattern, 7 - seg);
      bool toActive = bitRead(toPattern, 7 - seg);

      if (fromActive && !toActive) {
          // Turn segment off
          clearSeg(seg);
          delay(animSpeed);
      } else if (!fromActive && toActive) {
          // Turn segment on
          drawSeg(seg);
          delay(animSpeed);
      }
  }

  _value = toDigit; // Update to new digit value
}

void Digit::Morph(byte newValue) {
  if (newValue != _value) {
    MorphDigits(_value, newValue);
  }
}
