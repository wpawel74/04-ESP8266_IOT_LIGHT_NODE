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

int ICACHE_FLASH_ATTR cgiRLYSettings(HttpdConnData *cd) {
	if (cd->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	cgiCheckBox( cd, "relay-latching-enable", &config()->relay_latching_enable );

	cgiText( cd, "relay1name", config()->relay1name, sizeof(config()->relay1name) );
	cgiText( cd, "relay2name", config()->relay2name, sizeof(config()->relay2name) );

	CFG_Save();
	httpdRedirect(cd, "/");
	return HTTPD_CGI_DONE;
}

void ICACHE_FLASH_ATTR tplRLYSettings(HttpdConnData *cd, char *token, void **arg) {
	char buff[128];
	if ( token == NULL ) return;

	os_strcpy(buff, "Unknown");

	tplCheckBox( buff, token, "relay-latching-enable", config()->relay_latching_enable );
	tplText( buff, token, "relay1name", config()->relay1name );
	tplText( buff, token, "relay2name", config()->relay2name );

	httpdSend(cd, buff, -1);
}
