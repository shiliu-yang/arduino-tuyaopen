#include <TFT_SPI.h>
#include "TFT_Driver_ILI9488.h"

#include <Log.h>
#include <Ticker.h>

extern "C" {
#include "tal_memory.h"
}

#define TY_LCD_WIDTH      240
#define TY_LCD_HEIGHT     240

// tft pins
#ifdef ARDUINO_TUYA_T2
#define RST_PIN       p28
#define DC_PIN        p9
#define BL_PIN        p24
#endif

TFT_Driver_ILI9488 TFT_ILI9488(TY_LCD_WIDTH, TY_LCD_HEIGHT, RST_PIN, DC_PIN, BL_PIN);
TFT_SPI tft = TFT_SPI(&TFT_ILI9488);

Ticker heapTicker;

void heapCallback() {
  PR_NOTICE("Free heap: %d", tal_system_get_free_heap_size());
}

void setup(void) {
  Serial.begin(115200);

  tft.begin();
  tft.rotate(0);
  tft.fillScreen(0x000000);

  heapTicker.attach(5, heapCallback);
}

void loop() {
  Serial.println("hello");

  tft.fillScreen(0xff0000);
  delay(1000);
  tft.fillScreen(0x00ff00);
  delay(1000);
  tft.fillScreen(0x0000ff);
  delay(1000);
}
