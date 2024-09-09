#include "TFT_SPI.h"

#include "SPI.h"

TFT_SPI::TFT_SPI(TFT_SPI_Driver *driver)
{
  _driver = driver;
}

TFT_SPI::~TFT_SPI()
{
}

void TFT_SPI::begin(void)
{
  if (_driver == nullptr) {
    return;
  }

  // spi init
  SPI.begin();

  // TFT init
  _driver->init(&SPI);
}

void TFT_SPI::end(void)
{
  if (_driver == nullptr) {
    return;
  }

  _driver->end();
}

void TFT_SPI::rotate(int16_t angle)
{
  if (_driver == nullptr) {
    return;
  }

  _driver->rotate(angle);
}

void TFT_SPI::drawPixel(int32_t x, int32_t y, uint32_t color)
{
  _driver->draw(x, y, x, y, &color);
}

int16_t TFT_SPI::width(void)
{
  return _driver->width();
}

int16_t TFT_SPI::height(void)
{
  return _driver->height();
}

uint32_t TFT_SPI::readPixel(int32_t x, int32_t y)
{
  return 0;
}

void TFT_SPI::fillScreen(uint32_t color)
{
  _driver->fillScreen(color);
}

void TFT_SPI::drawArea(int32_t xb, int32_t yb, int32_t xe, int32_t ye, uint32_t *color)
{
  _driver->draw(xb, yb, xe, ye, color);
}

void TFT_SPI::lvDrawArea(int32_t xb, int32_t yb, int32_t xe, int32_t ye, uint32_t color)
{
  _driver->lv_draw(xb, yb, xe, ye, color);
}
