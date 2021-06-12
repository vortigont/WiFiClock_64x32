#include "config.h"
#include "matrix.h"
#include "image.h"
#include "main.h"
#include "weather.h"
#include "sensors.h"
MTX matrix;


void MTX::init()
{
    begin();
    cp437(true);
    setRotation(0); 
    setTextWrap(false);
    LOG(printf_P, PSTR("Matrix was initialized \n"));
    fillScreen(0);
  }

void MTX::handle()
{
  static unsigned long weather_home_switch;
  if (!mtxStarted) start();
  else {
    fillScreen(0);
    static unsigned long wait_handlers;
    if (wait_handlers + 500U > millis())
    return;
    wait_handlers = millis();

    if (nightMode) getNightMode();
    else{
    getGoodMorning();
    if (weather_home_switch + 60*1000 > millis()) getClock();
    else if (weather_home_switch + 75*1000 > millis()) getHome();
    else if (weather_home_switch + 90*1000 > millis()) getWeather();
    }

    swapBuffers(true);
    if (weather_home_switch + 89*1000 < millis()) weather_home_switch = millis();
  }
if(NIGHTMODE_TIME <= getHour() || MORNING_TIME > getHour()) nightMode = true; 
else nightMode = false;
}

void MTX::start()
{
  static unsigned long wait_handlers;
  if (wait_handlers + animInterv > millis())
    return;
  wait_handlers = millis();
  static unsigned long showIp;
  static int l;
  setFont();
  setTextColor(myCYAN);
  if (!l){
  setCursor(2, 0);
  print("WiFi");
  }
  if (!embui.sysData.wifi_sta && l < 6){
    print(".");
    #ifdef MP3PLAYER
    if(playAlert) dfPlayer.playFolder(wifi_connecting);
    #endif
    LOG(printf_P, PSTR("WiFi connecting... \n"));
    swapBuffers(true);
    l++;
  }
  else {
    #ifdef MP3PLAYER
    if(playAlert) dfPlayer.playFolder(wifi_connected);
    #endif
    if (l <= 6) {
      fillScreen(0);
      showIp = millis();
      setFont();
      setCursor(2, 0);
      println(embui.sysData.wifi_sta ? "WiFi OK" : "WiFi AP");
      setCursor(1, 10);
      setFont(&TomThumb);
      println(embui.sysData.wifi_sta ? WiFi.localIP().toString() : "192.168.4.1");
      l++;
      swapBuffers(true);
      fillScreen(0);
    }
    if (showIp + 4000 > millis()) return;
      switch (l){
      case 7: drawRGBBitmap(0, 0, image_01, 64, 32);
      break;
      case 8: drawRGBBitmap(0, 0, image_02, 64, 32);
      break;
      case 9: drawRGBBitmap(0, 0, image_03, 64, 32);
      break;
      case 10: drawRGBBitmap(0, 0, image_04, 64, 32);
      break;
      case 11: drawRGBBitmap(0, 0, image_05, 64, 32);
      break;
      case 12: drawRGBBitmap(0, 0, image_06, 64, 32);
      break;
      case 13: drawRGBBitmap(0, 0, image_07, 64, 32);
      break;
      case 14: drawRGBBitmap(0, 0, image_08, 64, 32);
      break;
      case 15: drawRGBBitmap(0, 0, image_09, 64, 32);
      break;
      case 16: drawRGBBitmap(0, 0, image_10, 64, 32);
      break;
    }
    if (l >= 7) l++;
    if (l==22) mtxStarted = true;
    swapBuffers(true);
  }
}
 
// Функция для времени в нужном формате
String MTX::getTime(){

  const tm* t = localtime(embui.timeProcessor.now());  // Определяем для вывода времени 
  static char dispTime[5]; 
  if (embui.timeProcessor.isDirtyTime()) sprintf (dispTime, (showPoints ? "--:--" : "-- --"));
  else sprintf (dispTime, (showPoints ? "%02d:%02d" : "%02d %02d"), t->tm_hour, t->tm_min);
  if (millis() % 1000 > 500) showPoints=!showPoints;
  return String(dispTime);

}



