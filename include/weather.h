#ifndef _WEATHER_H
#define _WEATHER_H
#endif
#include "config.h"
#include "main.h"
#ifdef WEATHER
#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <ArduinoJson.h>
class Weather {
    public:
    void handle();
    String getWeathTemp(){String now;
    now =  (location_temp > 0 ? "+" : ((location_temp < 0 ) ? "-" : "")) + String(location_temp) + + "\260" + "С"; return now;}
    String getWeathTempTmrw(){String now;
    now = (tempfor1 > 0 ? "+" : ((tempfor1 < 0 ) ? "-" : "")) + String(tempfor1) + "\260" + "С"; return now;}
    int image() {return location_code;}
    String showNarodmon();
    String showHome();
    String showToday();
    String showTomorrow();
    void getNarodmon();
    String getNarodmonTemp() { 
    String now;
    now =  (tempNM > 0 ? "+" : ((tempNM < 0 ) ? "-" : "")) + String(tempNM, 1) + + "\260" + "С"; return now;}
private:
    void getToday();
    void getTomorrow();
    String getMinTmrw(){String now;
    now = (tempfor1 > 0 ? "+" : ((tempfor1 < 0 ) ? "-" : "")) + String(tempfor1) + "\260" + "С"; return now;}
    String getMaxTmrw(){String now;
    now = (tempfor2 > 0 ? "+" : ((tempfor2 < 0 ) ? "-" : "")) + String(tempfor2)+ "\260" + "С"; return now;}
    String city = CITY;
    int data_1_rh;
    int data_1_clouds;
    int dst = 0;
    int ii;
    int nn;
    float hd;
    float td;
    String rh;
    String weatherDescription = "";
    String weatherLocation = "";
    uint32_t lastTime = 0; 
    unsigned long start_time=0;
    bool displayCityName = true;
    bool displayForecast = true;
    bool displayForecastNow = true;
    bool displayForecastToday = true;
    bool displayForecastTomorrow = true;
    bool pressSys = 1;
    String tNow, tCurr, tPress, tPress0, tSpeed, tMin, tTom, tYour, tPoint, tIp, tPass, tWeatrNot, tWeatrTN;
    String jsonConfig = "{}";
    HTTPClient http;
    WiFiClient ESPclient;
    String lat = "48.263";             //Географические координаты
    String lon = "25.180";             //
    String weatherLang = "ru";
    String weatherHost0 = "api.weatherbit.io";
    String weatherKey0 = WEATHERBIT_API_KEY;        //ключ weatherbit
    bool updateForecastNot = true;
    int updateForecast = 0;
    int updateForecasttomorrow = 0;
    String line; 
    byte httpErrCount = 0;
    String texttd;
    String texttempmin;
    String texttempmax;
    String texttemp;
    String texttempBme;
    String textmonth;
    String textdow;
    String texttempNM;
    long timer;
    byte sch = 0;
    String location_name = "";
    String location_region = "";
    String location_country = "";
    String location_localtime = "";
    int  location_temp;
    float  location_app_temp;
    int    location_rh;
    float  location_pres;
    int    location_code;
    int    coded1;
    int    mintext;
    float  location_wind_spd;
    String location_wind_cdir_full = "";
    String location_sunrise = "";
    String location_sunset = "";
    int    location_clouds;
    int    location_vis;
    int    location_uv;
    int  tempfor1;
    float  tempfor2;
    int   data_1_weather_code;
    float data_1_max_temp;
    float data_1_min_temp;
    float data_1_wind_spd;
    const char* data_1_weather_description;
    const char* data_1_wind_cdir_full;
    float valid_date_1;
    float tempNM = 0.0;
    float pressNM = 0;
    float humNM = 0;
    float tempBme = 0;
    float humBme = 0;
    float pressBme = 0;
    float altBme = 0;
    String location_weather_description = "";
    unsigned long epochNM;
    int sensors_ID0 = 8335;  // температура датчики народмона// 3075
    int sensors_ID1 = 3075; // 
    int sensors_ID2 = 0;
    String narodmonUid = NARODMON_UID;
    String narodmonApiKey = NARODMON_API_KEY;
    
};

extern Weather weather;

#endif