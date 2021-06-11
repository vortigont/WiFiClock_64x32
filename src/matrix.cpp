#include "config.h"
#include "matrix.h"
#include "image.h"
#include "main.h"

MTX matrix;


void MTX::init()
{
    begin();
    cp437(true);
    setRotation(0); 
    setTextWrap(false);
    fillScreen(0);
    LOG(printf_P, PSTR("Matrix was initialized \n"));

  }

void MTX::handle()
{
  if (!mtxStarted) start();
  else {
    static unsigned long wait_handlers;
    if (wait_handlers + 500U > millis())
    return;
    wait_handlers = millis();
    getWeather();
    // if (nightMode) getWeather();
    // else getHome();
}
if(NIGHTMODE_TIME == getHour()) nightMode = true; 
}

void MTX::start()
{
  static unsigned long wait_handlers;
  if (wait_handlers + animInterv > millis())
    return;
  wait_handlers = millis();
  static unsigned long showIp;
  static int l;
  if (l==0){
  setTextColor(myCYAN);
  setCursor(2, 0);
  setFont();
  print("WiFi");
  }
  if (!embui.sysData.wifi_sta && l < 6){
  print(".");
  swapBuffers(true);
  #ifdef MP3PLAYER
  if(playAlert) dfPlayer.playFolder(wifi_connecting);
  #endif
  LOG(printf_P, PSTR("WiFi connecting... \n"));
  }
  else {
    fillScreen(0);
    #ifdef MP3PLAYER
    if(playAlert) dfPlayer.playFolder(wifi_connected);
    #endif
    if (l <=6) {
      showIp = millis();
      setFont();
      setCursor(2, 0);
      println(embui.sysData.wifi_sta ? "WiFi OK" : "WiFi AP");
      setCursor(1, 10);
      setFont(&TomThumb);
      println(embui.sysData.wifi_sta ? WiFi.localIP().toString() : "192.168.4.1");
      swapBuffers(true);
      l++;
    }
    if (showIp + 2000 > millis()) return;
    switch (l){
    case 7: drawRGBBitmap(18, 3, image_data_Image30, 26, 26);
    break;
    case 8: drawRGBBitmap(18, 3, image_data_Image31, 26, 26);
    break;
    case 9: drawRGBBitmap(18, 3, image_data_Image32, 26, 26);
    break;
    case 10: drawRGBBitmap(18, 3, image_data_Image33, 26, 26);
    break;
    case 11: drawRGBBitmap(18, 3, image_data_Image34, 26, 26);
    break;
    case 12: drawRGBBitmap(18, 3, image_data_Image35, 26, 26);
    break;
    case 13: drawRGBBitmap(18, 3, image_data_Image36, 26, 26);
    break;
    }
    l++;
    swapBuffers(true);
    if (l==15) mtxStarted = true;
  }
}
 
//Цикл вывода времени
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
  print("+25c");
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
  print("+20c");
  setFont();
  setCursor(21, 9);
  setTextSize(1);
  setFont(&TomThumb);
  setTextColor(myRED);
  println(getTime());
  swapBuffers(true);
  setFont();
  getImage();
  fillScreen(0);


}

