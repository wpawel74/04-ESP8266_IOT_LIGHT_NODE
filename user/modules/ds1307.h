//////////////////////////////////////////////////
// I2C driver for DS1307 RTC for ESP8266.
// Copyright 2015 Richard A Burton
// richardaburton@gmail.com
// See license.txt for license terms.
//////////////////////////////////////////////////
#ifndef __DS1307_H__
#define __DS1307_H__

#include <time.h>

bool ds1307_setTime(struct tm *time);
bool ds1307_getOscillatorStopFlag();
bool ds1307_clearOscillatorStopFlag();
bool ds1307_enable32khz();
bool ds1307_disable32khz();
bool ds1307_enableSquarewave();
bool ds1307_disableSquarewave();
bool ds1307_getTime(struct tm *time);

#endif // __DS1307_H__
