#include "config.h"
#include "matrix.h"
#include "image.h"
#include "main.h"
#include "weather.h"
#include "sensors.h"

MTX matrix;

// Инициализация матрицы
void MTX::init()
{
    begin();
    cp437(true);
    setRotation(0); 
    setTextWrap(false);
    LOG(printf_P, PSTR("Matrix was initialized \n"));
    fillScreen(0);
    tmStringStepTime.setInterval(scroll_interval);
  }

// Обработчик матрицы
void MTX::handle() {
  if(NIGHTMODE_TIME <= getHour() || MORNING_TIME > getHour()) nightMode = true; 
  else nightMode = false;
  static uint32_t weather_home_switch;
  static uint32_t weatherTimer;
  if (!mtxStarted) start();
  if (!mtxStarted) return;
  if (isStringPrinting) doPrintStringToMtx();
  static uint32_t wait_handlers;
  if (wait_handlers + 500U > millis())
  return;
  wait_handlers = millis();
  if (getHour() == NIGHTMODE_TIME && getMin() == 0 && getSec() < 5) getScreen();
  else if (getHour() == MORNING_TIME && getMin() == 0 && getSec() < 5) getScreen();
  else 
  {
    if (nightMode) getNightMode();
    else 
    {
      if (weather_home_switch + 30*1000 > millis() || isStringPrinting) 
      {
        getClock();
        if (showWthTxt && millis() > 10*1000) 
        {
          if (!switchHome)
          {
            sendStringToMtx(String("Сегодня " + String(getMDay()) + "  " + getMonthTxt() + "  " + String(getYear()) + " года " + getWDayTxt()).c_str());  // show text
            if (weather.isWeatherCheck() > 1) sendStringToMtx(weather.showToday().c_str());
          } else {
            if (weather.isWeatherCheck() > 1) sendStringToMtx(weather.showTomorrow().c_str());
            sendStringToMtx(weather.showNarodmon().c_str());
          }

          showWthTxt = false;
        }
        weatherTimer = millis();
      }
      if (weather_home_switch + 30*1000 < millis() && !isStringPrinting) 
      {
        if(switchHome) getHome();
        else getWeather();
        swapBuffers(true);
        if (weatherTimer + 15*1000 > millis()) 
        return;
        weather_home_switch = millis();
        showWthTxt = true;
        switchHome=!switchHome;
      } 
    }
  }
  // LOG(printf_P, PSTR("CORE CORE CORE === %d \n"), xPortGetCoreID());
  if (!isStringPrinting) swapBuffers(true);
}

