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
#include "config.h"

int ICACHE_FLASH_ATTR cgiSensorSettings(HttpdConnData *connData) {
	int len;
	char buff[128];

	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	len=httpdFindArg(connData->post->buff, "sensor-ds18b20-enable", buff, sizeof(buff));
	sysCfg.sensor_ds18b20_enable = (len > 0) ? 1:0;

	len=httpdFindArg(connData->post->buff, "sensor-dht22-enable", buff, sizeof(buff));
	sysCfg.sensor_dht22_enable = (len > 0) ? 1:0;

	len=httpdFindArg(connData->post->buff, "sensor-bmp180-enable", buff, sizeof(buff));
	sysCfg.sensor_bmp180_enable = (len > 0) ? 1:0;

	CFG_Save();
	httpdRedirect(connData, "/");
	return HTTPD_CGI_DONE;
}

void ICACHE_FLASH_ATTR tplSensorSettings(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	if (token==NULL) return;

	os_strcpy(buff, "Unknown");

	if (os_strcmp(token, "sensor-ds18b20-enable")==0) {
		os_strcpy(buff, sysCfg.sensor_ds18b20_enable == 1 ? "checked" : "" );
	}

	if (os_strcmp(token, "sensor-dht22-enable")==0) {
		os_strcpy(buff, sysCfg.sensor_dht22_enable == 1 ? "checked" : "" );
	}

	if (os_strcmp(token, "sensor-bmp180-enable")==0) {
		os_strcpy(buff, sysCfg.sensor_bmp180_enable == 1 ? "checked" : "" );
	}

	httpdSend(connData, buff, -1);
}
