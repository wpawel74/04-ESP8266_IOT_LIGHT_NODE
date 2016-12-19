#include <esp8266.h>
#include <stdlib.h>
#include <string.h>
#include <osapi.h>
#include "cgi.h"
#include <time.h>
#include "ds1307.h"

void ICACHE_FLASH_ATTR tplRTC(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	struct tm tm;

	if(token == NULL) return;
	os_strcpy(buff, "Unknown");

	if( ds1307_getTime(&tm) ) {
		if (os_strcmp(token, "rtc_date")==0) {
			os_sprintf(buff, "%d/%d/%d", 1900 + tm.tm_year, tm.tm_mon + 1, tm.tm_mday );
		}
		if (os_strcmp(token, "rtc_time")==0) {
			os_sprintf(buff, "%d:%d", tm.tm_hour, tm.tm_min );
		}
	}
	httpdSend(connData, buff, -1);
}

int ICACHE_FLASH_ATTR cgiRTC(HttpdConnData *connData) {
extern int v_sscanf(const char *str, const char *format, ...);
	char buff[256];
	struct tm tm;

	if(connData->conn == NULL)
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;

	if( httpdFindArg(connData->post->buff, "time", buff, sizeof(buff)) > 0 )
		v_sscanf( buff, "%d:%d:%d", &tm.tm_hour, &tm.tm_min, &tm.tm_sec );

	if( httpdFindArg(connData->post->buff, "date", buff, sizeof(buff)) > 0 )
		v_sscanf( buff, "%d/%d/%d", &tm.tm_year, &tm.tm_mon, &tm.tm_mday );

	ds1307_setTime( &tm );
	httpdRedirect(connData, "/");
	return HTTPD_CGI_DONE;
}
