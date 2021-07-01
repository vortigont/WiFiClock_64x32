
// Main headers
#include "main.h"
#include "matrix.h"
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
  sens.begin();
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
// LOG(printf_P, PSTR("Matrix was initialized \n"));
#ifdef USE_FTP
    ftp_loop(); // цикл обработки событий фтп-сервера
#endif
//   static unsigned long wait_handlers;
//   if (wait_handlers + 60000 > millis())
//     return;
//   wait_handlers = millis();
// weather.getNarodmon();

}