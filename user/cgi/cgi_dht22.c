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
#include "dht22.h"

//Template code for the DHT 22 page.
void ICACHE_FLASH_ATTR tplDHT(HttpdConnData *cd, char *token, void **arg) {
	char buff[128];
	if (token==NULL) return;

	os_strcpy(buff, "Unknown");
	if (os_strcmp(token, "temperature")==0) {
		dht_temp_str(buff);
	}
	if (os_strcmp(token, "humidity")==0) {
		dht_humi_str(buff);
	}
	httpdSend(cd, buff, -1);
}

int ICACHE_FLASH_ATTR cgiDHT22(HttpdConnData *cd) {
	char buff[256];
	char temp[32];
	char humi[32];

	httpdStartResponse(cd, 200);
	httpdHeader(cd, "Content-Type", "text/json");
	httpdHeader(cd, "Access-Control-Allow-Origin", "*");
	httpdEndHeaders(cd);

	dht_temp_str(temp);
	dht_humi_str(humi);

	os_sprintf(buff, "{ \n\"temperature\": \"%s\"\n , \n\"humidity\": \"%s\"\n}\n", temp, humi );

	httpdSend(cd, buff, -1);
	return HTTPD_CGI_DONE;
}
