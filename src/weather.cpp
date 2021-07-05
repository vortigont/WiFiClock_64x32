#include "config.h"
#ifdef WEATHER
#include "main.h"
#include "weather.h"
#include "EmbUI.h"
#include "Adafruit_BME280.h"
Weather weather;



void Weather::handle(){
  if (!embui.sysData.wifi_sta) return;
  static uint32_t timer;
  if (!weatherCheck) return;
  if (weatherCheck < 4) {
    if (timer + 1000 > millis())
      return;
    timer = millis();
    switch (weatherCheck) {
    case 1:
      getToday();
      // weatherCheck++;
      break;
    case 2:
      getTomorrow();
      // weatherCheck++;
      break;
    case 3:
      getNarodmon();
      // weatherCheck++;
      break;
    }
  } else{
    if (millis() - timer >= 600*1000 && !matrix.isNightMode()) {
      timer += 600*1000;
      if (timer < 600*1000) return; 
      getToday();
      getTomorrow();
      getNarodmon();
      timer < millis() - 600*1000;
      LOG(printf_P, PSTR("TIMER CHECK"));
    }
  }
}

//===============================================================================================================================//
//                              ПОГОДА                                                
//===============================================================================================================================//
void Weather::getToday() {
  if(weatherKey0=="" || !displayForecast) return;
  if(!embui.sysData.wifi_sta) return;
  LOG(printf_P, PSTR("======== START GET WEATHER FROM WEATHERBIT.IO ========= \n"));
  
  // if(ESPclient.connect(weatherHost0.c_str(), 80)){}
  // else {

  //   updateForecast++;
  //   return;
  // }
  //   //String reqline="http://"+weatherHost0+"/v2.0/current/daily?city="+cityID0+"&lang="+weatherLang+"&key="+weatherKey0;
  String reqline="http://"+weatherHost0+"/v2.0/current/daily?city="+String("&lat=") + String(lat) + String("&lon=") + String(lon)+"&lang="+weatherLang+"&key="+weatherKey0;
  LOG(printf_P, PSTR("%s \n"), reqline.c_str());
  if(http.begin(ESPclient, reqline)){
    int httpCode = http.GET();
    if(httpCode > 0) {
      LOG(printf_P, PSTR("[HTTP] GET... code: %d \n"), httpCode);
      if(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        line = http.getString();
      }
    } 
    else {
      LOG(printf_P, PSTR("[HTTP] GET... failed, error: %s\n"), http.errorToString(httpCode).c_str());
      http.end();
      updateForecast++;
      return;
    }
    http.end();
  } 
  else {
    LOG(printf_P, PSTR("[HTTP] Unable to connect\n"));
    updateForecast++;
    return;
  }
  if(line==""){
    LOG(printf_P, PSTR("[HTTP] The answer is empty\n"));
    updateForecast++;
    return;
  }
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(37) + 1128; //https://arduinojson.org/v6/assistant/
  DynamicJsonDocument doc(capacity);
  // LOG(printf_P, PSTR("JSON WEATHER %s" \n), line.c_str());
  deserializeJson(doc, line);
  if(!doc.capacity()){
    LOG(printf_P, PSTR("Parse weather forecast - FAILED!!!"));
    updateForecast++;
    return;
  }
  LOG(printf_P, PSTR("JSON WEATHER %s \n"), line.c_str());
  JsonObject data = doc["data"][0];
  location_rh = data["rh"]; 
  location_pres = data["pres"]; 
  if(pressSys == 1) location_pres /= 1.3332239;
  const char* data_timezone = data["timezone"]; 
  location_region = data_timezone;
  const char* data_ob_time = data["ob_time"]; 
  location_localtime = data_ob_time;
  const char* data_country_code = data["country_code"];
  location_country = data_country_code;
  location_clouds = data["clouds"]; 
  const char*   data_city_name = data["city_name"]; 
  location_name = data_city_name;
  location_wind_spd = data["wind_spd"]; // 1
  const char* data_wind_cdir_full = data["wind_cdir_full"]; 
  location_wind_cdir_full = data_wind_cdir_full;
  location_vis = data["vis"]; // 5
  const char*   data_sunset = data["sunset"];
  location_sunset = data_sunset;
  location_uv = data["uv"]; // 0
  int           data_wind_dir = data["wind_dir"]; 
  const char*   data_sunrise = data["sunrise"]; 
  location_sunrise = data_sunrise;   
  JsonObject    data_weather = data["weather"];
  const char*   data_weather_description = data_weather["description"]; 
  location_weather_description = data_weather_description;
  location_code = data_weather["code"];
  location_temp = data["temp"]; 
  location_app_temp = data["app_temp"];
  setWeatherChek();
}


