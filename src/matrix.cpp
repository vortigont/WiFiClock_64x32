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
    sendStringToMtx(utf8rus("СТАРТУЕМ !").c_str());
  }

void MTX::handle()
{
  if(NIGHTMODE_TIME <= getHour() || MORNING_TIME > getHour()) nightMode = true; 
  else nightMode = false;
  static unsigned long weather_home_switch;
  // if (!mtxStarted) start();
  // else {
    fillRect(0, 22, 64, 12, matrix.Color333(0, 0, 0));
   if (isStringPrinting) doPrintStringToMtx();


    static unsigned long wait_handlers;
    if (wait_handlers + 500U > millis())
    return;
    wait_handlers = millis();
    if (getHour() == NIGHTMODE_TIME && getMin() == 0 && getSec() < 5) getScreen();
    else if (getHour() == MORNING_TIME && getMin() == 0 && getSec() < 5) getScreen();
    else {
      if (nightMode) getNightMode();
      else{
      if (weather_home_switch + 60*1000 > millis()) getClock();
      else if (weather_home_switch + 75*1000 > millis()) getHome();
      else if (weather_home_switch + 90*1000 > millis()) getWeather();
      }
    }
    // swapBuffers(true);
    if (weather_home_switch + 89*1000 < millis()) weather_home_switch = millis();
  // }

  if (!isStringPrinting) swapBuffers(true);
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
  if (!embui.sysData.wifi_sta && l < 6 && millis() < 3000){
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
    if (showIp + 3000 > millis()) return;
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
  fillScreen(0);
  setFont();
  fillRect(0, 0, 64, 32, myBLACK);
  setCursor(13, -2);
  setTextColor(myYELLOW);
  println("погода");
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
  fillScreen(0);
  fillRect(0, 0, 64, 32, myBLACK);
  setFont();
  setCursor(42, 0);
  setTextSize(1);
  setFont(&TomThumb);
  setTextColor(myBLUE);
  println(getTime());
  setFont();
  setCursor(4, -2);
  println("дома");
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
  // drawLine(55, 8, 63, 8, myBLACK);
  // if (getHour() == NIGHTMODE_TIME - 1 && getMin() >= 58){
  //     drawLine(55, 8, 63, 8, myBLACK);
  //     drawLine(55, 7, 63, 7, myBLACK);
  //     // scroll_text(24, frameDelay, ("Подготовка к ночному режиму"));
  // }
  // else {
    // fillRect(46, 10, 2, 13, myBLACK);
    // drawLine(0, 7, 63, 7, myBLACK);
    // setTextColor(myMAGENTA);

    // print("Сегодня " + getMDay() +  getMonth() + "  " + getYear() + " года " + getWDay());  // show text
    // scroll_text(24, frameDelay - 4, (weatherString));       // show text
    // scroll_text(24, frameDelay - 4, (weatherStringZ));    // show text
    fillScreen(0);
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
    setCursor(22,0);
    setTextColor(myGREEN);
    print(getMonthTxt());
    
    setFont();
    setCursor(49,15);
    setTextColor(myGREEN);
    setFont(&kongtext4pt7b);
    if (getSec() < 10) print("0" + String(getSec()));
    else print(getSec());

    setFont();
    setCursor(1, 15);
    setTextColor(myGREEN);
    fillRect(0, 8, 20, 15, myBLACK);
    setFont(&FreeSansBold9pt7b);
    setTextSize(1);
    print(getTime());

    sendStringToMtx(utf8rus("Test TEST").c_str());
    sendStringToMtx(utf8rus("ПРОВЕРКА Проверка").c_str());
    sendStringToMtx(utf8rus("Сегодня отличная погода!").c_str());
  // }

}



void MTX::sendStringToMtx(const char* text, bool forcePrint, bool clearQueue, int8_t textOffset, const int16_t fixedPos)
{
  // if((!flags.ONflag && !forcePrint) || (isAlarm() && !forcePrint)) return; // если выключена, или если будильник, но не задан принудительный вывод - то на выход
  if(textOffset==-128) textOffset=this->txtOffset;

  if(text==nullptr){ // текст пустой
    if(!isStringPrinting){ // ничего сейчас не печатается
      if(!docArrMessages){ // массив пустой
        return; // на выход
      }
      else { // есть что печатать
        JsonArray arr = (*docArrMessages).as<JsonArray>(); // используем имеющийся
        JsonObject var=arr[0]; // извлекаем очередной
        if(!var.isNull()){
          String storage = var[F("s")];
          prepareText(storage);
          doPrintStringToMtx(storage.c_str(), (var[F("o")].as<int>()), (var[F("f")].as<int>())); // отправляем
#ifdef MP3PLAYER
          String tmpStr = var[F("s")];
          if(mp3!=nullptr && ((mp3->isOn() && isLampOn()) || isAlarm()) && flags.playTime && tmpStr.indexOf(String(F("%TM")))>=0)
            mp3->playTime(embui.timeProcessor.getHours(), embui.timeProcessor.getMinutes(), (TIME_SOUND_TYPE)flags.playTime);
#endif
        }
        if(arr.size()>0)
          arr.remove(0); // удаляем отправленный
        if(!arr.size()){ // очередь опустела, освобождаем массив
          delete docArrMessages;
          docArrMessages = nullptr;
        }
      }
    } else {
        // текст на входе пустой, идет печать
        return; // на выход
    }
  } else { // текст не пустой
    if(clearQueue){
      LOG(println, F("Clear message queue"));
      if(docArrMessages){ // очистить очередь, освободить память
          delete docArrMessages;
          docArrMessages = nullptr;
      }
      isStringPrinting = false; // сбросить текущий вывод строки
    }

    if(!isStringPrinting){ // ничего сейчас не печатается
      String storage = text;
      prepareText(storage);
      doPrintStringToMtx(storage.c_str(), textOffset, fixedPos); // отправляем
#ifdef MP3PLAYER
      String tmpStr = text;
      if(mp3!=nullptr && ((mp3->isOn() && isLampOn()) || isAlarm()) && flags.playTime && tmpStr.indexOf(String(F("%TM")))>=0)
        mp3->playTime(embui.timeProcessor.getHours(), embui.timeProcessor.getMinutes(), (TIME_SOUND_TYPE)flags.playTime);
#endif
    } else { // идет печать, помещаем в очередь
      JsonArray arr; // добавляем в очередь

      if(docArrMessages){
        arr = (*docArrMessages).as<JsonArray>(); // используем имеющийся
      } else {
        docArrMessages = new DynamicJsonDocument(512);
        arr = (*docArrMessages).to<JsonArray>(); // создаем новый
      }

      for (size_t i = 0; i < arr.size(); i++)
      {
        if((arr[i])[F("s")]==text
        && (arr[i])[F("o")]==textOffset
           && (arr[i])[F("f")]==fixedPos
        ){
          LOG(println, F("Duplicate string skipped"));
          //LOG(println, (*docArrMessages).as<String>());
          return;
        }
      }

      JsonObject var = arr.createNestedObject();
      var[F("s")]=text;
      var[F("o")]=textOffset;
      var[F("f")]=fixedPos;

      String tmp; // Тут шаманство, чтобы не ломало JSON
      serializeJson((*docArrMessages), tmp);
      deserializeJson((*docArrMessages), tmp);

      LOG(print, F("Array: "));
      LOG(println, (*docArrMessages).as<String>());
    }
  }
}

void MTX::doPrintStringToMtx(const char* text, const int8_t textOffset, const int16_t fixedPos)
{
  static String toPrint;
  setTextColor(myMAGENTA);

  if(!isStringPrinting){
    toPrint.clear();
    // fillRect(0, 8, 64, 8, myBLACK);    //
  }

  isStringPrinting = true;
  int8_t offs=(textOffset==-128?txtOffset:textOffset);
  if(text!=nullptr && text[0]!='\0'){
    toPrint.concat(text);
    // _letterColor = letterColor;
  }

  if(toPrint.length()==0) {
    isStringPrinting = false;
    return; // нечего печатать
  } else {
    isStringPrinting = true;
  }

  if(tmStringStepTime.isReadyManual()){
    if(!fillStringManual(toPrint.c_str(), false, 0, fixedPos, 1, offs)){ // смещаем
      tmStringStepTime.reset();
    }
    else {
      isStringPrinting = false;
      toPrint.clear(); // все напечатали
      sendStringToMtx(); // получаем новую порцию
    } 
  //   // if((!isWarning() || (isWarning() && fixedPos)))
      fillStringManual(toPrint.c_str(), true);
  }
}

typedef enum {FIRSTSYMB=1,LASTSYMB=2} SYMBPOS;

bool MTX::fillStringManual(const char* text,  bool stopText, bool isInverse, int32_t pos, int8_t letSpace, int8_t txtOffset, int8_t letWidth, int8_t letHeight)
{
  static int32_t offset = MATRIX_WIDTH;
  int16_t lenght = strlen(text);

  if (!text || !strlen(text))
  {
    return true;
  }

   setFont();
   setCursor(offset, 24);
   setFont(&Arkhip6pt8b);
    println(text);
  swapBuffers(true);
  if(!stopText) {
    offset--;   ///////// МЕНЯТЬ
    // LOG(println, F("TEST?"));
    }
  if ((offset + lenght * 7 < 0 ))
  {
    offset = MATRIX_WIDTH;
    lenght = 0;
    return true;
  }
  // if(pos) // если задана позиция, то считаем что уже отобразили
  // {
  //   offset = (flags.MIRR_V ? 0 : WIDTH);
  //   return true;
  // }

  return false;
}


String &MTX::prepareText(String &source){
  source.replace(F("%TM"), embui.timeProcessor.getFormattedShortTime());
  source.replace(F("%IP"), WiFi.localIP().toString());
  // source.replace(F("%EN"), effects.getEffectName());
  const tm *tm = localtime(embui.timeProcessor.now());
  char buffer[11]; //"xx.xx.xxxx"
  sprintf_P(buffer,PSTR("%02d.%02d.%04d"),tm->tm_mday,tm->tm_mon+1,tm->tm_year+TM_BASE_YEAR);
  source.replace(F("%DT"), buffer);
// #ifdef MATRIX_DEBUG  
    LOG(println, source.c_str()); // вывести в лог строку, которая после преобразований получилась
// #endif
  return source;  
}

// void MTX::scrollText(String text, int scroll_delay)
// {
//   uint16_t text_length = text.length();
//   int ypos = 24;
//     if (int xpos = MATRIX_WIDTH; xpos > -(MATRIX_WIDTH + text_length * 5); xpos--)
//   {
//     setFont();
//     setCursor(xpos, ypos);                //
//     fillRect(0, ypos, 64, 8, myBLACK);    //
//     setTextColor(myMAGENTA);              //
//     print(utf8rus(text));                 //
//     setFont();                            //
//     delay(scroll_delay);                         //
//     // yield();
//   }
  
// }


void MTX::getScreen(){
  
  // static unsigned long showScreen;
  if (nightMode) drawRGBBitmap(0, 0, image_data_noch2, 64, 32);
  else drawRGBBitmap(0, 0, image_data_ytro2, 64, 32);
}


void MTX::getNightMode(){
  fillScreen(0);
  setFont();
  setCursor(8, 16);
  setTextSize(1);
  setFont(&FreeSansBold9pt7b);
  setTextColor(Color333(0,0,2));
  println(getTime());
  // sendStringToMtx("TEST TEST TEST");
  // sendStringToMtx("Сегодня отличная погода!");

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

// char * MTX::utf8rus(String source)
// {
//   int i,k;
//   // String target;
//   unsigned char n;
//   char m[2] = { '0', '\0' };
//   k = source.length(); i = 0;
//   char target[100];

//   while (i < k) {
//     n = source[i]; i++;

//     if (n >= 0xC0) {
//       switch (n) {
//         case 0xD0: {
//           n = source[i]; i++;
//           if (n == 0x81) { n = 0xA8; break; }
//           if (n >= 0x90 && n <= 0xBF) n = n + 0x30;
//           break;
//         }
//         case 0xD1: {
//           n = source[i]; i++;
//           if (n == 0x91) { n = 0xB8; break; }
//           if (n >= 0x80 && n <= 0x8F) n = n + 0x70;
//           break;
//         }
//       }
//     }
//     m[0] = n; target = target + char(m);
//   }
// return target;
// }