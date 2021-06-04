#include "config.h"
#include "matrix.h"
#include "image.h"

RGBmatrixPanel matrix(MTX_A, MTX_B, MTX_C, MTX_D, MTX_CLK, MTX_LAT, MTX_OE, true, 64);

uint16_t myRED = matrix.Color333(5,0,0);
uint16_t myGREEN = matrix.Color333(0,5,0);
uint16_t myBLUE = matrix.Color333(0,0,5);
uint16_t myWHITE = matrix.Color333(7, 7,7);
uint16_t myYELLOW = matrix.Color333(2,2,0);
uint16_t myCYAN = matrix.Color333(0,5,5);
uint16_t myMAGENTA = matrix.Color333(5,0,5);
uint16_t myShadow = matrix.Color333(2,0,5);
uint16_t myROSE = matrix.Color333(5,0,2);
uint16_t myBLACK = matrix.Color333(0,0,0);
bool showPoints;
  static unsigned long myTimer1;
void matrix_setup() {
    matrix.begin();
    matrix.cp437(true);
    matrix.setRotation(0); 
    matrix.setTextWrap(false);
    matrix.fillScreen(0);
    matrix.setTextColor(myCYAN);
    matrix.setCursor(2,0);
    matrix.println("WiFi");
    matrix.swapBuffers(true);
    // while (!embui.sysData.wifi_sta) {
    //     delay(2000);
    //     if (!embui.sysData.wifi_sta){
    //         matrix.print(".");
    //         matrix.swapBuffers(true);
    //         #ifdef MP3PLAYER
    //         if(playAlert) dfPlayer.playFolder(wifi_connecting);
    //         #endif
    //         }else {
    //             #ifdef MP3PLAYER
    //             if(playAlert) dfPlayer.playFolder(wifi_connected);
    //             #endif
    //         }
    // }
    // matrix.fillScreen(0);
    // matrix.setCursor(2, 0);
    // matrix.print("WiFi OK");
    // delay(3000);
    // matrix.fillScreen(0);
    // matrix.setCursor(1, 1);
    // matrix.print("IP address:");
    // matrix.swapBuffers(true);
    // matrix.setCursor(1, 10);
    // matrix.setFont(&TomThumb);
    // matrix.println(WiFi.localIP());
    // matrix.swapBuffers(true);
    // delay(1000);
    // matrix.fillScreen(0);
    // matrix.drawRGBBitmap(18, 3, image_data_Image30, 26, 26);
    // matrix.swapBuffers(false);
    // delay(1000);
    // matrix.fillScreen(0);
    // matrix.drawRGBBitmap(18, 3, image_data_Image31, 26, 26);
    // matrix.swapBuffers(false);
    // delay(1000);
    // matrix.fillScreen(0);
    // matrix.drawRGBBitmap(18, 3, image_data_Image32, 26, 26);
    // matrix.swapBuffers(false);
    // delay(1000);
    // matrix.fillScreen(0);
    // matrix.drawRGBBitmap(18, 3, image_data_Image33, 26, 26);
    // matrix.swapBuffers(false);
    // delay(1000);
    // matrix.fillScreen(0);
    // matrix.drawRGBBitmap(18, 3, image_data_Image34, 26, 26);
    // matrix.swapBuffers(false);
    // delay(1000);
    // matrix.fillScreen(0);
    // matrix.drawRGBBitmap(18, 3, image_data_Image35, 26, 26);
    // matrix.swapBuffers(false);
    // delay(1000);
    // matrix.fillScreen(0);
    // matrix.drawRGBBitmap(18, 3, image_data_Image36, 26, 26);
    // matrix.swapBuffers(false);
    // delay(3000);
    matrix.fillScreen(0);
    LOG(printf_P, PSTR("Matrix was initialized \n"));
}

void matrix_loop(){

    uint16_t i = millis();
    const tm* t = localtime(embui.timeProcessor.now());  // Определяем для вывода времени 
    char dispTime[5]; 
    sprintf (dispTime, (showPoints ? "%02d:%02d" : "%02d %02d"), t->tm_hour, t->tm_min);
    matrix.setCursor(21, 9);
    matrix.setTextSize(1);
    matrix.setFont(&TomThumb);
    matrix.setTextColor(myBLUE);
    if(!embui.timeProcessor.isDirtyTime()) {matrix.println((showPoints) ? "--:--" : "-- --");}
    else {matrix.println(String(dispTime));}
    matrix.swapBuffers(true);
    matrix.setFont();
    matrix.fillScreen(0);


if (millis() - myTimer1 >= 0 && millis() - myTimer1 <= 1000) 
showPoints = 1;
    if(millis() - myTimer1 >= 1001 && millis() - myTimer1 <= 2000) showPoints = 0;
    if (millis() - myTimer1 >= 2001) myTimer1 = millis();



}