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


	len=httpdFindArg(connData->post->buff, "thermostat1-input", buff, sizeof(buff));
	if (len>0) {
		sysCfg.thermostat1_input=atoi(buff);
	}

	len=httpdFindArg(connData->post->buff, "thermostat1-input", buff, sizeof(buff));
	if (len>0) {
		sysCfg.thermostat1_input=atoi(buff);
	}

	len=httpdFindArg(connData->post->buff, "thermostat1hysteresishigh", buff, sizeof(buff));
	if (len>0) {
		sysCfg.thermostat1hysteresishigh=atoi(buff);
	}

	len=httpdFindArg(connData->post->buff, "thermostat1hysteresislow", buff, sizeof(buff));
	if (len>0) {
		sysCfg.thermostat1hysteresislow=atoi(buff);
	}

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

	if (os_strcmp(token, "selectedds18b20")==0) {
		os_strcpy(buff, sysCfg.thermostat1_input == 0 ? "selected" : "" );
	}

	if (os_strcmp(token, "selecteddht22t")==0) {
		os_strcpy(buff, sysCfg.thermostat1_input == 1 ? "selected" : "" );
	}

	if (os_strcmp(token, "selecteddht22h")==0) {
		os_strcpy(buff, sysCfg.thermostat1_input == 2 ? "selected" : "" );
	}

	if (os_strcmp(token, "selectedmqtt")==0) {
		os_strcpy(buff, sysCfg.thermostat1_input == 3 ? "selected" : "" );
	}

	if (os_strcmp(token, "selectedserial")==0) {
		os_strcpy(buff, sysCfg.thermostat1_input == 4 ? "selected" : "" );
	}

	if (os_strcmp(token, "selectedfixed")==0) {
		os_strcpy(buff, sysCfg.thermostat1_input == 5 ? "selected" : "" );
	}

	if (os_strcmp(token, "thermostat1hysteresishigh")==0) {
		os_sprintf(buff,"%d", (int)sysCfg.thermostat1hysteresishigh);
	}

	if (os_strcmp(token, "thermostat1hysteresislow")==0) {
		os_sprintf(buff,"%d", (int)sysCfg.thermostat1hysteresislow);
	}

	httpdSend(connData, buff, -1);
}
