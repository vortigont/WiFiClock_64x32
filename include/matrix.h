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

extern void matrix_setup();
extern void matrix_loop();
#endif