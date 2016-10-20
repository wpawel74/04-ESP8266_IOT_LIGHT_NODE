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
#include <time.h>
#include "ds1307.h"

void ICACHE_FLASH_ATTR tplRTC(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	struct tm tm;

	if(token == NULL) return;
	os_strcpy(buff, "Unknown");

	if( ds1307_getTime(&tm) ) {
		if (os_strcmp(token, "year")==0) {
			os_sprintf(buff, "%d", 1900 + tm.tm_year );
		}
		if (os_strcmp(token, "month")==0) {
			os_sprintf(buff, "%d", tm.tm_mon + 1 );
		}
		if (os_strcmp(token, "day")==0) {
			os_sprintf(buff, "%d", tm.tm_mday );
		}
		if (os_strcmp(token, "hours")==0) {
			os_sprintf(buff, "%d", tm.tm_hour );
		}
		if (os_strcmp(token, "minutes")==0) {
			os_sprintf(buff, "%d", tm.tm_min );
		}
	}
	httpdSend(connData, buff, -1);
}

int ICACHE_FLASH_ATTR cgiRTC(HttpdConnData *connData) {
	char buff[256];
	struct tm tm;

	if(connData->conn == NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	if( httpdFindArg(connData->post->buff, "year", buff, sizeof(buff)) > 0 ){
		if( atoi(buff) > 1900 )
			tm.tm_year = atoi(buff) - 1900;
		else
			return HTTPD_CGI_DONE;
	}

	if( httpdFindArg(connData->post->buff, "month", buff, sizeof(buff)) > 0 ){
		if( atoi(buff) > 0 && atoi(buff) < 13 )
			tm.tm_mon = atoi(buff) - 1;
		else
			return HTTPD_CGI_DONE;
	}

	if( httpdFindArg(connData->post->buff, "day", buff, sizeof(buff)) > 0 ){
		if( atoi(buff) > 0 && atoi(buff) < 32 )
			tm.tm_mday = atoi(buff);
		else
			return HTTPD_CGI_DONE;
	}

	if( httpdFindArg(connData->post->buff, "hours", buff, sizeof(buff)) > 0 ){
		if( atoi(buff) > 0 && atoi(buff) < 24 )
			tm.tm_hour = atoi(buff);
		else
			return HTTPD_CGI_DONE;
	}

	if( httpdFindArg(connData->post->buff, "minutes", buff, sizeof(buff)) > 0 ){
		if( atoi(buff) > 0 && atoi(buff) < 60 )
			tm.tm_min = atoi(buff);
		else
			return HTTPD_CGI_DONE;
	}

	ds1307_setTime( &tm );

	httpdRedirect(connData, "/");
	return HTTPD_CGI_DONE;
}
