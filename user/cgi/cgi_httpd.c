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
#include "httpd.h"
#include "cgi.h"
#include "io.h"
#include "config.h"

void ICACHE_FLASH_ATTR tplHTTPD(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	if (token==NULL) return;

	os_strcpy(buff, "Unknown");

	if (os_strcmp(token, "httpd-auth")==0) {
		os_sprintf(buff,"%d", (int)sysCfg.httpd_auth);
	}
	
	if (os_strcmp(token, "httpd-port")==0) {
		os_sprintf(buff,"%d", (int)sysCfg.httpd_port);
	}

	if (os_strcmp(token, "httpd-user")==0) {
		os_strcpy(buff, (char *)sysCfg.httpd_user);
	}

	if (os_strcmp(token, "httpd-pass")==0) {
		os_strcpy(buff, (char *)sysCfg.httpd_pass);
	}

	httpdSend(connData, buff, -1);
}

int ICACHE_FLASH_ATTR cgiHTTPD(HttpdConnData *connData) {
	char buff[128];
	int len;

	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	len=httpdFindArg(connData->post->buff, "httpd-auth", buff, sizeof(buff));
	sysCfg.httpd_auth = (len > 0) ? 1:0;

	len=httpdFindArg(connData->post->buff, "httpd-port", buff, sizeof(buff));
	if (len>0) {
		sysCfg.httpd_port=atoi(buff);
	}

	len=httpdFindArg(connData->post->buff, "httpd-user", buff, sizeof(buff));
	if (len>0) {
		os_strcpy((char *)sysCfg.httpd_user,buff);
	}

	len=httpdFindArg(connData->post->buff, "httpd-pass", buff, sizeof(buff));
	if (len>0) {
		os_strcpy((char *)sysCfg.httpd_pass,buff);
	}

	CFG_Save();

	httpdRedirect(connData, "/");
	return HTTPD_CGI_DONE;
}
