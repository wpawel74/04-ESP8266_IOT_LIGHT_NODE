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
#include "io.h"
#include "dht22.h"
#include "ds18b20.h"


int ICACHE_FLASH_ATTR cgiState(HttpdConnData *cd) {
	char buff[512];
	char tmp[32];

	char temp[32];
	char humi[32];

	httpdStartResponse(cd, 200);
	httpdHeader(cd, "Content-Type", "text/json");
	httpdHeader(cd, "Access-Control-Allow-Origin", "*");
	httpdEndHeaders(cd);

	ds_str(tmp,0);

	dht_temp_str(temp);
	dht_humi_str(humi);

	os_sprintf( buff, "{ \n\"relay1\": \"%d\"\n,\n\"relay2\": \"%d\"\n , \n\"DHT22temperature\": \"%s\"\n , \n\"DHT22humidity\": \"%s\"\n,\"DS18B20temperature\": \"%s\"\n}\n",
				io_GPIOGet(GPIO_RELAY1), io_GPIOGet(GPIO_RELAY2), temp, humi, tmp );

	httpdSend(cd, buff, -1);
	return HTTPD_CGI_DONE;
}
