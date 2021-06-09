
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

extern MTX matrix;

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
  matrix.init();
}


// MAIN loop
void loop() {
  embui.handle();
  matrix.handle();
// LOG(printf_P, PSTR("Matrix was initialized \n"));
#ifdef USE_FTP
    ftp_loop(); // цикл обработки событий фтп-сервера
#endif
}