#ifndef ARDUINO_H
#define ARDUINO_H

#include "api/ArduinoAPI.h"
#include "PinsArduino.h"

#if defined(__cplusplus) && !defined(c_plusplus)

using namespace arduino;

#include "SerialUART.h"
#define Serial _SerialUART0_

#endif // __cplusplus

#endif // ARDUINO_H
