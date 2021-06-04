
// Main headers
#include "main.h"
// #include <SPIFFSEditor.h>
/**
 * построение интерфейса осуществляется в файлах 'interface.*'
 *
 */
MTX myMtx;
#ifdef USE_FTP
  #include "ftpSrv.h"
#endif

// MAIN Setup
void setup() {
  Serial.begin(460800);
  // Serial.begin(BAUD_RATE);
  // Serial.println("Starting test...");

  // Start EmbUI framework
  embui.begin();
   
  #ifdef USE_FTP
      ftp_setup(); // запуск ftp-сервера
  #endif
  myMtx.init();
}


// MAIN loop
void loop() {
  embui.handle();
  myMtx.handle();
// LOG(printf_P, PSTR("Matrix was initialized \n"));
#ifdef USE_FTP
    ftp_loop(); // цикл обработки событий фтп-сервера
#endif
}