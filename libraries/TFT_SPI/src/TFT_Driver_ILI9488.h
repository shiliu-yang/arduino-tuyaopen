#pragma once

#include "Arduino.h"
#include "TFT_SPI_Driver.h"

extern "C" {
#include "tal_memory.h"
#include "tal_log.h"
}

#define SPI_FREQ 30000000

#define TFT_DRIVER_ID_ILI9488 0

class TFT_Driver_ILI9488 : public TFT_SPI_Driver
{
private:
  uint32_t _driverId = TFT_DRIVER_ID_ILI9488;

  /* pin */
  uint8_t _rstPin = 0;
  uint8_t _dcPin = 0;
  uint8_t _blPin = 0;

  /* data */
  int16_t _width = 0;
  int16_t _hight = 0;

  SPIClassTuyaOpen *_spi;

  void writeCommand(uint8_t cmd) {
    digitalWrite(_dcPin, LOW);
    _spi->transfer(&cmd, 1);
  }

  void writeData(uint8_t data) {
    digitalWrite(_dcPin, HIGH);
    _spi->transfer(&data, 1);
  }

  void writeData(uint8_t* data, size_t size) {
    digitalWrite(_dcPin, HIGH);
    _spi->transfer(data, size);
  }

  void setCursor(int16_t x, int16_t y) {
    _spi->beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));

    writeCommand(0x2A); // Column Address Set
    writeData(x >> 8);
    writeData(x);
    writeData(_width >> 8);
    writeData(_width);

    writeCommand(0x2B); // Row Address Set
    writeData(y >> 8);
    writeData(y);
    writeData(_hight >> 8);
    writeData(_hight);

    _spi->endTransaction();
  }

  void setWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    uint8_t xb[4] = {0, 0, 0, 0};
    uint8_t yb[4] = {0, 0, 0, 0};

    xb[0] = x0 >> 8;
    xb[1] = x0;
    xb[2] = x1 >> 8;
    xb[3] = x1;

    yb[0] = y0 >> 8;
    yb[1] = y0;
    yb[2] = y1 >> 8;
    yb[3] = y1;

    _spi->beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));

    writeCommand(0x2A); // Column Address Set
    writeData(xb, 4);

    writeCommand(0x2B); // Row Address Set
    writeData(yb, 4);

    _spi->endTransaction();
  }

