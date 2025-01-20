#ifndef CONFIG_H
#define CONFIG_H

// NTP
#define NTP_INTERVAL 3600
#define NTP_ADDRESS "pool.ntp.org"
#define NTP_OFFSET 3600

// Panel dimensions
#define PANEL_WIDTH   64
#define PANEL_HEIGHT  64

// Clock
#define CLOCK_X 0
#define CLOCK_Y 24
#define CLOCK_SEGMENT_HEIGHT 6
#define CLOCK_SEGMENT_WIDTH  6
#define CLOCK_SEGMENT_SPACING 4
#define CLOCK_WIDTH  6*(CLOCK_SEGMENT_WIDTH+CLOCK_SEGMENT_SPACING)+4
#define CLOCK_HEIGHT 2*CLOCK_SEGMENT_HEIGHT+3

// Clock color
#define CLOCK_DIGIT_COLOR  ((0x00 & 0xF8) << 8) | ((0xFF & 0xFC) << 3) | (0xFF >> 3)

// Delay in ms for clock animation
#define CLOCK_ANIMATION_DELAY_MSEC 20

// Day of week
#define DOW_X 0
#define DOW_Y 0

// Day of week color
#define DOW_COLOR ((0x00 & 0xF8) << 8) | ((0x40 & 0xFC) << 3) | (0xFF >> 3)

// Date
#define DATE_X DOW_X
#define DATE_Y DOW_Y+11

// Width and height are for both DATE and DOW
#define DATE_WIDTH  40
#define DATE_HEIGHT 18

#define BITMAP_X 0
#define BITMAP_Y 44

// Date color
#define DATE_COLOR DOW_COLOR

#endif
