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
    now = "+" + String(location_temp)+"c"; return now;}
    String getWeathTempTmrw(){String now;
    now = "+" + String(tempfor1)+"c"; return now;}
    int image() {return location_code;}

private:
    void getToday();
    void getTomorrow();
    int dst = 0;
    int ii;
    int nn;
    float hd;
    float td;
    String rh;
    String weatherString;
    String weatherStringZ;
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
    String city = "Гомеле";
    WiFiClient ESPclient;
    String lat = "38.123";             //Географические координаты
    String lon = "-78.543";             //
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
    float valid_date_1;
    float tempNM = 0.0;
    float pressNM = 0;
    float tempBme = 0;
    float humBme = 0;
    float pressBme = 0;
    float altBme = 0;
    String location_weather_description = "";
};

extern Weather weather;

#endif