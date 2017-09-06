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

int ICACHE_FLASH_ATTR cgiSensorSettings(HttpdConnData *cd) {
	if ( cd->conn == NULL ) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	cgiCheckBox( cd, "sensor-ds18b20-enable", &config()->sensor_ds18b20_enable );
	cgiCheckBox( cd, "sensor-dht22-enable", &config()->sensor_dht22_enable );
	cgiCheckBox( cd, "sensor-bmp180-enable", &config()->sensor_bmp180_enable );

	CFG_Save();
	httpdRedirect(cd, "/");
	return HTTPD_CGI_DONE;
}

void ICACHE_FLASH_ATTR tplSensorSettings(HttpdConnData *cd, char *token, void **arg) {
	char buff[64];
	if (token == NULL) return;
	os_strcpy(buff, "Unknown");

	tplCheckBox( buff, token, "sensor-ds18b20-enable", config()->sensor_ds18b20_enable );
	tplCheckBox( buff, token, "sensor-dht22-enable", config()->sensor_dht22_enable );
	tplCheckBox( buff, token, "sensor-bmp180-enable", config()->sensor_bmp180_enable);
	httpdSend(cd, buff, -1);
}
