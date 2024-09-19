#pragma once

#include "Arduino.h"
#include "TFT_SPI_Driver.h"

extern "C" {
#include "tal_memory.h"
#include "tal_log.h"
}

#define SPI_FREQ (25000000UL)

#define TFT_DRIVER_ID_ST7789 1

class TFT_Driver_ST7789 : public TFT_SPI_Driver
{
private:
  uint32_t _driverId = TFT_DRIVER_ID_ST7789;

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
  TFT_Driver_ST7789(int16_t w, int16_t h, uint8_t rst_pin, uint8_t dc_pin, uint8_t bl_pin)
  {
    _width = w;
    _hight = h;
    _rstPin = rst_pin;
    _dcPin = dc_pin;
    _blPin = bl_pin;
  }

  ~TFT_Driver_ST7789(){}

  void init(SPIClassTuyaOpen *spi) override
  {
    if (spi == nullptr) {
      return;
    }

    _spi = spi;

    pinMode(_rstPin, OUTPUT);
    digitalWrite(_rstPin, HIGH);

    pinMode(_dcPin, OUTPUT);
    digitalWrite(_dcPin, HIGH);

    pinMode(_blPin, OUTPUT);
    digitalWrite(_blPin, LOW);

    delay(100);

    digitalWrite(_rstPin, LOW);
    delay(20);
    digitalWrite(_rstPin, HIGH);
    delay(100);

#if 1
    _spi->beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));

    writeCommand(0x11);
    delay(120);

    writeCommand(0x13);

    writeCommand(0x36);
    writeData(0x00);

    writeCommand(0xB6);
    writeData(0x0A);
    writeData(0x82);

    writeCommand(0xB0);
    writeData(0x00);
    writeData(0xE0); // 5 to 6-bit conversion: r0 = r5, b0 = b5

    writeCommand(0x3A);
    writeData(0x55);
    delay(10);

    writeCommand(0xB2);
    writeData(0x0c);
    writeData(0x0c);
    writeData(0x00);
    writeData(0x33);
    writeData(0x33);

    writeCommand(0xB7);
    writeData(0x35);

    writeCommand(0xBB);
    writeData(0x28);

    writeCommand(0xC0);
    writeData(0x0C);

    writeCommand(0xC2);
    writeData(0x01);
    writeData(0xFF);

    writeCommand(0xC3);
    writeData(0x10);

    writeCommand(0xC4);
    writeData(0x20);

    writeCommand(0xC6);
    writeData(0x0f);

    writeCommand(0xD0);
    writeData(0xa4);
    writeData(0xa1);

    writeCommand(0xE0);
    writeData(0xd0);
    writeData(0x00);
    writeData(0x02);
    writeData(0x07);
    writeData(0x0a);
    writeData(0x28);
    writeData(0x32);
    writeData(0x44);
    writeData(0x42);
    writeData(0x06);
    writeData(0x0e);
    writeData(0x12);
    writeData(0x14);
    writeData(0x17);

    writeCommand(0xE1);
    writeData(0xd0);
    writeData(0x00);
    writeData(0x02);
    writeData(0x07);
    writeData(0x0a);
    writeData(0x28);
    writeData(0x31);
    writeData(0x54);
    writeData(0x47);
    writeData(0x0e);
    writeData(0x1c);
    writeData(0x17);
    writeData(0x1b);
    writeData(0x1e);

    writeCommand(0x21);

    writeCommand(0x2A);    // Column address set
    writeData(0x00);
    writeData(0x00);
    writeData(0x00);
    writeData(0xF0);    // 240

    writeCommand(0x2B);    // Row address set
    writeData(0x00);
    writeData(0x00);
    writeData(0x01);
    writeData(0xF0);    // 240

    _spi->endTransaction();
    delay(120);

    _spi->beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));
    writeCommand(0x29);    //Display on
    _spi->endTransaction();
    delay(120);

    digitalWrite(_blPin, HIGH);
#endif

