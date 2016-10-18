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

void ICACHE_FLASH_ATTR tplBroadcastD(HttpdConnData *connData, char *token, void **arg) {
	char buff[255];
	if (token==NULL) return;

	os_strcpy(buff, "Unknown");

	if (os_strcmp(token, "broadcastd-enable")==0) {
		os_sprintf(buff,"%d", (int)sysCfg.broadcastd_enable);
	}

	if (os_strcmp(token, "broadcastd-port")==0) {
		os_sprintf(buff,"%d", (int)sysCfg.broadcastd_port);
	}

	if (os_strcmp(token, "broadcastd-host")==0) {
		os_strcpy(buff, (char *)sysCfg.broadcastd_host);
	}

	if (os_strcmp(token, "broadcastd-URL")==0) {
		os_strcpy(buff, (char *)sysCfg.broadcastd_url);
	}

	if (os_strcmp(token, "broadcastd-thingspeak-channel")==0) {
		os_sprintf(buff,"%d", (int)sysCfg.broadcastd_thingspeak_channel);
	}

	if (os_strcmp(token, "broadcastd-ro-apikey")==0) {
		os_strcpy(buff, (char *)sysCfg.broadcastd_ro_apikey);
	}

	httpdSend(connData, buff, -1);
}

int ICACHE_FLASH_ATTR cgiBroadcastD(HttpdConnData *connData) {
	char buff[255];
	int len;

	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	len=httpdFindArg(connData->getArgs, "js", buff, sizeof(buff));
	if (len>0) {

		httpdStartResponse(connData, 200);
		httpdHeader(connData, "Content-Type", "text/javascript");
		httpdEndHeaders(connData);

		len=os_sprintf(buff, "var channel=%d;\nvar ro_apikey=\"%s\";\n",
					(int)sysCfg.broadcastd_thingspeak_channel,(char *)sysCfg.broadcastd_ro_apikey );
		httpdSend(connData, buff, -1);
		return HTTPD_CGI_DONE;	
	}

	len=httpdFindArg(connData->post->buff, "broadcastd-enable", buff, sizeof(buff));
	sysCfg.broadcastd_enable = (len > 0) ? 1:0;

	len=httpdFindArg(connData->post->buff, "broadcastd-port", buff, sizeof(buff));
	if (len>0) {
		sysCfg.broadcastd_port=atoi(buff);
	}

	len=httpdFindArg(connData->post->buff, "broadcastd-host", buff, sizeof(buff));
	if (len>0) {
		os_strcpy((char *)sysCfg.broadcastd_host,buff);
	}

	len=httpdFindArg(connData->post->buff, "broadcastd-URL", buff, sizeof(buff));
	if (len>0) {
		os_strcpy((char *)sysCfg.broadcastd_url,buff);
	}

	len=httpdFindArg(connData->post->buff, "broadcastd-thingspeak-channel", buff, sizeof(buff));
	if (len>0) {
		sysCfg.broadcastd_thingspeak_channel=atoi(buff);
	}

	len=httpdFindArg(connData->post->buff, "broadcastd-ro-apikey", buff, sizeof(buff));
	if (len>0) {
		os_strcpy((char *)sysCfg.broadcastd_ro_apikey,buff);
	}

	CFG_Save();

	httpdRedirect(connData, "/");
	return HTTPD_CGI_DONE;
}
