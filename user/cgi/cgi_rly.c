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

int ICACHE_FLASH_ATTR cgiRLYSettings(HttpdConnData *connData) {
	int len;
	char buff[128];

	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	len=httpdFindArg(connData->post->buff, "relay-latching-enable", buff, sizeof(buff));
	sysCfg.relay_latching_enable = (len > 0) ? 1:0;

	len=httpdFindArg(connData->post->buff, "relay1name", buff, sizeof(buff));
	if (len>0) {
		os_strcpy((char *)sysCfg.relay1name,buff);
	}

	len=httpdFindArg(connData->post->buff, "relay2name", buff, sizeof(buff));
	if (len>0) {
		os_strcpy((char *)sysCfg.relay2name,buff);
	}

	CFG_Save();
	httpdRedirect(connData, "/");
	return HTTPD_CGI_DONE;
}

void ICACHE_FLASH_ATTR tplRLYSettings(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	if (token==NULL) return;

	os_strcpy(buff, "Unknown");

	if (os_strcmp(token, "relay-latching-enable")==0) {
		os_strcpy(buff, sysCfg.relay_latching_enable == 1 ? "checked" : "" );
	}

	if (os_strcmp(token, "relay1name")==0) {
		os_strcpy(buff, (char *)sysCfg.relay1name);
	}

	if (os_strcmp(token, "relay2name")==0) {
		os_strcpy(buff, (char *)sysCfg.relay2name);
	}

	httpdSend(connData, buff, -1);
}