void MTX::getWeather(){
  setFont();
  fillRect(0, 0, 64, 32, myBLACK);
  setCursor(13, -2);
  setTextColor(myYELLOW);
  println(utf8rus("погода"));
  setFont();
  if (getMDay() < 10) setCursor(7, 7); 
  else setCursor(2, 7);
  setTextSize(1);
  setFont(&TomThumb);
  setTextColor(myGREEN);
  print(getMDay());
  setFont();
  setCursor(4, 25);
  setTextSize(1);
  setFont(&TomThumb);
  setTextColor(myRED);
  // if ( location_temp > 0 ) texttemp = "+" ;  // ТЕМПЕРАТУРА
  print(weather.getWeathTemp());
  setTextSize(1);
  setFont();
  if (getMDay() < 10) setCursor(57, 7);
  else setCursor(52, 7);
  setFont(&TomThumb);
  setTextColor(myGREEN);
  print(getTomorrow());
  setFont();
  setCursor(39, 25);
  setTextSize(1);
  setFont(&TomThumb);
  setTextColor(myRED);
  print(weather.getWeathTempTmrw());
  setFont();
  setCursor(21, 9);
  setTextSize(1);
  setFont(&TomThumb);
  setTextColor(myRED);
  println(getTime());
  setFont();
  getImage();
    
}

