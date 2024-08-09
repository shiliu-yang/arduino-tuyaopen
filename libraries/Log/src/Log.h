#pragma once

#include "Arduino.h"

extern "C" {
#include "tal_log.h"
}

namespace arduino {

#define OPEN_SDK_LOG_BUFFER_SIZE 1024

class LogClass {
public:
  enum LogLevel {
    ERROR, WARN, NOTICE, INFO, DEBUG, TRACE
  };

  LogClass();
  ~LogClass();

  void setLevel(LogLevel level);
  void setColor(bool enable);
};

} // namespace arduino

extern arduino::LogClass Log;