String Weather::showToday(){
  String weatherString;
   if(displayForecastNow){
    if (location_temp > 0 ){
    weatherString +=  "Cейчас в " + String(city) + ":" + " температура " + getWeathTemp();
    }else{
    weatherString +=  "Cейчас в " + String(city) + ":" + " температура " + getWeathTemp();  
    }
    weatherString += " влажность " + String(location_rh) + "% "; 
    weatherString += "давление " + String((location_pres), 0) + (pressSys == 1 ? tPress : tPress0) + "ммРс ";
    weatherString += " ветер " + String(location_wind_cdir_full) + " " + String(location_wind_spd, 1) + "м/с";
    weatherString += " облачность " + String(location_clouds) +  "% " + location_weather_description + ".  ";
    return weatherString;
  }
}

// ============================================================================//
//               ПРОГНОЗ!!!    // 
// ============================================================================//
void Weather::getTomorrow() {
  if(weatherKey0=="" || !displayForecast) return;
  if(!embui.sysData.wifi_sta) return;
  LOG(printf_P, PSTR("======== START GET FORECAST FROM WEATHERBIT.IO ======== \n"));
   
  HTTPClient http;
  String line="";
  //String reqline="http://"+weatherHost0+"/v2.0/forecast/daily?city="+cityID0+"&lang="+weatherLang+"&days=2&key="+weatherKey0;
  String reqline="http://"+weatherHost0+"/v2.0/forecast/daily?city="+String("&lat=") + String(lat) + String("&lon=") + String(lon)+"&lang="+weatherLang+"&days=2&key="+weatherKey0;
  LOG(printf_P, PSTR("%s \n"), reqline.c_str());   
  if(http.begin(ESPclient, reqline)){
   int httpCode = http.GET();
   if(httpCode > 0) {
     LOG(printf_P, PSTR("[HTTP] GET... code: %d\n"), httpCode);
     if(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
      line = http.getString();
     }
   } 
   else {
     LOG(printf_P, PSTR("[HTTP] GET... failed, error: %s\n"), http.errorToString(httpCode).c_str());
     http.end();
     updateForecasttomorrow++;
     return;
   }
   http.end();
  } else {
    LOG(printf_P, PSTR("    [HTTP] Unable to connect\n"));
    updateForecasttomorrow++;
    return;
  }
  if(line==""){
    LOG(printf_P, PSTR("[HTTP] The answer is empty\n"));
    updateForecasttomorrow++;
    return;
  }
  const size_t capacity = JSON_ARRAY_SIZE(2) + 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(7) + 2*JSON_OBJECT_SIZE(37) + 3300;
  DynamicJsonDocument doc(capacity);
  LOG(printf_P, PSTR("JSON FORECAST %s \n"), line.c_str());
  deserializeJson(doc, line);
  if(!doc.capacity()){

    LOG(printf_P, PSTR("Parse weather forecast - FAILED!!!"));
    updateForecasttomorrow++;
    return;
  }
  // JsonObject data_0 = doc["data"][0];
  // JsonObject data_0_weather = data_0["weather"];
  // const char* data_0_weather_description = data_0_weather["description"]; 
  // float data_0_max_temp = data_0["max_temp"]; 
  // float data_0_min_temp = data_0["min_temp"]; 
  JsonObject data_1 = doc["data"][1];
  data_1_rh = data_1["rh"]; 
  data_1_clouds = data_1["clouds"]; 
  data_1_wind_spd = data_1["wind_spd"];
  JsonObject data_1_weather = data_1["weather"];
  const char* data_1_weather_description = data_1_weather["description"];
  tomorrow_weather_description = data_1_weather_description;
  const char* data_1_wind_cdir_full = data_1["wind_cdir_full"];
  tomorrow_wind_cdir_full = data_1_weather_description;
  float data_1_max_temp = data_1["max_temp"];
  float data_1_min_temp = data_1["min_temp"];
  float data_1_weather_code = data_1_weather["code"];
    coded1 = 0;
  tempfor1 = 0;
  tempfor2 = 0;
  coded1 += int(data_1_weather_code);
    tempfor1 += int(data_1_min_temp);
    tempfor2 += int(data_1_max_temp);
  LOG(printf_P, PSTR("Getting weather forecast for tomorrow - is OK."));
  updateForecasttomorrow = 0;
  updateForecastNot = false;
  setWeatherChek();
  LOG(printf_P, PSTR("CHECK WEATHER STRING %s \n"), showTomorrow().c_str());
}




