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
		os_sprintf(buff, "%d", BMP180_GetTemperature() );
	}
	if (os_strcmp(token, "pressure")==0) {
		os_sprintf(buff, "%d", BMP180_GetPressure(OSS_0));
	}
	httpdSend(connData, buff, -1);
}

int ICACHE_FLASH_ATTR cgiBMP180(HttpdConnData *connData) {
	char buff[256];

	httpdStartResponse(connData, 200);
	httpdHeader(connData, "Content-Type", "text/json");
	httpdHeader(connData, "Access-Control-Allow-Origin", "*");
	httpdEndHeaders(connData);

	os_sprintf(buff, "{ \n\"temperature\": \"%d\"\n , \n\"pressure\": \"%d\"\n}\n", BMP180_GetTemperature(), BMP180_GetPressure(OSS_0) );

	httpdSend(connData, buff, -1);
	return HTTPD_CGI_DONE;
}
