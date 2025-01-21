#include "display.h"
#include "setup.h"

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
// From: https://gist.github.com/davidegironi/3144efdc6d67e5df55438cc3cba613c8
uint16_t colorWheel(uint8_t pos) {
  if(pos < 85) {
    return dma_display->color565(pos * 3, 255 - pos * 3, 0);
  } else if(pos < 170) {
    pos -= 85;
    return dma_display->color565(255 - pos * 3, 0, pos * 3);
  } else {
    pos -= 170;
    return dma_display->color565(0, pos * 3, 255 - pos * 3);
  }
}

uint16_t displayBlack = dma_display->color565(0, 0, 0);

void display_init() {
  HUB75_I2S_CFG::i2s_pins _pins={R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN, A_PIN, B_PIN, C_PIN, D_PIN, E_PIN, LAT_PIN, OE_PIN, CLK_PIN};
  HUB75_I2S_CFG mxconfig(
		PANEL_WIDTH,
		PANEL_HEIGHT,
		1,
		_pins
	);

  mxconfig.driver = HUB75_I2S_CFG::FM6124;
  dma_display = new MatrixPanel_I2S_DMA(mxconfig);

  dma_display->begin();
}

void emptydisplay() {
  dma_display->fillScreen(displayBlack);
}
