/**
 * @file ArduinoMain.cpp
 * @author www.tuya.com
 * @brief ArduinoMain module is used to
 * @version 0.1
 * @date 2024-08-20
 *
 * @copyright Copyright (c) tuya.inc 2024
 *
 */

extern "C" {
#include "tuya_iot_config.h"

#include "tal_api.h"
}

#include <Arduino.h>

extern "C" void ArduinoMain(void)
{
  setup();

  for (;;) {
    loop();
    tal_system_sleep(1);
  }

  return;
}
