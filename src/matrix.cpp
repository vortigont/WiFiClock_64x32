#include "config.h"
#include "matrix.h"
#include "image.h"
#include "main.h"

extern MTX myMtx;

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

bool mtxStarted = false;

bool showPoints;
void MTX::init()
{
    matrix.begin();
    // matrix.cp437(true);
    matrix.setRotation(0); 
    matrix.setTextWrap(false);
    matrix.fillScreen(0);
    LOG(printf_P, PSTR("Matrix was initialized \n"));
}

void MTX::handle()
{
    if (!mtxStarted) myMtx.start();
    else {
        myMtx.time();
    }

// myMtx.time();
}


void MTX::start()
{
    static unsigned long starting_timer;
    static int l;
    if (!l){
    matrix.setTextColor(myCYAN);
    matrix.setCursor(2, 0);
    matrix.print("WiFi");
    // matrix.swapBuffers(true);
}
        if (!embui.sysData.wifi_sta && l < 6){
        if (starting_timer + 1000 < millis()){
            matrix.print(".");
            matrix.swapBuffers(true);
            #ifdef MP3PLAYER
            if(playAlert) dfPlayer.playFolder(wifi_connecting);
            #endif
            l++;
            starting_timer = millis();
                LOG(printf_P, PSTR("Matrix timer \n"));
            }
        }
        else {
            #ifdef MP3PLAYER
            if(playAlert) dfPlayer.playFolder(wifi_connected);
            #endif
            matrix.fillScreen(0);
            matrix.setCursor(2, 8);
            matrix.setFont();
            if (starting_timer + 2000 > millis()){
            matrix.print(embui.sysData.wifi_sta ? "WiFi OK" : "WiFi AP");
            matrix.setCursor(1, 10);
            matrix.setFont(&TomThumb);
            matrix.println(embui.sysData.wifi_sta ? WiFi.localIP().toString() : "192.168.4.1");
            matrix.swapBuffers(true);
                }
            matrix.fillScreen(0);
            if (starting_timer + 2000 < millis()) matrix.drawRGBBitmap(18, 3, image_data_Image30, 26, 26);
            if (starting_timer + 3000 < millis()) matrix.drawRGBBitmap(18, 3, image_data_Image31, 26, 26);
            if (starting_timer + 4000 < millis()) matrix.drawRGBBitmap(18, 3, image_data_Image32, 26, 26);
            if (starting_timer + 5000 < millis()) matrix.drawRGBBitmap(18, 3, image_data_Image33, 26, 26);
            if (starting_timer + 6000 < millis()) matrix.drawRGBBitmap(18, 3, image_data_Image34, 26, 26);
            if (starting_timer + 7000 < millis()) matrix.drawRGBBitmap(18, 3, image_data_Image35, 26, 26);
            if (starting_timer + 8000 < millis()) matrix.drawRGBBitmap(18, 3, image_data_Image36, 26, 26);
            matrix.swapBuffers(false);
            matrix.fillScreen(0);


           if (starting_timer + 10000 < millis()) mtxStarted = true;
    }

}


void MTX::time(){
        const tm* t = localtime(embui.timeProcessor.now());  // Определяем для вывода времени 
    char dispTime[5]; 
    sprintf (dispTime, (showPoints ? "%02d:%02d" : "%02d %02d"), t->tm_hour, t->tm_min);
    matrix.setCursor(21, 9);
    matrix.setTextSize(1);
    matrix.setFont(&TomThumb);
    matrix.setTextColor(myBLUE);
    if(embui.timeProcessor.isDirtyTime()) matrix.println((showPoints) ? "--:--" : "-- --");
    else matrix.println(String(dispTime));
    matrix.swapBuffers(true);
    matrix.setFont();
    matrix.fillScreen(0);
    static unsigned long wait_handlers;
    if (wait_handlers + 999U > millis())
      return;
    wait_handlers = millis();
    showPoints=!showPoints;

}