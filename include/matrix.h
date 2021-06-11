#ifndef _MATRIX_H
#define _MATRIX_H

#include "config.h"
#include "main.h"
#include <RGBmatrixPanel.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeMonoBold12pt7b.h>
#include <Fonts/kongtext4pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include <Fonts/TomThumb.h>
#include <Fonts/Picopixel.h>
#include "EmbUI.h"
#include <Wire.h>
#include <SPI.h>


class MTX : public RGBmatrixPanel {
public:
    MTX(uint8_t a=MTX_A, uint8_t b=MTX_B, uint8_t c=MTX_C, uint8_t d=MTX_D, uint8_t clk=MTX_CLK, uint8_t lat=MTX_LAT, uint8_t oe=MTX_OE, boolean dbuf=true, uint8_t width=64) : RGBmatrixPanel(a, b, c, d, clk, lat, oe, dbuf, width){} ;
    void init();
    void handle();

private:
    uint8_t frameDelay = 10;
    uint16_t  animInterv = ANIM_INTERVAL;   /// интервал стартовой анимации
    bool mtxStarted = false;
    bool nightMode = false;
    bool showMorning = false;
    bool showPoints;
    bool switchAnim;
    uint16_t myRED = Color333(3, 0, 0);
    uint16_t myGREEN = Color333(0, 3, 0);
    uint16_t myBLUE = Color333(0, 0, 3);
    uint16_t myWHITE = Color333(3, 3, 3);
    uint16_t myYELLOW = Color333(3, 3, 0);
    uint16_t myCYAN = Color333(0, 3, 3);
    uint16_t myMAGENTA = Color333(3, 0, 3);
    uint16_t myShadow = Color333(5, 0, 3);
    uint16_t myROSE = Color333(3, 0, 5);
    uint16_t myBLACK = Color333(0, 0, 0);
    String getTime();
    int getHour(){const tm* t = localtime(embui.timeProcessor.now()); return t->tm_hour;}
    int getMin(){const tm* t = localtime(embui.timeProcessor.now()); return t->tm_min;}
    int getMonth(){const tm* t = localtime(embui.timeProcessor.now()); return t->tm_mon + 1;}
    int getMDay(){const tm* t = localtime(embui.timeProcessor.now()); return t->tm_mday + 1;}
    int getWDay(){const tm* t = localtime(embui.timeProcessor.now()); return t->tm_wday;}
    int getYear(){const tm* t = localtime(embui.timeProcessor.now()); return t->tm_year + 1900;}
    String getMonthTxt(){ 
        String result;
        switch (getMonth()) {
            case 1: result = " января"; break;
            case 2: result = " февраля"; break;
            case 3: result = " марта"; break;
            case 4: result = " апреля"; break;
            case 5: result = " мая"; break;
            case 6: result =   " июня"; break;
            case 7: result = " июля"; break;
            case 8: result = " августа"; break;
            case 9: result = " сентября"; break;
            case 10: result = " октября"; break;
            case 11: result = " ноября"; break;
            case 12: result = " декабря"; break;
    }
    return result;
    }
    String getWDayTxt(){
        String result;
        switch (getWDay()) {
            case 1: result = " воскресенье"; break;
            case 2: result = " понедельник"; break;
            case 3: result = " вторник"; break;
            case 4: result = " среда"; break;
            case 5: result = " четверг"; break;
            case 6: result = " пятница"; break;
            case 7: result = " суббота"; break;
        }
    return result;
    }
    int getTomorrow(){
        int tommorrow;
        if (((getMonth() == 4 || getMonth() == 6 || getMonth() == 9 || getMonth() == 11) && getMDay() == 30) || ((getMonth() == 1 || getMonth() == 3 || getMonth() == 5 || getMonth() == 7 || getMonth() == 8 || getMonth() == 10 || getMonth() == 12) && getMDay() == 31) || ((getMonth() == 2) && getMDay() == 28))  //а если высокосный год?
        {
        tommorrow = 1; 
        } else { 
        tommorrow = getMDay() + 1;
        }
        return tommorrow;
    };
    void start();
    void getClock();
    void getHome();
    void getWeather();
    void getNightMode();
    void getGoodMorning();
    void getImage();
    void scrollText(uint8_t ypos, unsigned long scroll_delay, String text);
    String utf8rus(String source);

};
extern MTX matrix;
#endif