/******************************************************************************
 * INCLUDE
 ******************************************************************************/
#include "TuyaIoTWeather.h"

// tuya open sdk
#include "tal_time_service.h"
#include "tal_memory.h"
#include "tal_log.h"
#include "atop_base.h"
#include "cJSON.h"

/******************************************************************************
 * CONSTANT
 ******************************************************************************/
#define WEATHER_API              "thing.weather.get"
#define API_VERSION              "1.0"

/******************************************************************************
 * TYPEDEF DEFINE
 ******************************************************************************/
struct weatherCode {
  uint32_t index;
  char* code;
};

/******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************/
static struct weatherCode sgWeatherCode[] = {
    {.index = TW_INDEX_TEMP,          .code = "w.temp"},
    {.index = TW_INDEX_HUMIDITY,      .code = "w.humidity"},
    {.index = TW_INDEX_CONDITIONNUM,  .code = "w.conditionNum"},
    {.index = TW_INDEX_PRESSURE,      .code = "w.pressure"},
    {.index = TW_INDEX_REAL_FEEL,     .code = "w.realFeel"},
    {.index = TW_INDEX_UVI,           .code = "w.uvi"},
    {.index = TW_INDEX_SUNRISE,       .code = "w.sunrise"},
    {.index = TW_INDEX_SUNSET,        .code = "w.sunset"},
    {.index = TW_INDEX_WIND_SPEED,    .code = "w.windSpeed"},
    {.index = TW_INDEX_WIND_DIR,      .code = "w.windDir"},
    {.index = TW_INDEX_WIND_LEVEL,    .code = "w.windLevel"},
    {.index = TW_INDEX_AQI,           .code = "w.aqi"},
    {.index = TW_INDEX_RANK,          .code = "w.rank"},
    {.index = TW_INDEX_PM10,          .code = "w.pm10"},
    {.index = TW_INDEX_PM25,          .code = "w.pm25"},
    {.index = TW_INDEX_O3,            .code = "w.o3"},
    {.index = TW_INDEX_NO2,           .code = "w.no2"},
    {.index = TW_INDEX_CO,            .code = "w.co"},
    {.index = TW_INDEX_SO2,           .code = "w.so2"},
    {.index = TW_INDEX_THIGH,         .code = "w.thigh"},
    {.index = TW_INDEX_TLOW,          .code = "w.tlow"},
    {.index = TW_INDEX_DATE_N,        .code = "w.date.n"}, // need replace n
    {.index = TW_INDEX_CURRDATE,      .code = "w.currdate"},
    {.index = TW_INDEX_C_AREA,        .code = "c.area"},
    {.index = TW_INDEX_C_CITY,        .code = "c.city"},
    {.index = TW_INDEX_C_PROVINCE,    .code = "c.province"},
};

/******************************************************************************
 * LOCAL MODULE FUNCTIONS
 ******************************************************************************/
int TuyaIoTWeatherClass::_allowWeatherUpdate(tuya_iot_client_t *client)
{
  if (false == tuya_iot_activated(client)) {
    return OPRT_COM_ERROR;
  }

  // check network status
  if (client->config.network_check && client->config.network_check()) {
    return OPRT_OK;
  } else {
    return OPRT_COM_ERROR;
  }
}

char* TuyaIoTWeatherClass::_getWeatherCode(uint32_t index)
{
  for (int i = 0; i < sizeof(sgWeatherCode) / sizeof(sgWeatherCode[0]); i++) {
    if (index == sgWeatherCode[i].index) {
      return sgWeatherCode[i].code;
    }
  }

  return NULL;
}

