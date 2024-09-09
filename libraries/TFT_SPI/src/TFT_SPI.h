#ifndef __TFT_SPI_H__
#define __TFT_SPI_H__

#include "Print.h"
#include "TFT_SPI_Driver.h"

#define TFT_BLACK       0x0000      /*   0,   0,   0 */

class TFT_SPI
{
private:
    TFT_SPI_Driver *_driver = nullptr;
public:
    TFT_SPI(TFT_SPI_Driver *driver);
    ~TFT_SPI();

    void begin(void);
    void end(void);

    void rotate(int16_t angle);

    void drawPixel(int32_t x, int32_t y, uint32_t color);
    void drawLine(int32_t xs, int32_t ys, int32_t xe, int32_t ye, uint32_t color);

    void drawArea(int32_t xb, int32_t yb, int32_t xe, int32_t ye, uint32_t *color);
    void lvDrawArea(int32_t xb, int32_t yb, int32_t xe, int32_t ye, uint32_t color);

    int16_t width(void);
    int16_t height(void);

    uint32_t readPixel(int32_t x, int32_t y);

    void fillScreen(uint32_t color);
};




#endif // __TFT_SPI_H__
