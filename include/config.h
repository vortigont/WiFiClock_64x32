#ifndef __CONFIG_H
#define __CONFIG_H
#include "Arduino.h"
#if defined __has_include
#  if __has_include("user_config.h")
#    include "user_config.h" // <- пользовательские настройки, пожалуйста меняйте все что требуется там, ЭТОТ ФАЙЛ (config.h) НЕ МЕНЯЙТЕ
#  endif
#endif

#ifndef TIMEOUT
#define TIMEOUT       (500U)                   // таймаут сохранения конфигурации эффекта, по умолчанию - 20 секунд
#endif



#ifdef STANDART_PINS
static uint8_t rgbpin[] = {25, 26, 27, 13, 14, 23};
#endif
#ifndef STANDART_PINS
static uint8_t rgbpin[] = {25, 27, 26, 13, 23, 14}; 
#endif

#ifndef PRESS_SYSTEM
#define PRESS_SYSTEM 0
#endif

#endif