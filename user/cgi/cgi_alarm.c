#include <esp8266.h>
#include <stdlib.h>
#include <string.h>
#include <osapi.h>
#include "cgi.h"
#include "fx.h"
#include "config.h"

void ICACHE_FLASH_ATTR cgiAlarmGet(HttpdConnData *cd, alarm *alrm, int alarm_token) {
extern int v_sscanf(const char *str, const char *format, ...);
	char buff[128];
	char token[64];
	int it;

	os_sprintf( token, "a%d-enable", alarm_token );
	alrm->on_days &= ~(0x01 << 7);
	alrm->on_days |= (((httpdFindArg(cd->post->buff, token, buff, sizeof(buff)) > 0) ? 1:0) << 7);

	os_sprintf( token, "a%d-time", alarm_token );
	if( httpdFindArg(cd->post->buff, token, buff, sizeof(buff)) > 0 ){
		int hour, min;
		v_sscanf( buff, "%d:%d", &hour, &min );
		alrm->hour = hour;
		alrm->min = min;
	}

	for( it = 0; it < 7; it++ ){
		os_sprintf( token, "a%d-day%d", alarm_token, it );

		alrm->on_days &= ~(0x01 << it);
		alrm->on_days |= (((httpdFindArg(cd->post->buff, token, buff, sizeof(buff)) > 0) ? 1:0) << it);
	}
}

int ICACHE_FLASH_ATTR cgiAlarmSettings(HttpdConnData *cd) {
	if(cd->conn == NULL)
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;

	cgiInt( cd, "alarm_duration", &config()->alarm_duration );

	cgiAlarmGet( cd, config()->alarms + 0, 0 );
	cgiAlarmGet( cd, config()->alarms + 1, 1 );
	cgiAlarmGet( cd, config()->alarms + 2, 2 );
	cgiAlarmGet( cd, config()->alarms + 3, 3 );
	cgiAlarmGet( cd, config()->alarms + 4, 4 );
	cgiAlarmGet( cd, config()->alarms + 5, 5 );

	CFG_Save();
	httpdRedirect(cd, "/");
	return HTTPD_CGI_DONE;
}


void ICACHE_FLASH_ATTR tplAlarmSet(char *data, char *token, const alarm *alrm, int alarm_token) {
	char buff[128];
	int it;

	os_sprintf( buff, "a%d-enable", alarm_token );
	if( os_strcmp(token, buff) == 0 )
		os_strcpy(data, (alrm->on_days >> 7) & 0x01 ? "checked" : "" );

	os_sprintf( buff, "a%d-time", alarm_token );
	if ( os_strcmp(token, buff) == 0 ) {
		os_sprintf(data, "%02d:%02d", alrm->hour, alrm->min );
	}

	for( it = 0; it < 7; it++ ){
		os_sprintf( buff, "a%d-day%d", alarm_token, it );
		if ( os_strcmp(token, buff) == 0 )
			os_strcpy(data, (alrm->on_days >> it) & 0x01 ? "checked" : "" );
	}
}

void ICACHE_FLASH_ATTR tplAlarmSettings(HttpdConnData *cd, char *token, void **arg) {
	char buff[256];

	if( token == NULL ) return;

	os_strcpy(buff, "Unknown");

	tplInt( buff, token, "alarm_duration", config()->alarm_duration );

	tplAlarmSet( buff, token, config()->alarms + 0, 0 );
	tplAlarmSet( buff, token, config()->alarms + 1, 1 );
	tplAlarmSet( buff, token, config()->alarms + 2, 2 );
	tplAlarmSet( buff, token, config()->alarms + 3, 3 );
	tplAlarmSet( buff, token, config()->alarms + 4, 4 );
	tplAlarmSet( buff, token, config()->alarms + 5, 5 );

	httpdSend(cd, buff, -1);
}