/// Стартовая анимация часов (с подключением к WiFi и отображением IP)
void MTX::start() {
  static unsigned long wait_handlers;
  if (wait_handlers + animInterv > millis())
    return;
  wait_handlers = millis();
  static unsigned long showIp;
  static uint8_t l;
  static uint8_t i;
  l++;
  swapBuffers(true);
  // setFont();
  // setTextColor(myCYAN);
  if (!i) {
    if (!embui.sysData.wifi_sta && millis() < 5000){

      drawRGBBitmap(0, 0, image_wifi, 64, 32);
      switch (l) {
        case 1:
          fillRect(0, 0, 64, 25, myBLACK);
          break;
        
        case 2:
          fillRect(0, 0, 64, 17, myBLACK);
          fillRect(0, 0, 22, 32, myBLACK);
          fillRect(42, 0, 20, 32, myBLACK);
          break;

        case 3:
          fillRect(0, 0, 64, 9, myBLACK);
          fillCircle(13, 7, 5, myBLACK);
          fillCircle(50, 7, 5, myBLACK);
          break;
        
        case 4:
          l = 0;
          break;
      }
      #ifdef MP3PLAYER
      if(playAlert) dfPlayer.playFolder(wifi_connecting);
      #endif
      LOG(printf_P, PSTR("WiFi connecting... \n"));
      swapBuffers(true);
    } else {
      #ifdef MP3PLAYER
      if(playAlert) dfPlayer.playFolder(wifi_connected);
      #endif

      fillScreen(0);
      setFont();
      setCursor(13, 5);
      setTextColor(embui.sysData.wifi_sta ? myGREEN : myRED);
      println(embui.sysData.wifi_sta ? "WiFi OK" : "WiFi AP");
      uint8_t x;
      x = (62 - (WiFi.localIP().toString().length() - 3) * 5) / 2;   // Расчитываем длину айпи и делаем вывод примерно по центру оси Х (+- пару пикселей, т.к. у шрифта цифры разной ширины)
      setCursor((embui.sysData.wifi_sta ? x : 12), 18);
      setFont(&TomThumb);
      setTextColor(myBLUE);
      println(embui.sysData.wifi_sta ? WiFi.localIP().toString() : "192.168.4.1");
      if (!showIp) showIp = millis();
    }
  }

    if (!showIp) return;
    if (showIp + 4000 > millis()) return;

    if (!i) fillScreen(0);

    switch (i){

      case 0:
        drawRect(16, 15, 32, 2, myBLUE);
        break;

      case 1: 
        drawRect(13, 13, 38, 6, myBLUE);
        drawRect(12, 12, 40, 8, myBLUE);
        break;

      case 2: 
        drawRect(9, 9, 46, 14, myBLUE);
        drawRect(8, 8, 48, 16, myBLUE);
        break;

      case 3:
        drawRect(5, 5, 54, 22, myBLUE);
        drawRect(4, 4, 56, 24, myBLUE);
        break;

      case 4: 
        drawRect(1, 1, 62, 30, myBLUE);
        drawRect(0, 0, 64, 32, myBLUE);
        break;

      case 5: 
        drawRect(16, 15, 32, 2, myBLACK);
        break;

      case 6: 
        drawRect(13, 13, 38, 6, myBLACK);
        drawRect(12, 12, 40, 8, myBLACK);
        break;

      case 7: 
        drawRect(9, 9, 46, 14, myBLACK);
        drawRect(8, 8, 48, 16, myBLACK);
        break;

      case 8:
        drawRect(5, 5, 54, 22, myBLACK);
        drawRect(4, 4, 56, 24, myBLACK);
        break;

      case 9:
        drawRGBBitmap(2, 4, image_10, 60, 24);
        if (!weather.isWeatherCheck()) weather.setWeatherChek();
        break;
    }
    i++;
    if ((i == 15 && !embui.sysData.wifi_sta) || (weather.isWeatherCheck() == 4 && embui.sysData.wifi_sta)) mtxStarted = true;

  // }
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

  // Выводим надпись "погода"
  fillScreen(0);
  setFont();
  setCursor(13, -1);
  setTextColor(myYELLOW);
  setFont(&Heebo7pt8b);
  println("погода");

  // Погода на сегодня
  setFont();
  if (getMDay() < 10) setCursor(7, 7); 
  else setCursor(2, 7);
  setTextSize(1);
  setFont(&TomThumb);
  setTextColor(myGREEN);
  print(getMDay());

  setFont();
  setCursor(2, 25);
  setTextSize(1);
  setFont(&TomThumb);
  setTextColor(myRED);
  print(weather.getWeathTemp());

  // Погода на завтра
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

  // Время
  setFont();
  setCursor(21, 9);
  setTextSize(1);
  setFont(&TomThumb);
  setTextColor(myRED);
  println(getTime());
  setFont();

  // Берем изображение погоды
  getImage();
    
}


/// Экран "дома"
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
  setCursor(4, -1);
  setFont(&Heebo7pt8b);
  println("дома");

  setTextSize(1);
  setFont(&TomThumb);
  setCursor(13, 15);
  setTextColor(myRED);
  println(sens.getPress()+ " mmMs");

  setCursor(10, 28);
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
  } else {
  drawRGBBitmap(0, 7, image_data_Image25, 12, 12);
  drawRGBBitmap(0, 20, image_data_Image23, 12, 12);
  drawRGBBitmap(38, 20, image_data_Image27, 10, 12);
  drawRGBBitmap(52, 9, image_data_Image29, 12, 12);
  }

}

