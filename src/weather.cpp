#include "config.h"
#ifdef WEATHER
#include "main.h"
#include "weather.h"
#include "EmbUI.h"
#include "Adafruit_BME280.h"
Weather weather;

void Weather::init(){
  flags.displayCityName = true;            // TODO
  flags.displayForecast = true;
  flags.displayForecastNow = true;
  flags.displayForecastToday = true;
  flags.displayForecastTomorrow = true;
  flags.displayNarodmon = true;
}

void Weather::handle(){
  if (!embui.sysData.wifi_sta) return;
    
  if (millis() - timer >= UPDATE_WEATHER_TIME * TASK_MINUTE && !matrix.isNightMode()) {
    timer +=  UPDATE_WEATHER_TIME * TASK_MINUTE;
    setUpdate();
    LOG(printf_P, PSTR("TIMER CHECK"));
  }
}

void Weather::update() {
  if (!upd)
    return;
  
Task *t = new Task(TASK_SECOND, TASK_FOREVER, []{
      switch (weather.getUpdate())
      {
      case TODAY:
        if (!weather.getToday())
          weather.updError |= (1UL << TODAY);
        break;
      case TOMORROW:
        if (!weather.getTomorrow())
          weather.updError |= (1UL << TOMORROW);
        break;
      case NAROD:
        if (!weather.getNarodmon())
          weather.updError |= (1UL << NAROD);
        break;
      }
      weather.upd--;
      if (!weather.getUpdate()) TASK_RECYCLE;},
    &ts, false);
    t->enableDelayed();
  updError = 0;
}

void Weather::setUpdate() {
if (!upd) {
    upd = 3;
    update();
  }
  else
    return;
}

int8_t Weather::getUpdate() {
return upd;
}

//===============================================================================================================================//
//                              ПОГОДА                                                
//===============================================================================================================================//
bool Weather::getToday() {
  if(weatherKey0=="" || !flags.displayForecastToday) return false;
  if(!embui.sysData.wifi_sta) return false;
  LOG(printf_P, PSTR("======== START GET WEATHER FROM WEATHERBIT.IO ========= \n"));
  
  String line;
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
      return false;
    }
    http.end();
  } 
  else {
    LOG(printf_P, PSTR("[HTTP] Unable to connect\n"));
    return false;
  }
  if(line==""){
    LOG(printf_P, PSTR("[HTTP] The answer is empty\n"));
    return false;
  }
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(37) + 1128; //https://arduinojson.org/v6/assistant/
  DynamicJsonDocument doc(capacity);
  // LOG(printf_P, PSTR("JSON WEATHER %s" \n), line.c_str());
  deserializeJson(doc, line);
  if(!doc.capacity()){
    LOG(printf_P, PSTR("Parse weather forecast - FAILED!!!"));
    return false;
  }
  LOG(printf_P, PSTR("JSON WEATHER %s \n"), line.c_str());
  JsonObject data = doc["data"][0];
  location_rh = data["rh"].as<int>(); 
  location_pres = data["pres"].as<int>(); 
  if(PRESS_SYSTEM) location_pres /= 1.3332239;
  // const char* data_timezone = data["timezone"]; 
  // location_region = data_timezone;
  // const char* data_ob_time = data["ob_time"]; 
  // location_localtime = data_ob_time;
  // location_country = data["country_code"].as<String>();
  location_clouds = data["clouds"].as<int>(); 
  location_name = data["city_name"].as<int>(); 
  location_wind_spd = data["wind_spd"].as<float>(); // 1
  location_wind_cdir_full = data["wind_cdir_full"].as<String>(); 
  // location_vis = data["vis"].as<int>(); // 5
  // location_sunset = data["sunset"].as<String>();
  // location_uv = data["uv"].as<int>(); // 0
  // int           data_wind_dir = data["wind_dir"]; 
  // location_sunrise = data["sunrise"].as<String>(); 
  JsonObject    data_weather = data["weather"];
  location_weather_description = data_weather["description"].as<String>(); 
  location_code = data_weather["code"].as<int>();
  location_temp = data["temp"].as<int>(); 
  location_app_temp = data["app_temp"].as<float>();
  return true;
}


String Weather::showToday(){
  if (getUpdError(TODAY))
    return "Ошибка обновления погоды";
  String today;
   if(weather.getWeatherSett().displayForecastNow){
    if (location_temp > 0 ){
    today +=  "Cейчас в " + String(city) + ":" + " температура " + getWeathTemp();
    }else{
    today +=  "Cейчас в " + String(city) + ":" + " температура " + getWeathTemp();  
    }
    today += " влажность " + String(location_rh) + "% "; 
    today += "давление " + String((location_pres), 0) + (PRESS_SYSTEM ? "ммРс " : "P ");
    today += " ветер " + String(location_wind_cdir_full) + " " + String(location_wind_spd, 1) + "м/с";
    today += " облачность " + String(location_clouds) +  "% " + location_weather_description + ".  ";
  }
  return today;
}