int TuyaIoTWeatherClass::_atopWeatherRequest(char* code, atop_base_response_t *response)
{
  int rt = OPRT_OK;
  TIME_T timestamp = 0;
  char *postData = NULL;
  int postDataLen = 0;
  atop_base_request_t atopRequest;

  rt = tal_time_check_time_sync();
  if (OPRT_OK != rt) {
    PR_ERR("tal_time_check_time_sync error:%d", rt);
    return rt;
  }

  // network check
  if (!_clientHandle->config.network_check) {
    PR_ERR("network_check is NULL");
    return OPRT_COM_ERROR;
  }
  if (!_clientHandle->config.network_check()) {
    PR_ERR("network is not connected");
    return OPRT_COM_ERROR;
  }

  timestamp = tal_time_get_posix();

  postDataLen = snprintf(NULL, 0, "{\"codes\":[%s], \"t\":%d}", code, timestamp);
  postDataLen++; // add '\0'

  postData = (char *)tal_malloc(postDataLen);
  if (NULL == postData) {
    return OPRT_MALLOC_FAILED;
  }
  memset(postData, 0, postDataLen);

  snprintf(postData, postDataLen, "{\"codes\":[%s], \"t\":%d}", code, timestamp);
  PR_DEBUG("Post: %s", postData);

  memset(&atopRequest, 0, sizeof(atop_base_request_t));
  atopRequest.devid = _clientHandle->activate.devid;
  atopRequest.key  = _clientHandle->activate.seckey;
  atopRequest.path = "/d.json";
  atopRequest.timestamp = timestamp;
  atopRequest.api = WEATHER_API;
  atopRequest.version = API_VERSION;
  atopRequest.data =  reinterpret_cast<void *>(postData);
  atopRequest.datalen = strlen(postData);

  rt = atop_base_request(&atopRequest, response);
  if (OPRT_OK != rt) {
    PR_ERR("atop_base_request error:%d", rt);
  }

  tal_free(postData);
  postData = NULL;

  return rt;
}

/******************************************************************************
 * CTOR/DTOR
 ******************************************************************************/


/******************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/
String TuyaIoTWeatherClass::get(uint32_t index)
{
  String value = "";
  int rt = OPRT_OK;
  atop_base_response_t response;
  // char* code = "w.temp, w.humidity, w.realFeel, w.pm25, w.thigh, w.tlow, w.date.3, w.currdate";
  char* code = "\"w.temp\",\"w.humidity\",\"w.realFeel\",\"w.pm25\",\"w.thigh\",\"w.tlow\",\"w.conditionNum\",\"w.currdate\",\"c.city\"";

  // code = _getWeatherCode(index);

  memset(&response, 0, sizeof(atop_base_response_t));

  rt = _atopWeatherRequest(code, &response);
  if (OPRT_OK != rt) {
    PR_ERR("atopWeatherRequest error:%d", rt);
    return "";
  }

  PR_DEBUG("response success: %d", response.success);

  if (response.success) {
    cJSON *result = response.result;
    // cJSON *item = cJSON_GetObjectItem(result, code);
    // if (item) {
    //   area = item->valuestring;
    // }
    char *result_value = cJSON_PrintUnformatted(result);
    value = String(result_value);
    // PR_DEBUG("result: %s", value.c_str());
    cJSON_free(result_value);
  }

  atop_base_response_free(&response);

  return value;
}

int TuyaIoTWeatherClass::getTempHighLow(int& high, int& low)
{
  int rt = OPRT_OK;

  atop_base_response_t response;

  char* code = "\"w.thigh\",\"w.tlow\",\"w.date.1\"";

  memset(&response, 0, sizeof(atop_base_response_t));

  rt = _atopWeatherRequest(code, &response);
  if (OPRT_OK != rt) {
    PR_ERR("atopWeatherRequest error:%d", rt);
    return rt;
  }

  PR_DEBUG("response success: %d", response.success);

  if (response.success && response.result) {
    cJSON *result = response.result;
    cJSON *dataObj = cJSON_GetObjectItem(result, "data");
    if (dataObj) {
      cJSON *item = cJSON_GetObjectItem(dataObj, "w.thigh.0");
      if (item) {
        high = item->valueint;
      }
      item = cJSON_GetObjectItem(dataObj, "w.tlow.0");
      if (item) {
        low = item->valueint;
      }
    }

    PR_DEBUG("high: %d, low: %d", high, low);
  } else {
    rt = OPRT_COM_ERROR;
  }

  atop_base_response_free(&response);

  return rt;
}

/******************************************************************************
 * PRIVATE MEMBER FUNCTIONS
 ******************************************************************************/