/// Главный экран часов (дневной режим)
void MTX::getClock(){
  // Будет крутить подготовку за 2 минуты до смены на ночной режим. 
  if (getHour() == NIGHTMODE_TIME - 1 && getMin() >= 58){
    drawLine(55, 8, 63, 8, myBLACK);
    drawLine(55, 7, 63, 7, myBLACK);
    sendStringToMtx("Подготовка к ночному режиму");
  }

  fillRect(0, 0, 64, 22, myBLACK); // Очищаем экран (не затрагивая бегущую строку)

  // Блок вывода дня недели (котороткий)
  setFont();
  setCursor(49, 8);
  // drawLine(55, 7, 9, 7, myBLACK);
  if (getWDay() == 0 || getWDay() == 6) setTextColor(myRED);
  else setTextColor(myBLUE);
  setFont(&kongtext4pt7b);
  print(getWDayShort());

  //  Вывод дня месяца и месяца (текст)
  setFont();
  static uint8_t x;
  x = (62 - (getMDay() < 10 ? 7 : 14) - (getMonthTxt().length() / 2) * 7) / 2;  // Центрируем вывод дня и месяца
  setCursor(x, 0);
  setTextColor(myYELLOW);
  setFont(&kongtext4pt7b);
  print(getMDay());

  //  Вывод месяца (текст)
  setFont();
  setCursor(getMDay() < 10 ? (x + 10) : (x + 17), 0);
  setFont(&Heebo7pt8b);
  setTextColor(myGREEN);
  print(getMonthTxt());  
    
  //  Вывод секунд
  setFont();
  setCursor(49,15);
  setTextColor(myGREEN);
  setFont(&kongtext4pt7b);
  if (getSec() < 10) print("0" + String(getSec()));
  else print(getSec());

  // Вывод часов
  setFont();
  setCursor(1, 15);
  if (getTime() == "-- --" || getTime() == "--:--" ) setCursor(10, 15);
  setTextColor(myGREEN);
  fillRect(0, 8, 20, 15, myBLACK);
  setFont(&FreeSansBold9pt7b);
  setTextSize(1);
  print(getTime());

  if (!embui.sysData.wifi_sta) drawRGBBitmap(0, 24, image_wifi_mini, 9, 8);

}

/// Функция отправки строки на матрицу
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
        docArrMessages = new DynamicJsonDocument(1536);
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
    if(!fillStringManual(toPrint.c_str(), false, 0, fixedPos, 0, offs)){ // смещаем
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
    if (!embui.sysData.wifi_sta) fillRect(9, 22, 64, 12, myBLACK);
    else fillRect(0, 22, 64, 12, myBLACK);
    setCursor(offset, 24);
    // setFont(&Heebo7pt8b);
    print(text);
    if (!embui.sysData.wifi_sta) drawRGBBitmap(0, 24, image_wifi_mini, 9, 8);
    swapBuffers(true);
  if(!stopText) {
    offset--;   
    }
  if ((offset + lenght * 5 < 0 ))
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

///   Заставка переключения дневного и ночного режима
void MTX::getScreen(){
  if (nightMode) drawRGBBitmap(0, 0, image_data_noch2, 64, 32);
  else drawRGBBitmap(0, 0, image_data_ytro2, 64, 32);
}

///   Ночной режим
void MTX::getNightMode(){
  fillScreen(0);
  setFont();
  setCursor(8, 16);
  if (getTime() == "-- --" || getTime() == "--:--" ) setCursor(19, 15);
  setTextSize(1);
  setFont(&FreeSansBold9pt7b);
  setTextColor(Color333(0,0,2));
  println(getTime());
  if (!embui.sysData.wifi_sta) drawRGBBitmap(0, 24, image_wifi_mini, 9, 8);

  if (MORNING_TIME == getHour()) showMorning = true;
}

/// Изображения значков погоды для экрана getWeather() (weatherbit.io)
void MTX::getImage() {
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
    
    if (switchAnim) drawRGBBitmap(10, 14, image_data_Image2, 10, 10); else drawRGBBitmap(10, 14, image_data_Image3, 10, 10);
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
    break;
    
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

    if (switchAnim) drawRGBBitmap(44, 14, image_data_Image2, 10, 10); else drawRGBBitmap(44, 14, image_data_Image3, 10, 10);
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



// // ================================ Вывод Русских Букв
// String MTX::utf8rus(String source)
// {
//   int i,k;
//   String target;
//   unsigned char n;
//   char m[2] = { '0', '\0' };

//   k = source.length(); i = 0;

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
//     m[0] = n; target = target + String(m);
//   }
// return target;
// }

