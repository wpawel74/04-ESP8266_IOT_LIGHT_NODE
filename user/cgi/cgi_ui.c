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

int ICACHE_FLASH_ATTR cgiUI(HttpdConnData *connData) {
	char buff[128];

	httpdStartResponse(connData, 200);
	httpdHeader(connData, "Content-Type", "text/json");
	httpdEndHeaders(connData);

	os_strcpy(buff, "Unknown");
	httpdSend(connData, buff, -1);

	return HTTPD_CGI_DONE;
}

void ICACHE_FLASH_ATTR tplUI(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	if (token==NULL) return;

	os_strcpy(buff, "Unknown");

	tplText( buff, token, "relay1name", sysCfg.relay1name);
	tplText( buff, token, "relay2name", sysCfg.relay2name);

	httpdSend(connData, buff, -1);
}
