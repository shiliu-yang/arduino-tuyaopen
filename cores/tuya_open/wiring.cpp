#include <Arduino.h>

#include "tal_system.h"

#if defined(ARDUINO_TUYA_T2)
#include "FreeRTOS.h"
#include "portmacro.h"
#endif // defined(ARDUINO_TUYA_T2)

unsigned long millis()
{
    unsigned long ms = tal_system_get_millisecond();
    return ms;
}

void delay(unsigned long ms)
{
    tal_system_sleep((uint32_t)ms);
}

void delayMicroseconds(unsigned int us)
{
    unsigned int ms = us / 1000;

    // Note: tuya open sdk not support delayMicroseconds
    ms = (ms == 0) ? 1 : ms;

    tal_system_sleep((uint32_t)ms);
}

void yield(void)
{
#if defined(ARDUINO_TUYA_T2)
    portYIELD();
#endif // defined(ARDUINO_TUYA_T2)
    return;
}
