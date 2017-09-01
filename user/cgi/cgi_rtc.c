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
		os_printf( "RTC: date %d/%02d/%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec );

		if (os_strcmp(token, "rtc_date")==0) {
			os_printf( "--> RTC_DATE\n" );
			os_sprintf(buff, "%d-%d-%d", 1900 + tm.tm_year, tm.tm_mon + 1, tm.tm_mday );
		}
		if (os_strcmp(token, "rtc_time")==0) {
			os_printf( "--> RTC_TIME\n" );
			os_sprintf(buff, "%d:%d", tm.tm_hour, tm.tm_min );
		}
	}
	httpdSend(connData, buff, -1);
}

int ICACHE_FLASH_ATTR cgiRTC(HttpdConnData *connData) {
extern int v_sscanf(const char *str, const char *format, ...);
	char buff[256];
	struct tm tm;
	int c_ok = 0;

	if(connData->conn == NULL)
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;

	if( httpdFindArg(connData->post->buff, "time", buff, sizeof(buff)) > 0 ){
		tm.tm_sec = 0;
		os_printf( "TIME %s\n", buff );
		c_ok += v_sscanf( buff, "%d:%d", &tm.tm_hour, &tm.tm_min );
	}

	if( httpdFindArg(connData->post->buff, "date", buff, sizeof(buff)) > 0 ){
		os_printf( "DATA %s\n", buff );
		c_ok += v_sscanf( buff, "%d-%d-%d", (int *)&tm.tm_year, &tm.tm_mon, &tm.tm_mday );
		os_printf("--> %d %d %d", tm.tm_year, tm.tm_mon, tm.tm_mday );
	}

	if( c_ok == 5 ){
		tm.tm_year = tm.tm_year - 1900;
		tm.tm_mon = tm.tm_mon - 1;
		tm.tm_sec = 0;
		ds1307_setTime( &tm );
	}
	httpdRedirect(connData, "/");
	return HTTPD_CGI_DONE;
}
