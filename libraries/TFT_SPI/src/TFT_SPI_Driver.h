#ifndef __TFT_SPI_DRIVER_H__
#define __TFT_SPI_DRIVER_H__

#include "Arduino.h"
#include "SPI.h"

class TFT_SPI_Driver
{
private:
    /* data */
public:
    virtual void init(SPIClassTuyaOpen *spi) = 0;

    virtual void rotate(int16_t angle) = 0;

    virtual void drawPixel(int32_t x, int32_t y, uint32_t color) = 0;
    virtual uint32_t readPixel(int32_t x, int32_t y) = 0;

    virtual int16_t width(void) = 0;
    virtual int16_t height(void) = 0;
    virtual void fillScreen(uint32_t color) = 0;
};

#endif