void MTX::getImage()
{
  switchAnim=!switchAnim;
  switch (weather.image()) {            // задать с монитора
      case 200: //Гроза с небольшим дождем
      case 201: // Гроза с дождем
      case 202: // гроза с сильным дождем
      case 210: // Легкая гроза
      case 211: // Гроза
      case 212: // Сильная гроза
      case 221: // Рваная гроза
      case 230: // Гроза с мелким дождиком
      case 231: // Гроза с моросью
      case 232: // Гроза с сильным дождиком
       
     if (switchAnim) drawRGBBitmap(10, 14, image_data_Image14, 10, 10); else drawRGBBitmap(10, 14, image_data_Image15, 10, 10);

     
        break;
      case 300: // Слабая морось
      case 301: // Морось
      case 302: // Сильный дождь
      case 310: // Моросящий дождь
      case 311: // Моросящий дождь
      case 312: // Сильный моросящий дождь
      case 500: // Легкий дождь
      case 501: // Умеренный дождь
      case 511: // Холодный дождь
      case 521: // Дождь
      case 531: // Рваный дождь
      case 613: // Мокрый дождь
     
     if (switchAnim) drawRGBBitmap(10, 14, image_data_Image10, 10, 10); else drawRGBBitmap(10, 14, image_data_Image11, 10, 10);

     
        break;
      case 313: // Ливень, дождь и изморозь
      case 314: // Сильный ливень дождь и изморозь
      case 502: // Сиильный дождь
      case 503: // Очень сильный дождь
      case 504: // Сильный дождь
      case 520: // Темно, ливень
      case 522: // Сильный пролевной дождь
     
     if (switchAnim) drawRGBBitmap(10, 14, image_data_Image8, 10, 10); else drawRGBBitmap(10, 14, image_data_Image9, 10, 10);

     
       break;
      
      case 601: // Снег
      case 602: // Сильный снегопад
      case 622: // Сильный снегопад
      
     if (switchAnim) drawRGBBitmap(10, 14, image_data_Image4, 10, 10); else drawRGBBitmap(10, 14, image_data_Image5, 10, 10);

     
      case 600: //легкий снег
      case 610: // Снег с дождем
      case 611: // Мокрый снег
      case 612: // Легкий дождь со снегом
      case 615: //Небольшой дождь и снег
      case 616: // Дождь и снег
      case 620: // Легкий снегопад
      case 621: // Ливень и снег
      
     if (switchAnim) drawRGBBitmap(10, 14, image_data_Image6, 10, 10); else drawRGBBitmap(10, 14, image_data_Image7, 10, 10);

     
       break;
      case 701: // Туман
      case 711: // Дым
      case 721: // Мгла
      case 731: // Песчаная буря
      case 741: // Туман
      case 751: // Песок
      case 761: // Пыль
      case 762: // Вулканический пепел
      case 771: // Шквал
      case 781: // Торнадо
      //int zik = 0;
      //for (int zik = 0; zik < 100; zik ++){
     if (switchAnim) drawRGBBitmap(10, 14, image_data_Image2, 10, 10); else drawRGBBitmap(10, 14, image_data_Image3, 10, 10);

     
      //}
        break;
      case 800: // Ясно, чистое небо
      if (getHour() < 21 && getHour() > 6){ 
     if (switchAnim) drawRGBBitmap(10, 14, image_data_Image, 10, 10); else drawRGBBitmap(10, 14, image_data_Image1, 10, 10);

     
      }else{
      if (switchAnim) drawRGBBitmap(10, 14, image_data_Image20, 10, 10); else drawRGBBitmap(10, 14, image_data_Image21, 10, 10);

     
         
      }
       break;
      case 801: // Мало облачно 25%
      case 802: // Облано 50%
      case 803: // Облачно 80%
      if (getHour() < 21 && getHour() > 6){
      if (switchAnim) drawRGBBitmap(10, 14, image_data_Image16, 10, 10); else drawRGBBitmap(10, 14, image_data_Image17, 10, 10);
 
    
      }else{
      if (switchAnim) drawRGBBitmap(10, 14, image_data_Image18, 10, 10); else drawRGBBitmap(10, 14, image_data_Image19, 10, 10);

      
      }
       break;
      case 804: // Пасмурно 100%
       
     if (switchAnim) drawRGBBitmap(10, 14, image_data_Image12, 10, 10); else drawRGBBitmap(10, 14, image_data_Image13, 10, 10);

     
        break;
  }
    switch (weather.image()) {      //ЗАДАТЬ С МОНИТОРА
      case 200: //Гроза с небольшим дождем
      case 201: // Гроза с дождем
      case 202: // гроза с сильным дождем
      case 210: // Легкая гроза
      case 211: // Гроза
      case 212: // Сильная гроза
      case 221: // Рваная гроза
      case 230: // Гроза с мелким дождиком
      case 231: // Гроза с моросью
      case 232: // Гроза с сильным дождиком
       
     if (switchAnim) drawRGBBitmap(44, 14, image_data_Image14, 10, 10); else drawRGBBitmap(44, 14, image_data_Image15, 10, 10);

     
        break;
      case 300: // Слабая морось
      case 301: // Морось
      case 302: // Сильный дождь
      case 310: // Моросящий дождь
      case 311: // Моросящий дождь
      case 312: // Сильный моросящий дождь
      case 500: // Легкий дождь
      case 501: // Умеренный дождь
      case 511: // Холодный дождь
      case 521: // Дождь
      case 531: // Рваный дождь
      case 613: // Мокрый дождь
     
     if (switchAnim) drawRGBBitmap(44, 14, image_data_Image10, 10, 10); else drawRGBBitmap(44, 14, image_data_Image11, 10, 10);

     
        break;
      case 313: // Ливень, дождь и изморозь
      case 314: // Сильный ливень дождь и изморозь
      case 502: // Сиильный дождь
      case 503: // Очень сильный дождь
      case 504: // Сильный дождь
      case 520: // Темно, ливень
      case 522: // Сильный пролевной дождь
     
     if (switchAnim) drawRGBBitmap(44, 14, image_data_Image8, 10, 10); else drawRGBBitmap(44, 14, image_data_Image9, 10, 10);

     
       break;
      
      case 601: // Снег
      case 602: // Сильный снегопад
      case 622: // Сильный снегопад
      
     if (switchAnim) drawRGBBitmap(44, 14, image_data_Image4, 10, 10); else drawRGBBitmap(44, 14, image_data_Image5, 10, 10);

     
      case 600: //легкий снег
      case 610: // Снег с дождем
      case 611: // Мокрый снег
      case 612: // Легкий дождь со снегом
      case 615: //Небольшой дождь и снег
      case 616: // Дождь и снег
      case 620: // Легкий снегопад
      case 621: // Ливень и снег
      
     if (switchAnim) drawRGBBitmap(44, 14, image_data_Image6, 10, 10); else drawRGBBitmap(44, 14, image_data_Image7, 10, 10);

     
       break;
      case 701: // Туман
      case 711: // Дым
      case 721: // Мгла
      case 731: // Песчаная буря
      case 741: // Туман
      case 751: // Песок
      case 761: // Пыль
      case 762: // Вулканический пепел
      case 771: // Шквал
      case 781: // Торнадо
      //int zik = 0;
      //for (int zik = 0; zik < 100; zik ++){
     if (switchAnim) drawRGBBitmap(44, 14, image_data_Image2, 10, 10); else drawRGBBitmap(44, 14, image_data_Image3, 10, 10);

     
      //}
        break;
      case 800: // Ясно, чистое небо
       
     if (switchAnim) drawRGBBitmap(44, 14, image_data_Image, 10, 10); else drawRGBBitmap(44, 14, image_data_Image1, 10, 10);

     
      
       break;
      case 801: // Мало облачно 25%
      case 802: // Облано 50%
      case 803: // Облачно 80%
      
     if (switchAnim) drawRGBBitmap(44, 14, image_data_Image16, 10, 10); else drawRGBBitmap(44, 14, image_data_Image17, 10, 10);

    
       break;
      case 804: // Пасмурно 100%
       
     if (switchAnim) drawRGBBitmap(44, 14, image_data_Image12, 10, 10); else drawRGBBitmap(44, 14, image_data_Image13, 10, 10);

     
        break;
  }
  
}

