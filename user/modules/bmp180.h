/*
    The driver for the temperature sensor and pressure BMP180
    Official repository: https://github.com/CHERTS/esp8266-i2c_bmp180
    Base on https://github.com/reaper7/esp8266_i2c_bmp180
    This driver depends on the I2C driver https://github.com/zarya/esp8266_i2c_driver/

    Copyright (C) 2014 reaper7
    Copyright (C) 2014 Mikhail Grigorev (CHERTS)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/
#ifndef BMP180_H
#define BMP180_H
#include "c_types.h"
#include "ets_sys.h"
#include "osapi.h"

enum PRESSURE_RESOLUTION {
	OSS_0 = 0,
	OSS_1,
	OSS_2,
	OSS_3
};

bool BMP180_Init(void);
int32_t BMP180_GetTemperature( int32_t *T );
int32_t BMP180_GetPressure(enum PRESSURE_RESOLUTION resolution, int32_t *P );
int32_t BMP180_CalcAltitude(int32_t pressure);

#endif // BMP180_H