#if 0
    _spi->beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));

    writeCommand(0x11);   // Sleep out
    delay(120);

    writeCommand(0x13);    // Normal display mode on

    //------------------------------display and color format setting--------------------------------//
    writeCommand(0x36);
    writeData(0x00);

  // writeCommand(ST7789_RAMCTRL);
  // writeData(0x00);
  // writeData(0xE0); // 5 to 6-bit conversion: r0 = r5, b0 = b5

    writeCommand(0x3A);
    writeData(0x55);
    delay(10);

    //--------------------------------ST7789V Frame rate setting----------------------------------//
    writeCommand(0xB2);
    writeData(0x0b);
    writeData(0x0b);
    writeData(0x00);
    writeData(0x33);
    writeData(0x33);

    writeCommand(0xB7);      // Voltages: VGH / VGL
    writeData(0x75);

    //---------------------------------ST7789V Power setting--------------------------------------//
    writeCommand(0xBB);
    writeData(0x28);		// JLX240 display datasheet

    writeCommand(0xC0);
    writeData(0x2C);

    writeCommand(0xC2);
    writeData(0x01);

    writeCommand(0xC3);       // voltage VRHS
    writeData(0x1F);

    writeCommand(0xC6);
    writeData(0x13);

    writeCommand(0xD0);
    writeData(0xa7);

    writeCommand(0xD0);
    writeData(0xa4);
    writeData(0xa1);

    writeCommand(0xD6);
    writeData(0xa1);

    //--------------------------------ST7789V gamma setting---------------------------------------//
    writeCommand(0xE0);
    writeData(0xf0);
    writeData(0x05);
    writeData(0x0a);
    writeData(0x06);
    writeData(0x06);
    writeData(0x03);
    writeData(0x2b);
    writeData(0x32);
    writeData(0x43);
    writeData(0x36);
    writeData(0x11);
    writeData(0x10);
    writeData(0x2b);
    writeData(0x32);

    writeCommand(0xE1);
    writeData(0xf0);
    writeData(0x08);
    writeData(0x0c);
    writeData(0x0b);
    writeData(0x09);
    writeData(0x24);
    writeData(0x2b);
    writeData(0x22);
    writeData(0x43);
    writeData(0x38);
    writeData(0x15);
    writeData(0x16);
    writeData(0x2f);
    writeData(0x37);

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    _spi->endTransaction();
    delay(120);
    _spi->beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));

    writeCommand(0x29);    //Display on
    delay(120);
    _spi->endTransaction();

    digitalWrite(_blPin, HIGH); // TODO
#endif

#if 0
    _spi->beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));

    writeCommand(0x01); // Software reset
    delay(120);

    writeCommand(0x11); // Sleep out
    delay(50);

    writeCommand(0x3A); // Interface pixel format
    writeData(0x55);    // Set colour mode to 16 bit
    delay(10);

    writeCommand(0xC5); // VCOM Control
    writeData(0x1a);

    writeCommand(0xb2); // Porch control
    writeData(0x05);
    writeData(0x05);
    writeData(0x00);
    writeData(0x33);
    writeData(0x33);

    writeCommand(0xB7); // Gate control
    writeData(0x05);

    writeCommand(0xBB); // Vcom setting
    writeData(0x3f);

    writeCommand(0xC0); // LCM Control
    writeData(0x2C);

    writeCommand(0xC2); // VDV and VRH command enable
    writeData(0x01);

    writeCommand(0xC3); // VRH Set
    writeData(0x0f);

    writeCommand(0xC4); // VDV Set
    writeData(0x20);

    writeCommand(0xC6); // Frame rate control in normal mode
    writeData(0x01);

    writeCommand(0xD0); // Power control
    writeData(0xA4);
    writeData(0xA1);

    writeCommand(0xE8);
    writeData(0x03);

    writeCommand(0xE9);
    writeData(0x09);
    writeData(0x09);
    writeData(0x08);

    writeCommand(0x36); // Memory Access Control
    writeData(0x00);

    writeCommand(0xe0); // Positive Gamma Control
    writeData(0xd0);
    writeData(0x05);
    writeData(0x09);
    writeData(0x09);
    writeData(0x08);
    writeData(0x14);
    writeData(0x28);
    writeData(0x33);
    writeData(0x3f);
    writeData(0x07);
    writeData(0x13);
    writeData(0x14);
    writeData(0x28);
    writeData(0x30);

    writeCommand(0xe1); // Negative Gamma Control
    writeData(0xd0);
    writeData(0x05);
    writeData(0x09);
    writeData(0x09);
    writeData(0x08);
    writeData(0x03);
    writeData(0x24);
    writeData(0x32);
    writeData(0x32);
    writeData(0x3b);
    writeData(0x14);
    writeData(0x13);
    writeData(0x28);
    writeData(0x2f);

    writeCommand(0x13); // Normal display mode on
    delay(10);

    writeCommand(0x29); // Display on
    delay(50);

    _spi->endTransaction();

    digitalWrite(_blPin, HIGH); // TODO
#endif

