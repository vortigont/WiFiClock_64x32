#ifndef _SENSORS_H
#define _SENSORS_H
// #ifdef SENSORS
#include "config.h"
#include "main.h"
#include "DHT.h"
#include "Adafruit_BME280.h"
#include "timerMinim.h"

class Sensors : public Adafruit_BME280 {
public:
  void start(){
    if (!begin()) {
      LOG(printf_P, PSTR("BME280 not initialized \n"));
      return;
    }
    update();
    }
    void update(){
      temp = int(readTemperature());
      hum = int(readHumidity());
      press = readPressure();
      alt = readAltitude(SEALEVELPRESSURE_HPA);
      LOG(printf_P, PSTR("Sensors updated Temp %d Hum %d Press %f Alt %f \n"), temp, hum, press, alt);
    }
    String getTemp(){
      String tempS;  
      if (temp > 0) tempS = "+" + String(temp) + "\260" + "c";
      if (temp < 0) tempS = "-" + String(temp) + "\260" + "c";
      else tempS = " " + String(temp) + "\260" + "c";
      return tempS;
    }
    String getHum(){return String(hum);}
    String getPress(){return String(press / 1.3332239).substring(0, 3);}
    String getAltitude(){return String(alt) + "m";};

private:
  String sensorType = SENSOR_TYPE;

  int16_t temp = 0;
  int16_t hum = 0;
  float press = 0.0;
  float alt = 0.0;

};

extern Sensors sens;

#endif