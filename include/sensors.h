#ifndef _SENSORS_H
#define _SENSORS_H
// #ifdef SENSORS
#include "config.h"
#include "main.h"
#include "DHT.h"
#include "Adafruit_BME280.h"

class Sensors : public Adafruit_BME280 {
public:
    void start();
    String getTemp(){String temp;
    if (readTemperature() > 0) temp = "+" + (String)int(readTemperature()) + "\260C";
    if (readTemperature() < 0) temp = "-" + (String)int(readTemperature()) + "\260C";
    else temp = " " + (String)int(readTemperature()) + "c";
    return temp;
    }
    String getHum(){String hum = (String)int(readHumidity()) + "%"; return hum;
    }
    String getPress(){String press = String(readPressure() / 1.3332239).substring(0, 3) + "mmMs";return press;}
    String getAltitude(){String alt = (String)readAltitude(SEALEVELPRESSURE_HPA) + "m"; return alt;};

private:
  String sensorType = SENSOR_TYPE;
};

extern Sensors sens;

#endif