void MTX::getImage()
{
  switchAnim=!switchAnim;
  switch (800) {            // задать с монитора
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
     swapBuffers(true);
     
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
     swapBuffers(true);
     
        break;
      case 313: // Ливень, дождь и изморозь
      case 314: // Сильный ливень дождь и изморозь
      case 502: // Сиильный дождь
      case 503: // Очень сильный дождь
      case 504: // Сильный дождь
      case 520: // Темно, ливень
      case 522: // Сильный пролевной дождь
     
     if (switchAnim) drawRGBBitmap(10, 14, image_data_Image8, 10, 10); else drawRGBBitmap(10, 14, image_data_Image9, 10, 10);
     swapBuffers(true);
     
       break;
      
      case 601: // Снег
      case 602: // Сильный снегопад
      case 622: // Сильный снегопад
      
     if (switchAnim) drawRGBBitmap(10, 14, image_data_Image4, 10, 10); else drawRGBBitmap(10, 14, image_data_Image5, 10, 10);
     swapBuffers(true);
     
      case 600: //легкий снег
      case 610: // Снег с дождем
      case 611: // Мокрый снег
      case 612: // Легкий дождь со снегом
      case 615: //Небольшой дождь и снег
      case 616: // Дождь и снег
      case 620: // Легкий снегопад
      case 621: // Ливень и снег
      
     if (switchAnim) drawRGBBitmap(10, 14, image_data_Image6, 10, 10); else drawRGBBitmap(10, 14, image_data_Image7, 10, 10);
     swapBuffers(true);
     
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
     swapBuffers(true);
     
      //}
        break;
      case 800: // Ясно, чистое небо
      if (getHour() < 21 && getHour() > 6){ 
     if (switchAnim) drawRGBBitmap(10, 14, image_data_Image, 10, 10); else drawRGBBitmap(10, 14, image_data_Image1, 10, 10);
     swapBuffers(true);
     
      }else{
      if (switchAnim) drawRGBBitmap(10, 14, image_data_Image20, 10, 10); else drawRGBBitmap(10, 14, image_data_Image21, 10, 10);
     swapBuffers(true);
     
         
      }
       break;
      case 801: // Мало облачно 25%
      case 802: // Облано 50%
      case 803: // Облачно 80%
      if (getHour() < 21 && getHour() > 6){
      if (switchAnim) drawRGBBitmap(10, 14, image_data_Image16, 10, 10); else drawRGBBitmap(10, 14, image_data_Image17, 10, 10);
      swapBuffers(true);
    
      }else{
      if (switchAnim) drawRGBBitmap(10, 14, image_data_Image18, 10, 10); else drawRGBBitmap(10, 14, image_data_Image19, 10, 10);
     swapBuffers(true);
      
      }
       break;
      case 804: // Пасмурно 100%
       
     if (switchAnim) drawRGBBitmap(10, 14, image_data_Image12, 10, 10); else drawRGBBitmap(10, 14, image_data_Image13, 10, 10);
     swapBuffers(true);
     
        break;
  }
    switch (801) {      //ЗАДАТЬ С МОНИТОРА
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
     swapBuffers(true);
     
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
     swapBuffers(true);
     
        break;
      case 313: // Ливень, дождь и изморозь
      case 314: // Сильный ливень дождь и изморозь
      case 502: // Сиильный дождь
      case 503: // Очень сильный дождь
      case 504: // Сильный дождь
      case 520: // Темно, ливень
      case 522: // Сильный пролевной дождь
     
     if (switchAnim) drawRGBBitmap(44, 14, image_data_Image8, 10, 10); else drawRGBBitmap(44, 14, image_data_Image9, 10, 10);
     swapBuffers(true);
     
       break;
      
      case 601: // Снег
      case 602: // Сильный снегопад
      case 622: // Сильный снегопад
      
     if (switchAnim) drawRGBBitmap(44, 14, image_data_Image4, 10, 10); else drawRGBBitmap(44, 14, image_data_Image5, 10, 10);
     swapBuffers(true);
     
      case 600: //легкий снег
      case 610: // Снег с дождем
      case 611: // Мокрый снег
      case 612: // Легкий дождь со снегом
      case 615: //Небольшой дождь и снег
      case 616: // Дождь и снег
      case 620: // Легкий снегопад
      case 621: // Ливень и снег
      
     if (switchAnim) drawRGBBitmap(44, 14, image_data_Image6, 10, 10); else drawRGBBitmap(44, 14, image_data_Image7, 10, 10);
     swapBuffers(true);
     
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
     swapBuffers(true);
     
      //}
        break;
      case 800: // Ясно, чистое небо
       
     if (switchAnim) drawRGBBitmap(44, 14, image_data_Image, 10, 10); else drawRGBBitmap(44, 14, image_data_Image1, 10, 10);
     swapBuffers(true);
     
      
       break;
      case 801: // Мало облачно 25%
      case 802: // Облано 50%
      case 803: // Облачно 80%
      
     if (switchAnim) drawRGBBitmap(44, 14, image_data_Image16, 10, 10); else drawRGBBitmap(44, 14, image_data_Image17, 10, 10);
     swapBuffers(true);
    
       break;
      case 804: // Пасмурно 100%
       
     if (switchAnim) drawRGBBitmap(44, 14, image_data_Image12, 10, 10); else drawRGBBitmap(44, 14, image_data_Image13, 10, 10);
     swapBuffers(true);
     
        break;
  }
  
}

// void MTX::getHome(){
//   drawLine(55, 8, 63, 8, myBLACK);
//   if (getHour() == NIGHTMODE_TIME - 1 && getMin() >= 58){
//       drawLine(55, 8, 63, 8, myBLACK);
//       drawLine(55, 7, 63, 7, myBLACK);
//       // scroll_text(24, frameDelay, ("Подготовка к ночному режиму"));
//   }
//   else {
//     fillRect(46, 10, 2, 13, myBLACK);
//     drawLine(0, 7, 63, 7, myBLACK);
//     setTextColor(myMAGENTA);

//     // print("Сегодня " + getMDay() +  getMonth() + "  " + getYear() + " года " + getWDay());  // show text
//     // scroll_text(24, frameDelay - 4, (weatherString));       // show text
//     // scroll_text(24, frameDelay - 4, (weatherStringZ));    // show text
//   }

// }

void MTX::getClock(){

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
  static unsigned long wait_handlers;
  static unsigned long goodMorning;
  if (wait_handlers + 999U > millis())
  return;
  wait_handlers = millis();
  if (showMorning) {
    fillRect(0, 0, 64, 32, myBLACK);
    drawRGBBitmap(0, 0, image_data_ytro2, 64, 32);
    swapBuffers(false);
    fillScreen(0);
    goodMorning = millis();
    showMorning = false;
  }
  if (goodMorning + 5000U > millis()) return;
  }


void MTX::getNightMode(){
  static unsigned long wait_handlers;
  static unsigned long goodNight;
  static bool l;
  if (wait_handlers + 999U > millis())
    return;
  wait_handlers = millis();
  if (!l){
  fillRect(0, 0, 64, 32, myBLACK); 
  drawRGBBitmap(0, 0, image_data_noch2, 64, 32);
  swapBuffers(false); 
  l++;
  goodNight=millis();
  }
  if (goodNight + 3000 > millis()) return;
  fillRect(0, 0, 64, 32, myBLACK);    
  fillScreen(0);
  setFont();
  setCursor(8, 16);
  setTextSize(1);
  setFont(&FreeSansBold9pt7b);
  setTextColor(Color333(0,0,2));
  println(getTime());
  swapBuffers(true);
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
