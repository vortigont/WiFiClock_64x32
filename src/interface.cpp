#include "main.h"

#include "EmbUI.h"
#include "interface.h"

#include "uistrings.h"   // non-localized text-strings

// #include <ESP32-targz.h>
// #define DEST_FS_USES_LITTLEFS

/**
 * можно нарисовать свой собственный интефейс/обработчики с нуля, либо
 * подключить статический класс с готовыми формами для базовых системных натсроек и дополнить интерфейс.
 * необходимо помнить что существуют системные переменные конфигурации с зарезервированными именами.
 * Список имен системных переменных можно найти в файле "constants.h"
 */
#include "basicui.h"

/**
 * переопределяем метод из фреймворка, регистрирующий необходимы нам в проекте переменные и методы обработки
 * 
 */
void create_parameters(){
    LOG(println, "UI: Creating application vars");

   /**
    * регистрируем статические секции для web-интерфейса с системными настройками,
    * сюда входит:
    *  - WiFi-manager
    *  - установка часового пояса/правил перехода сезонного времени
    *  - установка текущей даты/времени вручную
    *  - базовые настройки MQTT
    *  - OTA обновление прошивки и образа файловой системы
    */
    basicui::add_sections();

    /**
     * регистрируем свои переменные
     */
    embui.var_create(V_LED, "1");    // LED default status is on
    embui.var_create(V_VAR1, "");    // заводим пустую переменную по умолчанию

    /**
     * добавляем свои обрабочки на вывод UI-секций
     * и действий над данными
     */
    embui.section_handle_add(T_DEMO, block_demopage);                // generate "Demo" UI section

    // обработчики
    embui.section_handle_add(T_SET_DEMO, action_demopage);           // обработка данных из секции "Demo"

    embui.section_handle_add(V_LED, action_blink);               // обработка рычажка светодиода

};

/**
 * Headlile section
 * this is an overriden weak method that builds our WebUI interface from the top
 * ==
 * Головная секция,
 * переопределенный метод фреймфорка, который начинает строить корень нашего Web-интерфейса
 * 
 */
void section_main_frame(Interface *interf, JsonObject *data){
  if (!interf) return;

  interf->json_frame_interface();

  interf->json_section_manifest(T_HEADLINE, 0, "0.1");       // app name/version manifest
  interf->json_section_end();

  block_menu(interf, data);                         // Строим UI блок с меню выбора других секций
  interf->json_frame_flush();

  if(!(WiFi.getMode() & WIFI_MODE_STA)){                      // если контроллер не подключен к внешней AP, сразу открываем вкладку с настройками WiFi
    LOG(println, "UI: Opening network setup section");
    basicui::block_settings_netw(interf, data);
  } else {
    block_demopage(interf, data);                   // Строим блок с demo переключателями
  }

  //block_more(interf, data);                     // у нас есть и другие блоки, но строить сразу все

};


/**
 * This code builds UI section with menu block on the left
 * 
 */
void block_menu(Interface *interf, JsonObject *data){
    if (!interf) return;
    // создаем меню
    interf->json_section_menu();

    /**
     * пункт меню - "демо"
     */
    interf->option(T_DEMO, "UI Demo");

    /**
     * добавляем в меню пункт - настройки,
     * это автоматически даст доступ ко всем связанным секциям с интерфейсом для системных настроек
     * 
     */
    basicui::opt_setup(interf, data);       // пункт меню "настройки"
    interf->json_section_end();
}

/**
 * Demo controls
 * 
 */
void block_demopage(Interface *interf, JsonObject *data){
    if (!interf) return;
    interf->json_frame_interface();

    // Headline
    // параметр T_SET_DEMO определяет зарегистрированный обработчик данных для секции
    interf->json_section_main(T_SET_DEMO, "Some demo controls");
    interf->comment("Комментарий: набор контролов для демонстрации");     // комментарий-описание секции

    // переключатель, связанный со светодиодом. Изменяется синхронно 
    interf->checkbox(V_LED, embui.paramVariant(V_LED), "Onboard LED", true);

//    interf->text(V_VAR1, embui.paramVariant(V_VAR1), "текстовое поле");                                 // текстовое поле со значением переменной из конфигурации
//    interf->text(V_VAR2, embui.paramVariant(V_VAR2), "some default val", "Второе текстовое поле");   // текстовое поле со значением "по-умолчанию"
    /*  кнопка отправки данных секции на обработку
     *  первый параметр T_DEMO определяет какая секция откроется
     *  после обработки отправленных данных
     */ 
    // interf->button_submit(T_SET_DEMO, T_DICT[TD::D_Send], P_GRAY);
    interf->json_section_end();
    interf->json_frame_flush();
}

void action_demopage(Interface *interf, JsonObject *data){
    if (!data) return;

    LOG(println, "porcessig section demo");

    // сохраняем значение 1-й переменной в конфиг фреймворка
    SETPARAM(V_VAR1);

    // выводим значение 1-й переменной в serial
    const char *text = (*data)[V_VAR1];
    Serial.printf_P(PSTR("Varialble_1 value:%s\n"), text );

    // берем указатель на 2-ю переменную
    text = (*data)[V_VAR2];
    // или так:
    // String var2 = (*data)[V_VAR2];
    // выводим значение 2-й переменной в serial
    Serial.printf_P(PSTR("Varialble_2 value:%s\n"), text);

}


void action_blink(Interface *interf, JsonObject *data){
  if (!data) return;  // здесь обрабатывает только данные

  SETPARAM(V_LED);  // save new LED state to the config
  digitalWrite(LED_BUILTIN, !(*data)[V_LED].as<unsigned int>()); // write inversed signal for biuldin LED
  Serial.printf("LED: %d\n", (*data)[V_LED].as<unsigned int>());
}

/**
 * обработчик статуса (периодического опроса контроллера веб-приложением)
 */
void pubCallback(Interface *interf){
    basicui::embuistatus(interf);
}