//////////////////////////////////////////////////
// I2C driver for DS1307 RTC for ESP8266.
// Copyright 2015 Richard A Burton
// richardaburton@gmail.com
// See license.txt for license terms.
//////////////////////////////////////////////////

#ifndef __DS1307_H__
#define __DS1307_H__

#include <time.h>

bool ICACHE_FLASH_ATTR ds1307_setTime(struct tm *time);
bool ICACHE_FLASH_ATTR ds1307_getOscillatorStopFlag();
bool ICACHE_FLASH_ATTR ds1307_clearOscillatorStopFlag();
bool ICACHE_FLASH_ATTR ds1307_enable32khz();
bool ICACHE_FLASH_ATTR ds1307_disable32khz();
bool ICACHE_FLASH_ATTR ds1307_enableSquarewave();
bool ICACHE_FLASH_ATTR ds1307_disableSquarewave();
bool ICACHE_FLASH_ATTR ds1307_getTime(struct tm *time);

#endif // __DS1307_H__