void MTX::getHome(){
  fillRect(0, 0, 64, 32, myBLACK);
  setFont();
  setCursor(42, 0);
  setTextSize(1);
  setFont(&TomThumb);
  setTextColor(myBLUE);
  println(getTime());
  setFont();
  setCursor(4, -2);
  println(utf8rus("дома"));
  setTextSize(1);
  setFont(&TomThumb);
  setCursor(13, 15);
  setTextColor(myRED);
  println(sens.getPress());
  setCursor(13, 28);
  setTextColor(myRED);
  println(sens.getTemp());    // температура
  setCursor(49, 28);
  setTextColor(myRED);
  println(sens.getHum());   //влажность
  setFont();
  switchAnim=!switchAnim;
  if (switchAnim) {
  drawRGBBitmap(0, 7, image_data_Image24, 12, 12);
  drawRGBBitmap(0, 20, image_data_Image22, 12, 12);
  drawRGBBitmap(38, 20, image_data_Image26, 10, 12);
  drawRGBBitmap(52, 9, image_data_Image28, 12, 12);
  }
  else {
  drawRGBBitmap(0, 7, image_data_Image25, 12, 12);
  drawRGBBitmap(0, 20, image_data_Image23, 12, 12);
  drawRGBBitmap(38, 20, image_data_Image27, 10, 12);
  drawRGBBitmap(52, 9, image_data_Image29, 12, 12);
  }

}

