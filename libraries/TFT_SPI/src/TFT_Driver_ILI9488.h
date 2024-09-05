#pragma once

#include "Arduino.h"
#include "TFT_SPI_Driver.h"

extern "C" {
#include "tal_memory.h"
#include "tal_log.h"
}

#define SPI_FREQ 8000000

class TFT_Driver_ILI9488 : public TFT_SPI_Driver
{
private:
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
    delay(10);

#if 0
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
#else
  writeCommand(0XF7);    	
  writeData(0xA9); 	
  writeData(0x51); 	
  writeData(0x2C); 	
  writeData(0x82);

  writeCommand(0XEC);    	
  writeData(0x00); 	
  writeData(0x02); 	
  writeData(0x03); 	
  writeData(0x7A);

  writeCommand(0xC0); 	
  writeData(0x13); 	
  writeData(0x13); 	

  writeCommand(0xC1); 	
  writeData(0x41); 	

  writeCommand(0xC5); 	
  writeData(0x00); 	
  writeData(0x28); 	
  writeData(0x80);

  writeCommand(0xB1);   //Frame rate 70HZ  	
  writeData(0xB0);
  writeData(0x11);	

  writeCommand(0xB4); 	
  writeData(0x02);   	

  writeCommand(0xB6); //RGB/MCU Interface Control	
  writeData(0x02);   //MCU	
  writeData(0x22); 

  writeCommand(0xB7); 	
  writeData(0xc6); 

  writeCommand(0xBE); 	
  writeData(0x00); 
  writeData(0x04);	

  writeCommand(0xE9); 	
  writeData(0x00);

  writeCommand(0xF4); 	
  writeData(0x00); 
  writeData(0x00);
  writeData(0x0f);	

  writeCommand(0xE0); 	
  writeData(0x00); 	
  writeData(0x04); 	
  writeData(0x0E); 	
  writeData(0x08); 	
  writeData(0x17); 	
  writeData(0x0A); 	
  writeData(0x40); 	
  writeData(0x79); 	
  writeData(0x4D); 	
  writeData(0x07); 	
  writeData(0x0E); 	
  writeData(0x0A); 	
  writeData(0x1A); 	
  writeData(0x1D); 	
  writeData(0x0F);  	

  writeCommand(0xE1); 	
  writeData(0x00); 	
  writeData(0x1B); 	
  writeData(0x1F); 	
  writeData(0x02); 	
  writeData(0x10); 	
  writeData(0x05); 	
  writeData(0x32); 	
  writeData(0x34); 	
  writeData(0x43); 	
  writeData(0x02); 	
  writeData(0x0A); 	
  writeData(0x09); 	
  writeData(0x33); 	
  writeData(0x37); 	
  writeData(0x0F); 


  writeCommand(0xF4);
  writeData(0x00);
  writeData(0x00);
  writeData(0x0f);

  writeCommand(0x36); 	
  writeData(0x08); 	

  writeCommand(0x3A);  //Interface Mode Control	
  writeData(0x66);  //0x66 18bit; 0x55 16bit

  writeCommand(0x11);
  delay(120);
  writeCommand(0x29);
  delay(5);
#endif
    _spi->endTransaction();

    rotate(0);
  }

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

  void drawPixel(int32_t x, int32_t y, uint32_t color) override {};
  uint32_t readPixel(int32_t x, int32_t y) override { return 0; };

  int16_t width(void) override { return _width; };
  int16_t height(void) override { return _hight; };

  void fillScreen(uint32_t color) override {
#if 0
    setWindow(0, 0, _width - 1, _hight - 1);

    _spi->beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));

    color = 0x3F000;

    size_t bufSize = _width * _hight * 3;
    uint8_t *pBuf = (uint8_t *)tal_malloc(bufSize);
    if (pBuf == nullptr) {
      PR_DEBUG("malloc fail, bufSize %d", bufSize);
      _spi->endTransaction();
      return;
    }
    memset(pBuf, 0, bufSize);

    for (int i = 0; i < _width * _hight; i++) {
      pBuf[i * 3] = ((color >> 12) & 0x3f) << 2;
      pBuf[i * 3 + 1] = ((color >> 6) & 0x3f) << 2;
      pBuf[i * 3 + 2] = (color & 0x3f) << 2;
    }

    _spi->beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));

    writeCommand(0x2C);
    writeData(pBuf, bufSize);

    _spi->endTransaction();

    tal_free(pBuf);
#else

  color = 0x3F000;

  for (int i=0; i<_hight; i++) {
    setWindow(0, i, _width-1, i);

    size_t bufSize = _width * 3;
    uint8_t *pBuf = (uint8_t *)tal_malloc(bufSize);
    if (pBuf == nullptr) {
      PR_DEBUG("malloc fail, bufSize %d", bufSize);
      _spi->endTransaction();
      return;
    }
    memset(pBuf, 0, bufSize);

    for (int j = 0; j < _width; j++) {
      pBuf[j * 3] = ((color >> 12) & 0x3f) << 2;
      pBuf[j * 3 + 1] = ((color >> 6) & 0x3f) << 2;
      pBuf[j * 3 + 2] = (color & 0x3f) << 2;
    }

    _spi->beginTransaction(SPISettings(SPI_FREQ, MSBFIRST, SPI_MODE0));

    writeCommand(0x2C);
    writeData(pBuf, bufSize);

    _spi->endTransaction();
    tal_free(pBuf);
    pBuf = nullptr;
  }

#endif
  }
};
