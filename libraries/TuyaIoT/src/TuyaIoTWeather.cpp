/******************************************************************************
 * INCLUDE
 ******************************************************************************/
#include "TuyaIoTWeather.h"

#include "ArduinoTuyaIoTClient.h"

// tuya open sdk
#include "tal_time_service.h"
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

/******************************************************************************
 * CTOR/DTOR
 ******************************************************************************/


/******************************************************************************
 * PUBLIC MEMBER FUNCTIONS
 ******************************************************************************/
String TuyaIoTWeatherClass::getArea(void)
{
  int rt = OPRT_OK;
  String area = "";
  char requestData[64] = {0};

  atop_base_request_t atopRequest;
  atop_base_response_t response;

  tuya_iot_client_t *client = &ArduinoIoTClient;

  if (OPRT_OK != _allowWeatherUpdate(client)) {
    PR_ERR("Weather update not allowed");
    return "";
  }

  char* code = _getWeatherCode(TW_INDEX_C_AREA);
  if (NULL == code) {
    PR_ERR("Weather code not found");
    return "";
  }

  TIME_T timestamp = tal_time_get_posix();

  snprintf(requestData, sizeof(requestData), "{\"codes\":[\"%s\"], \"t\":%d}", code, timestamp);
  PR_DEBUG("Post: %s", requestData);

  memset(&atopRequest, 0, sizeof(atop_base_request_t));
  memset(&response, 0, sizeof(atop_base_response_t));

  atopRequest.devid = client->activate.devid;
  atopRequest.key  = client->activate.seckey;
  atopRequest.path = "/d.json";
  atopRequest.timestamp = timestamp;
  atopRequest.api = WEATHER_API;
  atopRequest.version = API_VERSION;
  atopRequest.data =  reinterpret_cast<void *>(requestData);
  atopRequest.datalen = strlen(requestData);

  rt = atop_base_request(&atopRequest, &response);
  if (OPRT_OK != rt) {
    PR_ERR("atop_base_request error:%d", rt);
    return "";
  }

  PR_DEBUG("response success: %d", response.success);

  if (response.success) {
    cJSON *result = response.result;
    // cJSON *item = cJSON_GetObjectItem(result, code);
    // if (item) {
    //   area = item->valuestring;
    // }
    char* value = cJSON_PrintUnformatted(result);
    PR_DEBUG("result: %s", value);
    cJSON_free(value);
  }

  atop_base_response_free(&response);

  return area;
}

/******************************************************************************
 * PRIVATE MEMBER FUNCTIONS
 ******************************************************************************/

