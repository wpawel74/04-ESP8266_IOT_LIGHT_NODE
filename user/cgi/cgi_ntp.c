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
#include "espmissingincludes.h"
#include "cgi.h"
#include "sntp.h"
#include "config.h"

void ICACHE_FLASH_ATTR tplNTP(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	if (token==NULL) return;

	os_strcpy(buff, "Unknown");

	if (os_strcmp(token, "ntp-enable")==0) {
		os_sprintf(buff, "%d", (int)sysCfg.ntp_enable);
	}

	if (os_strcmp(token, "ntp-tz")==0) {
		os_sprintf(buff, "%d", (int)sysCfg.ntp_tz);
	}

	if (os_strcmp(token, "NTP")==0) {
//		os_sprintf(buff,"Time: %s GMT%s%02d\n",epoch_to_str(sntp_time+(sntp_tz*3600)),sntp_tz > 0 ? "+" : "",sntp_tz);
	}

	httpdSend(connData, buff, -1);
}

int ICACHE_FLASH_ATTR cgiNTP(HttpdConnData *connData) {
	char buff[128];
	int len;

	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	len=httpdFindArg(connData->post->buff, "ntp-enable", buff, sizeof(buff));
	sysCfg.ntp_enable = len > 0 ? 1:0;
	
	len=httpdFindArg(connData->post->buff, "ntp-tz", buff, sizeof(buff));
	if (len>0) {
		sysCfg.ntp_tz=atoi(buff);
//		sntp_tz=sysCfg.ntp_tz;
	}

	CFG_Save();

	httpdRedirect(connData, "/");
	return HTTPD_CGI_DONE;
}
