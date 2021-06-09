#ifndef __CONFIG_H
#define __CONFIG_H
#if defined __has_include
#  if __has_include("user_config.h")
#    include "user_config.h" // <- пользовательские настройки, пожалуйста меняйте все что требуется там, ЭТОТ ФАЙЛ (config.h) НЕ МЕНЯЙТЕ
#  endif
#endif

#ifndef TIMEOUT
#define TIMEOUT       (500U)                   // таймаут сохранения конфигурации эффекта, по умолчанию - 20 секунд
#endif



#endif