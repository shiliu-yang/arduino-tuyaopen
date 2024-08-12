#ifndef __TUYA_IOT_H__
#define __TUYA_IOT_H__

/******************************************************************************
 * INCLUDE
 ******************************************************************************/
#include <Arduino.h>

extern "C" {
#include "tuya_cloud_types.h"
#include "tuya_iot.h"
}

/******************************************************************************
 * CONSTANTS
 ******************************************************************************/


/******************************************************************************
 * TYPEDEF
 ******************************************************************************/


/******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/
class TuyaIoTCloudClass {
public:
  TuyaIoTCloudClass();
  ~TuyaIoTCloudClass();

  void begin(const char *pid, const char *version);
  void begin(const char *pid, const char *version, const char *uuid, const char *authKey);

  void setLicense(const char *uuid, const char *authKey);
  void setLogLevel(TAL_LOG_LEVEL_E level);

  void setEventCallback(void (*callback)(tuya_event_msg_t* event));

  // obj dp write
  int objWrite(uint8_t dpid, void* value, TIME_T timestamp = 0);

  int write(uint8_t dpid, String value, TIME_T timestamp = 0) {
    return objWrite(dpid, reinterpret_cast<void*>(const_cast<char*>(value.c_str())), timestamp);
  }

  template <typename T>
  int write(uint8_t dpid, T value, TIME_T timestamp = 0) {
    return objWrite(dpid, static_cast<void *>(&value), timestamp);
  }

  // raw dp write
  int rawWrite(uint8_t dpid, uint8_t* value, uint16_t len, uint32_t timeout = 3);

  int write(uint8_t dpid, uint8_t* value, uint16_t len, uint32_t timeout = 3) {
    return rawWrite(dpid, value, len, timeout);
  }

  // event
  tuya_event_id_t getEventId(tuya_event_msg_t* event);
  uint16_t getEventDpNum(tuya_event_msg_t* event);
  uint8_t getEventDpId(tuya_event_msg_t* event, uint16_t index);

  bool readBool(tuya_event_msg_t* event, uint8_t dpid);
  int readValue(tuya_event_msg_t* event, uint8_t dpid);
  uint32_t readEnum(tuya_event_msg_t* event, uint8_t dpid);
  char* readString(tuya_event_msg_t* event, uint8_t dpid);
  uint32_t readBitmap(tuya_event_msg_t* event, uint8_t dpid);
  uint8_t* readRaw(tuya_event_msg_t* event, uint8_t dpid, uint16_t* len);

  int remove(void);
private:
  char _pid[MAX_LENGTH_PRODUCT_ID+1]  = {0}; // +1 is '\0'
  char _version[MAX_LENGTH_SW_VER+1]  = {0};
  char _uuid[MAX_LENGTH_UUID+1]       = {0};
  char _authKey[MAX_LENGTH_AUTHKEY+1] = {0};
};


/******************************************************************************
 * EXTERN DECLARATION
 ******************************************************************************/
extern TuyaIoTCloudClass TuyaIoT;

#endif // __TUYA_IOT_H__
