#include "config.h"
#include "common.h"
#include "display.h"

// NTP
bool wifiFirstConnected = false;
//Current time
struct tm timeinfo;

// Display
MatrixPanel_I2S_DMA *dma_display = nullptr;

// Flags to trigger display updates
bool clockStartingUp = true;


String epoch2String(unsigned long t) {
  char buf[32];
  struct tm * ts;

  ts = localtime((time_t*)&t);
  strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M:%S", ts);

  return String(buf);
}

String epoch2HHMM(unsigned long t) {
  char buf[6];
  struct tm *ts;

  ts = localtime((time_t*)&t);
  strftime(buf, sizeof(buf), "%H:%M", ts);

  return String(buf);
}