// ============================================================================//
//               ПРОГНОЗ!!!    // 
// ============================================================================//
bool Weather::getTomorrow() {
  if(weatherKey0=="" || !weather.getWeatherSett().displayForecastTomorrow) return false;
  if(!embui.sysData.wifi_sta) return false;
  LOG(printf_P, PSTR("======== START GET FORECAST FROM WEATHERBIT.IO ======== \n"));
   
  HTTPClient http;
  String line="";
  String reqline="http://"+weatherHost0+"/v2.0/forecast/daily?city="+ "&lat=" + lat + "&lon=" + lon+"&lang="+weatherLang+"&days=2&key="+weatherKey0;
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
     return false;
   }
   http.end();
  } 
  else {
    LOG(printf_P, PSTR("    [HTTP] Unable to connect\n"));
    return false;
  }
  if(line==""){
    LOG(printf_P, PSTR("[HTTP] The answer is empty\n"));
    return false;
  }
  const size_t capacity = JSON_ARRAY_SIZE(2) + 2*JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(7) + 2*JSON_OBJECT_SIZE(37) + 3300;
  DynamicJsonDocument doc(capacity);
  LOG(printf_P, PSTR("JSON FORECAST %s \n"), line.c_str());
  deserializeJson(doc, line);
  if(!doc.capacity()){
    LOG(printf_P, PSTR("Parse weather forecast - FAILED!!!"));
    return false;
  }
  // JsonObject data_0 = doc["data"][0];
  // JsonObject data_0_weather = data_0["weather"];
  // const char* data_0_weather_description = data_0_weather["description"]; 
  // float data_0_max_temp = data_0["max_temp"]; 
  // float data_0_min_temp = data_0["min_temp"]; 
  JsonObject data_1 = doc["data"][1];
  data_1_rh = data_1["rh"].as<int>(); 
  data_1_clouds = data_1["clouds"].as<int>(); 
  data_1_wind_spd = data_1["wind_spd"].as<int>();
  JsonObject data_1_weather = data_1["weather"];
  tomorrow_weather_description = data_1_weather["description"].as<String>();
  // tomorrow_weather_description = data_1_weather_description;
  tomorrow_wind_cdir_full = data_1["wind_cdir_full"].as<String>();
  // tomorrow_wind_cdir_full = data_1_weather_description;
  tempMax = data_1["max_temp"].as<int>();
  tempMin = data_1["min_temp"].as<int>();
  // coded1 = data_1_weather["code"].as<int>();
  LOG(printf_P, PSTR("Getting weather forecast for tomorrow - is OK."));
  LOG(printf_P, PSTR("CHECK WEATHER STRING %s \n"), showTomorrow().c_str());
  return true;
}




String Weather::showTomorrow(){
  if (getUpdError(TOMORROW))
    return "Ошибка обновления прогноза";
  String tomorrow;
  if(weather.getWeatherSett().displayForecastTomorrow) {
    tomorrow += "Прогноз на завтра: температура от " + getMinTmrw() + " до " + getMaxTmrw();
    tomorrow += " влажность " + String(data_1_rh) + "%";
    tomorrow += " ветер " + tomorrow_wind_cdir_full + " " + String(data_1_wind_spd, 1);//+ tSpeed
    tomorrow += "м/с " + tomorrow_weather_description;
    tomorrow += ".  ";
  }
  return tomorrow;
}






//=========================================================================================================
// //                                  Народмониторинг
bool Weather::getNarodmon() {
  if(!embui.sysData.wifi_sta || !weather.getWeatherSett().displayNarodmon)  return false;
  LOG(printf_P, PSTR("Connection to narodmon.ru \n"));
  if (ESPclient.connect("http://narodmon.ru", 80)) {
  LOG(printf_P, PSTR("connection failed \n"));
    return false;
  }


  String reqline = "http://narodmon.ru/api/sensorsValues?sensors=";
  String line;
  uint8_t sensCount = 0;
  for (uint8_t i = 0; i < sizeof(sensors_ID)/sizeof(sensors_ID[0]); i++) {
    if (sensors_ID[i]) {
    if (i) reqline += ",";
      reqline += String(sensors_ID[i]);
      sensCount++;
      LOG(printf_P, PSTR("Sens ID = %d \n"), i);
    }
    else
      LOG(printf_P, PSTR("NONE ID \n"));
  }

  reqline += "&uuid=" + narodmonUid + "&api_key=" + narodmonApiKey;
  LOG(printf_P, PSTR("NARODMON %s \n"), reqline.c_str());

  if (http.begin(ESPclient, reqline)) { // HTTP
    int httpCode = http.GET();
    if (httpCode > 0) {
      LOG(printf_P, PSTR("[HTTP] GET... NARODMON code: %d\n"), httpCode);
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        line = http.getString();
      }
    } else {
     LOG(printf_P, PSTR("[HTTP] GET... failed NARODMON, error: %s\n"), http.errorToString(httpCode).c_str());
     return false;
    }
    http.end();
  } else {
    LOG(printf_P, PSTR("[HTTP] Unable to connect NARODMON\n"));
    return false;
  }
  LOG(printf_P, PSTR("NARODMON answer=%s\n"), line.c_str());
  const size_t capacity = JSON_ARRAY_SIZE(2) + JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(6) + 340; //https://arduinojson.org/v6/assistant/
  DynamicJsonDocument doc(capacity);
  deserializeJson(doc, line);
  if (!doc.capacity()) {
    LOG(printf_P, PSTR("Parse weather forecast - FAILED!!!\n"));
    return false;
  }
  JsonObject sensors[sensCount];
  float sens_val[sensCount];
  for (uint8_t i = 0; i < sensCount; i++) {
    sensors[i] = doc["sensors"][i];
    sens_val[i] = sensors[i]["value"].as<float>(); // 14.2
  }
  tempNM = sens_val[0];
  for (uint8_t i = 1; i < sensCount; i++) {
    tempNM = tempNM < sens_val[i] ? tempNM : sens_val[i];
  }
  return true;
}

String Weather::showNarodmon(){
  if (!getWeatherSett().displayNarodmon)
    return "";
  if (getUpdError(NAROD))
    return "Ошибка обновления NARODMON";
  String narodmon;
  narodmon += "Народный мониторинг - на улице температура: " + getNarodmonTemp();
  narodmon += "     В помещении: " + sens.getTemp() + ",влажность: " + sens.getHum() + "%, давление: " + sens.getPress() +  "ммРc.";
  return narodmon;
}


#endif