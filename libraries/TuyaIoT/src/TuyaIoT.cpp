/******************************************************************************
 * INCLUDE
 ******************************************************************************/
#include "TuyaIoT.h"
#include "ArduinoTuyaIoTClient.h"

extern "C" {
#include <string.h>

#include "tuya_iot_dp.h"
}



/******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/


/******************************************************************************
 * LOCAL MODULE FUNCTIONS
 ******************************************************************************/


/******************************************************************************
 * CTOR/DTOR
 ******************************************************************************/
TuyaIoTCloudClass::TuyaIoTCloudClass()
{
  ;
}

TuyaIoTCloudClass::~TuyaIoTCloudClass()
{
  ;
}

/******************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/
void TuyaIoTCloudClass::setEventCallback(void (*callback)(tuya_event_msg_t* event))
{
  app_iot_event_register_cb(callback);
}

void TuyaIoTCloudClass::setLicense(const char *uuid, const char *authKey)
{
  strncpy(this->_uuid, uuid, MAX_LENGTH_UUID);
  strncpy(this->_authKey, authKey, MAX_LENGTH_AUTHKEY);
  app_iot_license_set(this->_uuid, this->_authKey);
}

void TuyaIoTCloudClass::setLogLevel(TAL_LOG_LEVEL_E level)
{
  tal_log_set_level(level);
}

void TuyaIoTCloudClass::begin(const char *pid, const char *version)
{
  strncpy(this->_pid, pid, MAX_LENGTH_PRODUCT_ID);
  strncpy(this->_version, version, MAX_LENGTH_SW_VER);

  app_iot_task_start(this->_pid, this->_version);
}

void TuyaIoTCloudClass::begin(const char *pid, const char *version, const char *uuid, const char *authKey)
{
  strncpy(this->_pid, pid, MAX_LENGTH_PRODUCT_ID);
  strncpy(this->_version, version, MAX_LENGTH_SW_VER);
  strncpy(this->_uuid, uuid, MAX_LENGTH_UUID);
  strncpy(this->_authKey, authKey, MAX_LENGTH_AUTHKEY);

  app_iot_license_set(this->_uuid, this->_authKey);
  app_iot_task_start(this->_pid, this->_version);
}

int TuyaIoTCloudClass::objWrite(uint8_t dpid, void* value, TIME_T timestamp)
{
  dp_obj_t dpObj;

  memset(&dpObj, 0, sizeof(dp_obj_t));

  dp_node_t *dpNode = dp_node_find_by_devid(ArduinoIoTClient.activate.devid, dpid);
  if (dpNode == NULL) {
    PR_ERR("dpid %d not found", dpid);
    return OPRT_COM_ERROR;
  }

  if (T_OBJ != dpNode->desc.type) {
    PR_ERR("dpid %d not object type", dpid);
    return OPRT_NOT_SUPPORTED;
  }

  // dpid
  dpObj.id = dpid;
  // type
  dpObj.type = dpNode->desc.prop_tp;
  // timestamp
  dpObj.time_stamp = timestamp;
  // value
  switch (dpNode->desc.prop_tp) {
    case (PROP_STR) : {
      dpObj.value.dp_str = reinterpret_cast<char *>(value);
    } break;
    case PROP_VALUE:{
      dpObj.value.dp_value = *static_cast<int *>(value);
    } break;
    case PROP_ENUM:{
      dpObj.value.dp_enum = *static_cast<uint32_t *>(value);
    } break;
    case PROP_BITMAP:{
      dpObj.value.dp_bitmap = *static_cast<uint32_t *>(value);
    } break;
    case PROP_BOOL:{
      dpObj.value.dp_bool = *static_cast<bool *>(value);
    } break;
    default:{
      PR_ERR("The data type (void *) is not supported, dpid %d type %d  ", dpid, dpNode->desc.prop_tp);
      return OPRT_COM_ERROR;
    }
  }

  return tuya_iot_dp_obj_report(&ArduinoIoTClient, ArduinoIoTClient.activate.devid, &dpObj, 1, timestamp);;
}

int TuyaIoTCloudClass::rawWrite(uint8_t dpid, uint8_t* value, uint16_t len, uint32_t timeout)
{
  int rt = OPRT_OK;

  dp_raw_t *dpRaw = NULL;
  size_t mallocSize = 0;

  dp_node_t *dpNode = dp_node_find_by_devid(ArduinoIoTClient.activate.devid, dpid);
  if (dpNode == NULL) {
    PR_ERR("dpid %d not found", dpid);
    return OPRT_COM_ERROR;
  }

  if (T_RAW != dpNode->desc.type) {
    PR_ERR("dpid %d not raw type", dpid);
    return OPRT_NOT_SUPPORTED;
  }

  mallocSize = sizeof(dp_raw_t) + len;
  dpRaw = (dp_raw_t *)tal_malloc(mallocSize);
  if (dpRaw == NULL) {
    PR_ERR("malloc failed");
    return OPRT_MALLOC_FAILED;
  }
  memset(dpRaw, 0, mallocSize);

  dpRaw->id = dpid;
  dpRaw->len = len;
  memcpy(dpRaw->data, value, len);

  rt = tuya_iot_dp_raw_report(&ArduinoIoTClient, ArduinoIoTClient.activate.devid, dpRaw, timeout);

  tal_free(dpRaw);
  dpRaw = NULL;

  return rt;
}

tuya_event_id_t TuyaIoTCloudClass::getEventId(tuya_event_msg_t* event)
{
  return event->id;
}
uint16_t TuyaIoTCloudClass::getEventDpNum(tuya_event_msg_t* event)
{
  if (NULL == event) {
    PR_ERR("event is NULL");
    return 0;
  }

  if (TUYA_EVENT_DP_RECEIVE_OBJ != event->id) {
    PR_ERR("event id is not TUYA_EVENT_DP_RECEIVE_OBJ");
    return 0;
  }

  return event->value.dpobj->dpscnt;
}

uint8_t TuyaIoTCloudClass::getEventDpId(tuya_event_msg_t* event, uint16_t index)
{
  if (NULL == event) {
    PR_ERR("event is NULL");
    return 0;
  }

  if (TUYA_EVENT_DP_RECEIVE_OBJ != event->id) {
    PR_ERR("event id is not TUYA_EVENT_DP_RECEIVE_OBJ");
    return 0;
  }

  return event->value.dpobj->dps[index].id;
}

bool TuyaIoTCloudClass::readBool(tuya_event_msg_t* event, uint8_t dpid)
{
  if (NULL == event) {
    PR_ERR("event is NULL");
    return false;
  }

  if (TUYA_EVENT_DP_RECEIVE_OBJ != event->id) {
    PR_ERR("event id is not TUYA_EVENT_DP_RECEIVE_OBJ");
    return false;
  }

  dp_obj_recv_t *dpObj = event->value.dpobj;

  for (int i = 0; i < dpObj->dpscnt; i++) {
    if (dpid == dpObj->dps[i].id) {
      if (PROP_BOOL != dpObj->dps[i].type) {
        PR_ERR("dpid %d type is not bool", dpid);
        return false;
      } else {
        return dpObj->dps[i].value.dp_bool;
      }
    }
  }

  PR_ERR("dpid %d not found", dpid);
  return false;
}

int TuyaIoTCloudClass::readValue(tuya_event_msg_t* event, uint8_t dpid)
{
  if (NULL == event) {
    PR_ERR("event is NULL");
    return 0;
  }

  if (TUYA_EVENT_DP_RECEIVE_OBJ != event->id) {
    PR_ERR("event id is not TUYA_EVENT_DP_RECEIVE_OBJ");
    return 0;
  }

  dp_obj_recv_t *dpObj = event->value.dpobj;

  for (int i = 0; i < dpObj->dpscnt; i++) {
    if (dpid == dpObj->dps[i].id) {
      if (PROP_VALUE != dpObj->dps[i].type) {
        PR_ERR("dpid %d type is not value", dpid);
        return 0;
      } else {
        return dpObj->dps[i].value.dp_value;
      }
    }
  }

  PR_ERR("dpid %d not found", dpid);
  return 0;
}

uint32_t TuyaIoTCloudClass::readEnum(tuya_event_msg_t* event, uint8_t dpid)
{
  if (NULL == event) {
    PR_ERR("event is NULL");
    return 0;
  }

  if (TUYA_EVENT_DP_RECEIVE_OBJ != event->id) {
    PR_ERR("event id is not TUYA_EVENT_DP_RECEIVE_OBJ");
    return 0;
  }

  dp_obj_recv_t *dpObj = event->value.dpobj;

  for (int i = 0; i < dpObj->dpscnt; i++) {
    if (dpid == dpObj->dps[i].id) {
      if (PROP_ENUM != dpObj->dps[i].type) {
        PR_ERR("dpid %d type is not enum", dpid);
        return 0;
      } else {
        return dpObj->dps[i].value.dp_enum;
      }
    }
  }

  PR_ERR("dpid %d not found", dpid);
  return 0;
}

char* TuyaIoTCloudClass::readString(tuya_event_msg_t* event, uint8_t dpid)
{
  if (NULL == event) {
    PR_ERR("event is NULL");
    return NULL;
  }

  if (TUYA_EVENT_DP_RECEIVE_OBJ != event->id) {
    PR_ERR("event id is not TUYA_EVENT_DP_RECEIVE_OBJ");
    return NULL;
  }

  dp_obj_recv_t *dpObj = event->value.dpobj;

  for (int i = 0; i < dpObj->dpscnt; i++) {
    if (dpid == dpObj->dps[i].id) {
      if (PROP_STR != dpObj->dps[i].type) {
        PR_ERR("dpid %d type is not string", dpid);
        return NULL;
      } else {
        return dpObj->dps[i].value.dp_str;
      }
    }
  }

  PR_ERR("dpid %d not found", dpid);
  return NULL;
}

uint32_t TuyaIoTCloudClass::readBitmap(tuya_event_msg_t* event, uint8_t dpid)
{
  if (NULL == event) {
    PR_ERR("event is NULL");
    return 0;
  }

  if (TUYA_EVENT_DP_RECEIVE_OBJ != event->id) {
    PR_ERR("event id is not TUYA_EVENT_DP_RECEIVE_OBJ");
    return 0;
  }

  dp_obj_recv_t *dpObj = event->value.dpobj;

  for (int i = 0; i < dpObj->dpscnt; i++) {
    if (dpid == dpObj->dps[i].id) {
      if (PROP_BITMAP != dpObj->dps[i].type) {
        PR_ERR("dpid %d type is not bitmap", dpid);
        return 0;
      } else {
        return dpObj->dps[i].value.dp_bitmap;
      }
    }
  }

  PR_ERR("dpid %d not found", dpid);
  return 0;
}

uint8_t* TuyaIoTCloudClass::readRaw(tuya_event_msg_t* event, uint8_t dpid, uint16_t* len)
{
  if (NULL == event) {
    PR_ERR("event is NULL");
    return NULL;
  }

  if (NULL == len) {
    PR_ERR("len is NULL");
    return NULL;
  }

  if (TUYA_EVENT_DP_RECEIVE_RAW != event->id) {
    PR_ERR("event id is not TUYA_EVENT_DP_RECEIVE_RAW");
    return NULL;
  }

  dp_raw_recv_t *dpRaw = event->value.dpraw;

  if (dpid != dpRaw->dp.id) {
    PR_ERR("dpid %d not found", dpid);
    return NULL;
  }

  *len = dpRaw->dp.len;

  return dpRaw->dp.data;
}

int TuyaIoTCloudClass::remove(void)
{
  tuya_iot_reset(&ArduinoIoTClient);
  return OPRT_OK;
}

/******************************************************************************
 * PRIVATE MEMBER FUNCTIONS
 ******************************************************************************/


/******************************************************************************
 * EXTERN DEFINITION
 ******************************************************************************/
TuyaIoTCloudClass TuyaIoT;
