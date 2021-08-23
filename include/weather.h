#ifndef _WEATHER_H
#define _WEATHER_H
#endif
#include "config.h"
#include "main.h"
#ifdef WEATHER
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <ArduinoJson.h>

typedef union {
    struct {
    bool displayCityName:1;
    bool displayForecast:1;
    bool displayForecastNow:1;
    bool displayForecastToday:1;
    bool displayForecastTomorrow:1;
    bool displayNarodmon:1;
    };
    uint8_t weatherFlags;
}WEATHERFLAGS;

enum {
    TODAY = 1,
    TOMORROW,
    NAROD
};
    
class Weather {
    public:
    void init();
    void handle();
    String getWeathTemp(){String now;
        now =  (location_temp > 0 ? "+" : ((location_temp < 0 ) ? "-" : "")) + String(location_temp) + + "\260" + "c"; return now;}
    String getWeathTempTmrw(){String now;
        now = (tempMin > 0 ? "+" : ((tempMax < 0 ) ? "-" : "")) + String(tempMax) + "\260" + "c"; return now;}
        int image() {return location_code;}
    String showNarodmon();
    String showHome();
    String showToday();
    String showTomorrow();
    void update();
    void setUpdate();
    int8_t getUpdate();
    bool getNarodmon();
    bool getTomorrow();
    bool getToday();
    bool getUpdError(uint8_t err) {
        if (!updError)
            return false;

        switch (err)
        {
        case TODAY:
            if (bitRead(updError, TODAY))
                return true;
            break;
        case TOMORROW:
            if (bitRead(updError, TOMORROW))
                return true;
            break;
        case NAROD:
            if (bitRead(updError, NAROD))
                return true;
            break;
        
        default:
            break;
        }
        return false;
    }
    String getNarodmonTemp() { 
        String now;
        now =  (tempNM > 0 ? "+" : ((tempNM < 0 ) ? "-" : "")) + String(tempNM, 1) + + "\260" + "c"; return now;}
    const WEATHERFLAGS &getWeatherSett() {return flags;}
private:
    WEATHERFLAGS flags;
    byte updError = 0;
    String getMinTmrw(){String now;
    now = (tempMin > 0 ? "+" : ((tempMin < 0 ) ? "-" : "")) + String(tempMin) + "\260" + "c"; return now;}
    String getMaxTmrw(){String now;
    now = (tempMax > 0 ? "+" : ((tempMax < 0 ) ? "-" : "")) + String(tempMax)+ "\260" + "c"; return now;}
    String city = CITY;
    int data_1_rh = 0;
    int data_1_clouds = 0;
    uint8_t upd = 0;
    HTTPClient http;
    WiFiClient ESPclient;
    String lat = WEATHERBIT_LAT;             //Географические координаты
    String lon = WEATHERBIT_LON;             //
    String weatherLang = "ru";
    String weatherHost0 = "api.weatherbit.io";
    String weatherKey0 = WEATHERBIT_API_KEY;        //ключ weatherbit

    long timer = 0;
    byte sch = 0;
    String location_name = "";
    // String location_region = "";
    // String location_country = "";
    // String location_localtime = "";
    int  location_temp = 0;
    float  location_app_temp = 0.0;
    int    location_rh = 0;
    float  location_pres = 0.0;
    int    location_code = 0;
    // int    coded1;
    float  location_wind_spd;
    String location_wind_cdir_full = "";
    // String location_sunrise = "";
    // String location_sunset = "";
    int    location_clouds = 0;
    // int    location_vis;
    int    location_uv = 0;
    int  tempMin = 0;
    int  tempMax = 0;
    float data_1_wind_spd = 0.0;

    String tomorrow_weather_description = "";
    String tomorrow_wind_cdir_full = "";
    float tempNM = 0.0;
    // float pressNM = 0;
    // float humNM = 0;

    String location_weather_description = "";

    uint16_t sensors_ID[5] = {NARODMON_SENSOR_ID1, NARODMON_SENSOR_ID2, NARODMON_SENSOR_ID3, NARODMON_SENSOR_ID4, NARODMON_SENSOR_ID5};
    String narodmonUid = NARODMON_UID;
    String narodmonApiKey = NARODMON_API_KEY;
    
};

extern Weather weather;

#endif