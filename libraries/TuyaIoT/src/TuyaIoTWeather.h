#ifndef __TUYA_IOT_WEATHER_H__
#define __TUYA_IOT_WEATHER_H__

/******************************************************************************
 * INCLUDE
 ******************************************************************************/
#include <Arduino.h>
#include "ArduinoTuyaIoTClient.h"

// tuya open sdk
#include "tuya_iot.h"

/******************************************************************************
 * CONSTANTS
 ******************************************************************************/
// https://developer.tuya.com/en/docs/mcu-standard-protocol/mcusdk-wifi-weather?id=Kd2fvzw7ny80s#title-10-Appendix
#define TW_INDEX_TEMP         (1)
#define TW_INDEX_HUMIDITY     (1 << 1)
#define TW_INDEX_CONDITIONNUM (1 << 2)
#define TW_INDEX_PRESSURE     (1 << 3)
#define TW_INDEX_REAL_FEEL    (1 << 4)
#define TW_INDEX_UVI          (1 << 5)
#define TW_INDEX_SUNRISE      (1 << 6)
#define TW_INDEX_SUNSET       (1 << 7)
#define TW_INDEX_WIND_SPEED   (1 << 8)
#define TW_INDEX_WIND_DIR     (1 << 9)
#define TW_INDEX_WIND_LEVEL   (1 << 10)
#define TW_INDEX_AQI          (1 << 11)
#define TW_INDEX_RANK         (1 << 12)
#define TW_INDEX_PM10         (1 << 13)
#define TW_INDEX_PM25         (1 << 14)
#define TW_INDEX_O3           (1 << 15)
#define TW_INDEX_NO2          (1 << 16)
#define TW_INDEX_CO           (1 << 17)
#define TW_INDEX_SO2          (1 << 18)
#define TW_INDEX_THIGH        (1 << 19)
#define TW_INDEX_TLOW         (1 << 20)
#define TW_INDEX_DATE_N       (1 << 21)
#define TW_INDEX_CURRDATE     (1 << 22)

// City
// https://developer.tuya.com/en/docs/mcu-standard-protocol/mcusdk-wifi-weather?id=Kd2fvzw7ny80s#title-13-Appendix%203%3A%20Weather%20service%20data
#define TW_INDEX_C_AREA       (1 << 23)
#define TW_INDEX_C_CITY       (1 << 24)
#define TW_INDEX_C_PROVINCE   (1 << 25)


// Weather data
// https://developer.tuya.com/en/docs/mcu-standard-protocol/mcusdk-wifi-weather?id=Kd2fvzw7ny80s#title-14-Appendix%204%3A%20Weather%20data%20in%20UTF-8%20encoding
#define TW_WEATHER_SUNNY                    (120)
#define TW_WEATHER_HEAVY_RAIN               (101)
#define TW_WEATHER_THUNDERSTORM             (102)
#define TW_WEATHER_SANDSTORM                (103)
#define TW_WEATHER_LIGHT_SNOW               (104)
#define TW_WEATHER_SNOW                     (105)
#define TW_WEATHER_FREEZING_FOG             (106)
#define TW_WEATHER_RAINSTORM                (107)
#define TW_WEATHER_ISOLATED_SHOWER          (108)
#define TW_WEATHER_DUST                     (109)
#define TW_WEATHER_THUNDER_AND_LIGHTNING    (110)
#define TW_WEATHER_LIGHT_SHOWER             (111)
#define TW_WEATHER_RAIN                     (112)
#define TW_WEATHER_SLEET                    (113)
#define TW_WEATHER_DUST_DEVIL               (114)
#define TW_WEATHER_ICE_PELLETS              (115)
#define TW_WEATHER_STRONG_SANDSTORM         (116)
#define TW_WEATHER_SAND_BLOWING             (117)
#define TW_WEATHER_LIGHT_TO_MODERATE_RAIN   (118)
#define TW_WEATHER_MOSTLY_CLEAR             (119)
#define TW_WEATHER_FOG                      (121)
#define TW_WEATHER_SHOWER                   (122)
#define TW_WEATHER_HEAVY_SHOWER             (123)
#define TW_WEATHER_HEAVY_SNOW               (124)
#define TW_WEATHER_EXTREME_RAINSTORM        (125)
#define TW_WEATHER_BLIZZARD                 (126)
#define TW_WEATHER_HAIL                     (127)
#define TW_WEATHER_LIGHT_TO_MODERATE_SNOW   (128)
#define TW_WEATHER_PARTLY_CLOUDY            (129)
#define TW_WEATHER_LIGHT_SNOW_SHOWER        (130)
#define TW_WEATHER_MODERATE_SNOW            (131)
#define TW_WEATHER_OVERCAST                 (132)
#define TW_WEATHER_NEEDLE_ICE               (133)
#define TW_WEATHER_DOWNPOUR                 (134)
#define TW_WEATHER_THUNDERSHOWER_AND_HAIL   (136)
#define TW_WEATHER_FREEZING_RAIN            (137)
#define TW_WEATHER_SNOW_SHOWER              (138)
#define TW_WEATHER_LIGHT_RAIN               (139)
#define TW_WEATHER_HAZE                     (140)
#define TW_WEATHER_MODERATE_RAIN            (141)
#define TW_WEATHER_CLOUDY                   (142)
#define TW_WEATHER_THUNDERSHOWER            (143)
#define TW_WEATHER_MODERATE_TO_HEAVY_RAIN   (144)
#define TW_WEATHER_HEAVY_RAIN_TO_RAINSTORM  (145)
#define TW_WEATHER_CLEAR                    (146)

// Wind direction code
// https://developer.tuya.com/en/docs/mcu-standard-protocol/mcusdk-wifi-weather?id=Kd2fvzw7ny80s#title-15-Appendix%205%3A%20Wind%20direction%20code

/******************************************************************************
 * TYPEDEF
 ******************************************************************************/


/******************************************************************************
 * CLASS DECLARATION
 ******************************************************************************/
class TuyaIoTWeatherClass {
public:
  TuyaIoTWeatherClass() {};
  ~TuyaIoTWeatherClass() {};

  String getCityArea(void);

  // city
  String get(uint32_t index);

private:
  tuya_iot_client_t *_clientHandle = &ArduinoIoTClient;

  int _allowWeatherUpdate(tuya_iot_client_t *client);
  char* _getWeatherCode(uint32_t index);
  int _atopWeatherRequest(char* code, atop_base_response_t *response);
};

#endif // !__TUYA_IOT_WEATHER_H__
