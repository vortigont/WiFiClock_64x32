
// Main headers
#include "main.h"
#include "matrix.h"
#include "rtc.h"
// #include <SPIFFSEditor.h>
/**
 * построение интерфейса осуществляется в файлах 'interface.*'
 *
 */
#ifdef USE_FTP
  #include "ftpSrv.h"
#endif

#ifdef WEATHER
#include "weather.h"
#endif

#ifdef SENSORS
#include "sensors.h"
#endif
// TaskHandle_t TaskEmb;

timerMinim rtcUpd;

// void TaskEmbCode( void * parameter) {
//   for(;;){
//     // weather.handle();
//   static unsigned long wait_handlers;
//   if (wait_handlers + 60000 > millis())
//     return;
//   wait_handlers = millis();
//   LOG(printf_P, PSTR("CORE CORE CORE === %d \n"), xPortGetCoreID());
//   }
// }

// MAIN Setup
void setup() {
  Serial.begin(460800);
  // Serial.begin(BAUD_RATE);
  // Serial.println("Starting test...");

  // Start EmbUI framework
  embui.begin();
  // weather.handle();
  #ifdef USE_FTP
      ftp_setup(); // запуск ftp-сервера
  #endif
  matrix.init();
  sens.start();
  rtc.init();
  rtcUpd.setInterval(24*3600*1000);
  weather.init();
  // xTaskCreatePinnedToCore(
  //                   TaskEmbCode,   /* Функция задачи */
  //                   "TaskEmb",     /* Название задачи */
  //                   10000,       /* Размер стека задачи */
  //                   NULL,        /* Параметр задачи */
  //                   1,           /* Приоритет задачи */
  //                   &TaskEmb,      /* Идентификатор задачи,
  //                                   чтобы ее можно было отслеживать */
  //                   0);          /* Ядро для выполнения задачи (0) */

}

// MAIN loop
void loop() {
  embui.handle();
  matrix.handle();
  weather.handle();


  if (rtcUpd.isReady())
    rtc.updateRtcTime();
  static unsigned long wait_handlers;
  if (wait_handlers + 1000 > millis())
    return;
  wait_handlers = millis();
  LOG(printf_P, PSTR("DIRTY %d \n"), embui.timeProcessor.isDirtyTime());


}