#if 1
    _spi->beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));

    writeCommand(0x11);
    delay(120);

    writeCommand(0x3a);
    writeData(0x05);
    writeCommand(0x21);
    writeCommand(0x2a);
    writeData(0x00);
    writeData(0x00);
    writeData(0x00);
    writeData(0xef);
    writeCommand(0x2b);
    writeData(0x00);
    writeData(0x00);
    writeData(0x00);
    writeData(0xef);
    //--------------------------------ST7789V Frame rate setting----------------------------------//
    writeCommand(0xb2);
    writeData(0x0c);
    writeData(0x0c);
    writeData(0x00);
    writeData(0x33);
    writeData(0x33);
    writeCommand(0xb7);
    writeData(0x35);
    //---------------------------------ST7789V Power setting--------------------------------------//
    writeCommand(0xbb);
    writeData(0x1f);
    writeCommand(0xc0);
    writeData(0x2c);
    writeCommand(0xc2);
    writeData(0x01);
    writeCommand(0xc3);
    writeData(0x12);
    writeCommand(0xc4);
    writeData(0x20);
    writeCommand(0xc6);
    writeData(0x0f);
    writeCommand(0xd0);
    writeData(0xa4);
    writeData(0xa1);
    //--------------------------------ST7789V gamma setting--------------------------------------//
    writeCommand(0xe0);
    writeData(0xd0);
    writeData(0x08);
    writeData(0x11);
    writeData(0x08);
    writeData(0x0c);
    writeData(0x15);
    writeData(0x39);
    writeData(0x33);
    writeData(0x50);
    writeData(0x36);
    writeData(0x13);
    writeData(0x14);
    writeData(0x29);
    writeData(0x2d);
    writeCommand(0xe1);
    writeData(0xd0);
    writeData(0x08);
    writeData(0x10);
    writeData(0x08);
    writeData(0x06);
    writeData(0x06);
    writeData(0x39);
    writeData(0x44);
    writeData(0x51);
    writeData(0x0b);
    writeData(0x16);
    writeData(0x14);
    writeData(0x2f);
    writeData(0x31);

    writeCommand(0x2A); //Column Address Set
    writeData(0x00);
    writeData(0x00); //0
    writeData(0x00);
    writeData(0xEF); //239

    writeCommand(0x2B); //Row Address Set
    writeData(0x00);
    writeData(0x00); //0
    writeData(0x00);
    writeData(0xEF); //239

    writeCommand(0x29);	//Display on

    _spi->endTransaction();

    digitalWrite(_blPin, HIGH); // TODO
#endif

#if 0
    _spi->beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));

    writeCommand(0x11);
    delay(480); // todo: 120ms

    writeCommand(0x3A);
    writeData(0x55);

    writeCommand(0x36);
    writeData(0x00);

    writeCommand(0x2A);
    writeData(0x00);
    writeData(0x00);
    writeData(0x00);
    writeData(0xF0);
    writeCommand(0x2B);
    writeData(0x00);
    writeData(0x00);
    writeData(0x00);
    writeData(0xF0);

    writeCommand(0x21);
    delay(10);

    writeCommand(0x13);
    delay(10);

    writeCommand(0x29);
    delay(500);

    writeCommand(0x3C);
    writeData(0x00);

    writeCommand(0x21);
    writeCommand(0x21);

    _spi->endTransaction();

    digitalWrite(_blPin, HIGH); // TODO
#endif

#if 0
    _spi->beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));

    writeCommand(0x11);
    delay(500); // todo: 120ms

    writeCommand(0x3A);
    writeData(0x55);
    delay(10);

    writeCommand(0x36);
    writeData(0x00);

    writeCommand(0x2A);
    writeData(0x00);
    writeData(0x00);
    writeData(0x00);
    writeData(0xF0);
    writeCommand(0x2B);
    writeData(0x00);
    writeData(0x00);
    writeData(0x00);
    writeData(0xF0);

    writeCommand(0x21);
    delay(10);

    writeCommand(0x13);
    delay(10);

    writeCommand(0x29);
    delay(120);

    _spi->endTransaction();

    digitalWrite(_blPin, HIGH); // TODO
#endif

  }

  void end() override {

  }

  uint32_t readId(void) override { return _driverId; }

  void rotate(int16_t angle) override {
    _spi->beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));

    writeCommand(0x36);
    angle = angle % 4;

    switch (angle) {
      case 0:
        writeData(0x00);
        break;
      case 1:
        writeData(0x40 | 0x20 | 0x00);
        break;
      case 2:
        writeData(0x40 | 0x80 | 0x00);
        break;
      case 3:
        writeData(0x20 | 0x80 | 0x00);
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
    
  }

  void lv_draw(int32_t xb, int32_t yb, int32_t xe, int32_t ye, uint16_t *color) override {
    uint8_t pBuf[2] = {0};
    uint8_t r = 0, g = 0, b = 0;

    setWindow(xb, yb, xe, ye);

    _spi->beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));

    writeCommand(0x2C);

    writeData((uint8_t *)(color), (xe - xb + 1) * (ye - yb + 1) * 2);

    _spi->endTransaction();
  }

  void fillScreen(uint32_t color) override {
    uint8_t pBuf[2] = {0};
    uint8_t r = 0, g = 0, b = 0;

    setWindow(0, 0, _width - 1, _hight - 1);

    _spi->beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));

    writeCommand(0x2C);

    for (int i = 0; i < _width * _hight; i++) {
      r = (color >> 16) & 0xff;
      g = (color >> 8) & 0xff;
      b = color & 0xff;

      // 888 -> 565
      pBuf[0] = ((r & 0xf8) | (g >> 5));
      pBuf[1] = (((g & 0x1c) << 3) | (b >> 3));

      writeData(pBuf, 2);
    }

    _spi->endTransaction();
  }
};
