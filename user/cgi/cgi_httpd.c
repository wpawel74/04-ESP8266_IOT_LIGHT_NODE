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
#include "config.h"

void ICACHE_FLASH_ATTR tplHTTPD(HttpdConnData *cd, char *token, void **arg) {
	char buff[128];
	if ( token == NULL ) return;

	os_strcpy(buff, "Unknown");

	tplInt( buff, token, "httpd-auth", (int)sysCfg.httpd_auth);
	tplInt( buff, token, "httpd-port", (int)sysCfg.httpd_port);
	tplText( buff, token, "httpd-user", (char *)sysCfg.httpd_user);
	tplText( buff, token, "httpd-pass", (char *)sysCfg.httpd_pass);

	httpdSend(cd, buff, -1);
}

int ICACHE_FLASH_ATTR cgiHTTPD(HttpdConnData *cd) {
	char buff[64];
	int len;

	if (cd->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	len = httpdFindArg(cd->post->buff, "httpd-auth", buff, sizeof(buff));
	sysCfg.httpd_auth = (len > 0) ? 1:0;

	cgiInt( cd, "httpd-port", (int32_t *)&config()->httpd_port );
	cgiText( cd, "httpd-user", (char *)config()->httpd_user, sizeof(config()->httpd_user) );
	cgiText( cd, "httpd-pass", (char *)config()->httpd_pass, sizeof(config()->httpd_user) );

	CFG_Save();
	httpdRedirect( cd, "/" );
	return HTTPD_CGI_DONE;
}
