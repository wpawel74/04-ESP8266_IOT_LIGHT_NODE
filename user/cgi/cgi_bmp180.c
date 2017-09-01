/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */
#include <esp8266.h>
#include <stdlib.h>
#include <string.h>
#include <osapi.h>
#include "cgi.h"
#include "bmp180.h"

void ICACHE_FLASH_ATTR tplBMP180(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	if (token==NULL) return;

	os_strcpy(buff, "Unknown");
	if (os_strcmp(token, "temperature")==0) {
		int32_t temp;
		if( BMP180_GetTemperature(&temp) == 0 )
			os_sprintf(buff, "%ld.%ld", temp/10, temp - ((temp/10)*10) );
	}
	if (os_strcmp(token, "pressure")==0) {
		int32_t pressure;
		if( BMP180_GetPressure(OSS_0, &pressure) == 0 )
			os_sprintf(buff, "%ld", pressure);
	}
	httpdSend(connData, buff, -1);
}

int ICACHE_FLASH_ATTR cgiBMP180(HttpdConnData *connData) {
	char buff[256];
	int32_t temp = 0, pressure = 0;
	BMP180_GetTemperature( &temp );
	BMP180_GetPressure(OSS_0, &pressure);

	httpdStartResponse(connData, 200);
	httpdHeader(connData, "Content-Type", "text/json");
	httpdHeader(connData, "Access-Control-Allow-Origin", "*");
	httpdEndHeaders(connData);

	os_sprintf(buff, "{ \n\"temperature\": \"%ld.%ld\"\n , \n\"pressure\": \"%ld\"\n}\n", temp/10, temp - ((temp/10)*10), pressure );

	httpdSend(connData, buff, -1);
	return HTTPD_CGI_DONE;
}
