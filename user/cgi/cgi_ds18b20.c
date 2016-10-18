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
#include "ds18b20.h"
#include "config.h"

//Template code for the DS18b20 page.
void ICACHE_FLASH_ATTR tplDS18b20(HttpdConnData *connData, char *token, void **arg) {
	char buff[384];
	if (token==NULL) return;

	os_strcpy(buff, "Unknown");

	if (os_strcmp(token, "numds")==0) {
		os_sprintf( buff,"%d", numds);
	}

	if (os_strcmp(token, "temperatures")==0) {
		int Treading, SignBit, Whole, Fract;

		os_strcpy(buff, "");

		for(int i=0; i<numds; i++) {
			if(dsreading[i].success) {
			Treading = dsreading[i].temperature;
			SignBit = Treading & 0x8000;  // test most sig bit
			if (SignBit) // negative
				Treading = (Treading ^ 0xffff) + 1; // 2's comp
			Whole = Treading >> 4;  // separate off the whole and fractional portions
			Fract = (Treading & 0xf) * 100 / 16;
			if (SignBit) // negative
				Whole*=-1;
				os_sprintf( buff+strlen(buff) ,"Sensor %d (%02x %02x %02x %02x %02x %02x %02x %02x) reading is %d.%d°C<br />",
						i+1, addr[i][0], addr[i][1], addr[i][2], addr[i][3], addr[i][4], addr[i][5], addr[i][6], addr[i][7],
						Whole, Fract < 10 ? 0 : Fract);
			} else {
				os_sprintf( buff+strlen(buff) ,"Sensor %d (%02x %02x %02x %02x %02x %02x %02x %02x) reading is invalid<br />",
						i+1, addr[i][0], addr[i][1], addr[i][2], addr[i][3], addr[i][4], addr[i][5], addr[i][6], addr[i][7]);
			}
		}
	}

	httpdSend(connData, buff, -1);
}

int ICACHE_FLASH_ATTR cgiDS18b20(HttpdConnData *connData) {
	char buff[256];
	char tmp[32];
	httpdStartResponse(connData, 200);
	httpdHeader(connData, "Content-Type", "text/json");
	httpdHeader(connData, "Access-Control-Allow-Origin", "*");
	httpdEndHeaders(connData);

	ds_str(tmp,0);
	os_sprintf( buff, "{ \n\"temperature\": \"%s\"\n}\n",tmp );

	httpdSend(connData, buff, -1);
	return HTTPD_CGI_DONE;
}