public:
  TFT_Driver_ILI9488(int16_t w, int16_t h, uint8_t rst_pin, uint8_t dc_pin, uint8_t bl_pin)
  {
    _width = w;
    _hight = h;
    _rstPin = rst_pin;
    _dcPin = dc_pin;
    _blPin = bl_pin;
  }

  ~TFT_Driver_ILI9488(){}

  void init(SPIClassTuyaOpen *spi) override
  {
    if (spi == nullptr) {
      return;
    }

    _spi = spi;

    pinMode(_rstPin, OUTPUT);
    pinMode(_dcPin, OUTPUT);
    pinMode(_blPin, OUTPUT);

    // TFT init
    digitalWrite(_rstPin, HIGH);
    delay(1);
    digitalWrite(_rstPin, LOW);
    delay(10);
    digitalWrite(_rstPin, HIGH);
    delay(120);

    digitalWrite(_blPin, HIGH);
    delay(10);

    _spi->beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));

    writeCommand(0xE0); // Positive Gamma Control
    writeData(0x00);
    writeData(0x03);
    writeData(0x09);
    writeData(0x08);
    writeData(0x16);
    writeData(0x0A);
    writeData(0x3F);
    writeData(0x78);
    writeData(0x4C);
    writeData(0x09);
    writeData(0x0A);
    writeData(0x08);
    writeData(0x16);
    writeData(0x1A);
    writeData(0x0F);

    writeCommand(0XE1); // Negative Gamma Control
    writeData(0x00);
    writeData(0x16);
    writeData(0x19);
    writeData(0x03);
    writeData(0x0F);
    writeData(0x05);
    writeData(0x32);
    writeData(0x45);
    writeData(0x46);
    writeData(0x04);
    writeData(0x0E);
    writeData(0x0D);
    writeData(0x35);
    writeData(0x37);
    writeData(0x0F);

    writeCommand(0XC0); // Power Control 1
    writeData(0x17);
    writeData(0x15);

    writeCommand(0xC1); // Power Control 2
    writeData(0x41);

    writeCommand(0xC5); // VCOM Control
    writeData(0x00);
    writeData(0x12);
    writeData(0x80);

    writeCommand(0x36); // Memory Access Control
    writeData(0x48);          // MX, BGR

    writeCommand(0x3A); // Pixel Interface Format
    writeData(0x66);  // 18-bit colour for SPI
    // writeData(0x55);  // 16-bit colour for SPI

    writeCommand(0xB0); // Interface Mode Control
    writeData(0x00);

    writeCommand(0xB1); // Frame Rate Control
    writeData(0xA0);

    writeCommand(0xB4); // Display Inversion Control
    writeData(0x02);

    writeCommand(0xB6); // Display Function Control
    writeData(0x02);
    writeData(0x02);
    writeData(0x3B);

    writeCommand(0xB7); // Entry Mode Set
    writeData(0xC6);

    writeCommand(0xF7); // Adjust Control 3
    writeData(0xA9);
    writeData(0x51);
    writeData(0x2C);
    writeData(0x82);

    writeCommand(0x11);  //Exit Sleep
    delay(120);

    writeCommand(0x29);  //Display on
    delay(25);

    _spi->endTransaction();
  }

  void end() override {
    digitalWrite(_rstPin, LOW);
    digitalWrite(_dcPin, LOW);
    digitalWrite(_blPin, LOW);

    _spi->end();
  }

  uint32_t readId(void) override { return _driverId; }

  void rotate(int16_t angle) override {
    _spi->beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));

    writeCommand(0x36); // Adjust Control 3
    switch (angle) {
      case 0:
        writeData((1<<3)|(0<<7)|(0<<6)|(0<<5));
        break;
      case 90:
        writeData((0<<3)|(0<<7)|(0<<6)|(1<<5));
        break;
      case 180:
        writeData((0<<3)|(1<<7)|(1<<6)|(0<<5));
        break;
      case 270:
        writeData((0<<3)|(1<<7)|(0<<6)|(1<<5));
        break;
      default:
        break;
    }

    _spi->endTransaction();
  }

  uint32_t readPixel(int32_t x, int32_t y) override { return 0; };

  int16_t width(void) override { return _width; };
  int16_t height(void) override { return _hight; };

  void draw(int32_t xb, int32_t yb, int32_t xe, int32_t ye, uint32_t *color) override {
    uint8_t pBuf[3] = {0};

    setWindow(xb, yb, xe, ye);

    _spi->beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));

    writeCommand(0x2C);

    for (int i = 0; i < (xe - xb + 1) * (ye - yb + 1); i++) {
      pBuf[0] = (color[i] >> 16) & 0xfc;
      pBuf[1] = (color[i] >> 8) & 0xfc;
      pBuf[2] = color[i] & 0xfc;

      writeData(pBuf, 3);
    }

    _spi->endTransaction();
  }

  void lv_draw(int32_t xb, int32_t yb, int32_t xe, int32_t ye, uint32_t color) override {
    uint8_t pBuf[3] = {0};

    setWindow(xb, yb, xe, ye);

    pBuf[0] = (color >> 16) & 0xfc;
    pBuf[1] = (color >> 8) & 0xfc;
    pBuf[2] = color & 0xfc;

    _spi->beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));

    writeCommand(0x2C);

    for (int i = 0; i < (xe - xb + 1) * (ye - yb + 1); i++) {
      writeData(pBuf, 3);
    }

    _spi->endTransaction();
  }

  void fillScreen(uint32_t color) override {
    uint8_t pBuf[3] = {0};

    setWindow(0, 0, _width - 1, _hight - 1);

    _spi->beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));

    writeCommand(0x2C);

    for (int i = 0; i < _width * _hight; i++) {
      pBuf[0] = (color >> 16) & 0xfc;
      pBuf[1] = (color >> 8) & 0xfc;
      pBuf[2] = color & 0xfc;

      writeData(pBuf, 3);
    }

    _spi->endTransaction();
  }
};