String Weather::showTomorrow(){
  String weatherStringZ;
  if(displayForecastTomorrow) {
    weatherStringZ += "Прогноз на завтра: температура от " + getMinTmrw() + " до " + getMaxTmrw();
    weatherStringZ += " влажность " + String(data_1_rh) + "%";
    weatherStringZ += " ветер " + tomorrow_wind_cdir_full + " " + String(data_1_wind_spd, 1);//+ tSpeed
    weatherStringZ += "м/с " + tomorrow_weather_description;
    weatherStringZ += ".  ";
  }
  LOG(printf_P, PSTR("CHECK WEATHER %s \n"), weatherStringZ.c_str());
  return weatherStringZ;
}






//=========================================================================================================
// //                                  Народмониторинг
void Weather::getNarodmon() {
  if(!embui.sysData.wifi_sta)  return;
  LOG(printf_P, PSTR("Connection to narodmon.ru \n"));
  if (ESPclient.connect("http://narodmon.ru", 80)) {
  LOG(printf_P, PSTR("connection failed \n"));
    return;
  }
  if (!sensors_ID0) return;
  String line2 = "";
  String reqline = "http://narodmon.ru/api/sensorsValues?sensors=";
  if (sensors_ID0) reqline += String(sensors_ID0);
  if (sensors_ID1) reqline += "," + String(sensors_ID1);
  if (sensors_ID2) reqline += "," + String(sensors_ID2);
  reqline += "&uuid=" + narodmonUid + "&api_key=" + narodmonApiKey;
  // LOG(printf_P, PSTR("NARODMON %s \n"), reqline);

  if (http.begin(ESPclient, reqline)) { // HTTP
    int httpCode = http.GET();
    if (httpCode > 0) {
      LOG(printf_P, PSTR("[HTTP] GET... NARODMON code: %d\n"), httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        line = http.getString();
      }
    } else {
     LOG(printf_P, PSTR("[HTTP] GET... failed NARODMON, error: %s\n"), http.errorToString(httpCode).c_str());
    }
    http.end();
  } else {
    LOG(printf_P, PSTR("[HTTP] Unable to connect NARODMON\n"));
  }
      LOG(printf_P, PSTR("NARODMON answer="));
      LOG(println, F(line.c_str()));
    const size_t capacity = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(6) + 340; //https://arduinojson.org/v6/assistant/
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);
  if (!doc.capacity()) {
    LOG(printf_P, PSTR("Parse weather forecast - FAILED!!!"));
    return;
  }
  JsonObject sensors_0 = doc["sensors"][0];
  float sensors_0_value = sensors_0["value"]; // 14.2
  long sensors_0_time = sensors_0["time"]; // 1571853360
  JsonObject sensors_1 = doc["sensors"][1];
  float sensors_1_value = sensors_1["value"]; // 14
  long sensors_1_time = sensors_1["time"]; // 1571853000
  JsonObject sensors_2 = doc["sensors"][2];
  float sensors_2_value = sensors_2["value"];
  long sensors_2_time = sensors_2["time"];

  long timestamp = epochNM + (millis() / 1000);
  // LOG(println, F("sensors_0 = " + sensors_0_value.c_str() + "sensors_1 = " + sensors_1_value.c_str() + "    sensors_2 = " + sensors_2_value.c_str());
  // LOG(println, F("time_0 = " + (timestamp - sensors_0_time).c_str() + "      time_1 = " + (timestamp - sensors_1_time).c_str() + "      time_2 = " + (timestamp - sensors_2_time).c_str());

  //   pressNM = 0;
  // humNM = 0;
  // if (sensors_ID0) {
    // if ((timestamp - sensors_0_time) > 3600) {
      // sensors_0_value = 99;
    // } else 
    tempNM = sensors_0_value;
  // }
  // if (sensors_ID1) {

    // if ((timestamp - sensors_1_time) > 3600) {
      // sensors_1_value = 99;
    // } else if 
    if (tempNM > sensors_1_value) tempNM = sensors_1_value;
  // }
  // if (sensors_ID2) {
  //   if ((timestamp - sensors_2_time) > 3600) {
  //     sensors_2_value = 99;
  //   } else if (tempNM > sensors_2_value) humNM = sensors_2_value;
  // }
  // if (!tempNM && !updateForecast) tempNM = location_temp;
  //Serial.println("tempNM = " + String(tempNM, 1) + "'C");
  setWeatherChek();
}

String Weather::showNarodmon(){
  String now;
  now += "Народный мониторинг - на улице температура: " + getNarodmonTemp();
  now += "     В помещении: " + sens.getTemp() + ",влажность: " + sens.getHum() + ", давление: " + sens.getPress() +  "ммРc.";
  return now;
}


#endif