void MTX::getClock(){
  drawLine(55, 8, 63, 8, myBLACK);
  if (getHour() == NIGHTMODE_TIME - 1 && getMin() >= 58){
      drawLine(55, 8, 63, 8, myBLACK);
      drawLine(55, 7, 63, 7, myBLACK);
      // scroll_text(24, frameDelay, ("Подготовка к ночному режиму"));
  }
  else {
    // fillRect(46, 10, 2, 13, myBLACK);
    // drawLine(0, 7, 63, 7, myBLACK);
    // setTextColor(myMAGENTA);

    // print("Сегодня " + getMDay() +  getMonth() + "  " + getYear() + " года " + getWDay());  // show text
    // scroll_text(24, frameDelay - 4, (weatherString));       // show text
    // scroll_text(24, frameDelay - 4, (weatherStringZ));    // show text
    setFont();
    setCursor(49,8);
    // drawLine(55, 7, 9, 7, myBLACK);
    setTextColor(myRED);
    setFont(&kongtext4pt7b);
    print(getWDayShort());


    setFont();
    if (getMDay() < 10) setCursor(5,-1);
    else setCursor(9,0);
    setTextColor(myRED);
    setFont(&kongtext4pt7b);
    print(getMDay());

    setFont();
    setCursor(22,0);// добавил
    setTextColor(myGREEN);
    print(utf8rus(getMonthTxt()));
    
    setFont();
    if (getSec() < 10) setCursor(55,16);
    else setCursor(49,16);
    setTextColor(myGREEN);
    print(getSec());

    setFont();
    setCursor(1, 15);
    setTextColor(myGREEN);
    fillRect(0, 8, 20, 15, myBLACK);
    setFont(&FreeSansBold9pt7b);
    setTextSize(1);
    print(getTime());

  }

}


void MTX::scrollText(uint8_t ypos, unsigned long scroll_delay, String text)
{
  static unsigned long wait_handlers;
  if (wait_handlers + scroll_delay > millis())
    return;
  wait_handlers = millis();
  uint16_t text_length = text.length();

    for (int xpos = MATRIX_WIDTH; xpos > -(MATRIX_WIDTH + text_length * 5); xpos--)
  {
    setFont();
    setCursor(xpos, ypos);                //
    fillRect(0, ypos, 64, 8, myBLACK);    //
    setTextColor(myMAGENTA);              //
    print(utf8rus(text));                 //
    setFont();                            //
    delay(scroll_delay);                         //
    // yield();
  }
  
}


void MTX::getGoodMorning(){
  static unsigned long goodMorning;
  if (showMorning) {
    fillRect(0, 0, 64, 32, myBLACK);
    drawRGBBitmap(0, 0, image_data_ytro2, 64, 32);
    swapBuffers(false);
    
    goodMorning = millis();
  }
  if (goodMorning + 5000U > millis()) return;
      showMorning = false;
  }


void MTX::getNightMode(){

  static unsigned long goodNight;
  static bool l;
  if (!l){
  fillRect(0, 0, 64, 32, myBLACK); 
  drawRGBBitmap(0, 0, image_data_noch2, 64, 32);
  swapBuffers(false); 
  l++;
  goodNight=millis();
  }
  if (goodNight + 3000 > millis()) return;
  fillRect(0, 0, 64, 32, myBLACK);    
  
  setFont();
  setCursor(8, 16);
  setTextSize(1);
  setFont(&FreeSansBold9pt7b);
  setTextColor(Color333(0,0,2));
  println(getTime());

  if (MORNING_TIME == getHour()) showMorning = true;
}


// ================================ Вывод Русских Букв
String MTX::utf8rus(String source)
{
  int i,k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };

  k = source.length(); i = 0;

  while (i < k) {
    n = source[i]; i++;

    if (n >= 0xC0) {
      switch (n) {
        case 0xD0: {
          n = source[i]; i++;
          if (n == 0x81) { n = 0xA8; break; }
          if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
          break;
        }
        case 0xD1: {
          n = source[i]; i++;
          if (n == 0x91) { n = 0xB8; break; }
          if (n >= 0x80 && n <= 0x8F) n = n + 0x70;
          break;
        }
      }
    }
    m[0] = n; target = target + String(m);
  }
return target;
}
