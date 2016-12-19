#include <esp8266.h>
#include <stdlib.h>
#include <string.h>
#include <osapi.h>
#include "cgi.h"
#include "config.h"
#include "light.h"

int ICACHE_FLASH_ATTR cgiLight(HttpdConnData *connData) {
	char buff[128];

	if(connData->conn == NULL)
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;

	if( httpdFindArg(connData->getArgs, "light", buff, sizeof(buff)) > 0 ){
		light_enable( atoi(buff) ? true: false );
		httpdRedirect(connData, "relay.tpl");
		return HTTPD_CGI_DONE;
	}

	httpdStartResponse(connData, 200);
	httpdHeader(connData, "Content-Type", "text/json");
	httpdHeader(connData, "Access-Control-Allow-Origin", "*");
	httpdEndHeaders(connData);

	os_sprintf(buff, "{\"light\": %d\n}\n", light_is_enabled() );
	httpdSend(connData, buff, -1);
	return HTTPD_CGI_DONE;
}

void ICACHE_FLASH_ATTR tplLight(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];

	if( token == NULL ) return;

	os_strcpy(buff, "Unknown");
	httpdSend(connData, buff, -1);
}
