#include <esp8266.h>
#include <stdlib.h>
#include <string.h>
#include <osapi.h>
#include "cgi.h"
#include "config.h"
#include "light.h"

int ICACHE_FLASH_ATTR cgiLight(HttpdConnData *cd) {
	char buff[128];

	if(cd->conn == NULL)
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;

	if( httpdFindArg(cd->getArgs, "light", buff, sizeof(buff)) > 0 ){
		light_enable( atoi(buff) ? true: false );
		httpdRedirect(cd, "relay.tpl");
		return HTTPD_CGI_DONE;
	}

	httpdStartResponse(cd, 200);
	httpdHeader(cd, "Content-Type", "text/json");
	httpdHeader(cd, "Access-Control-Allow-Origin", "*");
	httpdEndHeaders(cd);

	os_sprintf(buff, "{\"light\": %d\n}\n", light_is_enabled() );
	httpdSend(cd, buff, -1);
	return HTTPD_CGI_DONE;
}

void ICACHE_FLASH_ATTR tplLight(HttpdConnData *cd, char *token, void **arg) {
	char buff[128];

	if( token == NULL ) return;

	os_strcpy(buff, "Unknown");
	httpdSend(cd, buff, -1);
}
