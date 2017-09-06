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
#include "espmissingincludes.h"
#include "cgi.h"
#include "sntp.h"
#include "config.h"

void ICACHE_FLASH_ATTR tplNTP(HttpdConnData *cd, char *token, void **arg) {
	char buff[128];
	if ( token == NULL ) return;

	os_strcpy(buff, "Unknown");

	tplInt( buff, token, "ntp-enable", (int)config()->ntp_enable );
	tplInt( buff, token, "ntp-tz", (int)config()->ntp_tz );
	tplText( buff, token, "ntp-server-1", config()->ntp_servers[0] );
	tplText( buff, token, "ntp-server-2", config()->ntp_servers[1] );
	tplText( buff, token, "ntp-server-3", config()->ntp_servers[2] );

	if (os_strcmp(token, "NTP")==0) {
		//os_sprintf(buff,"Time: %s GMT%s%02d\n",epoch_to_str(sntp_time+(sntp_tz*3600)),sntp_tz > 0 ? "+" : "",sntp_tz);
	}

	httpdSend(cd, buff, -1);
}

int ICACHE_FLASH_ATTR cgiNTP(HttpdConnData *cd) {
	if ( cd->conn == NULL ) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	cgiCheckBox( cd, "ntp-enable", &config()->ntp_enable );

	memset( config()->ntp_servers[0], 0, 32 );
	memset( config()->ntp_servers[1], 0, 32 );
	memset( config()->ntp_servers[2], 0, 32 );

	cgiText( cd, "ntp-server-1", config()->ntp_servers[0], sizeof(config()->ntp_servers[0]));
	cgiText( cd, "ntp-server-2", config()->ntp_servers[1], sizeof(config()->ntp_servers[1]));
	cgiText( cd, "ntp-server-3", config()->ntp_servers[2], sizeof(config()->ntp_servers[2]));

	cgiInt( cd, "ntp-tz", &config()->ntp_tz );

	if( config()->ntp_enable ){
		sntp_set_timezone(config()->ntp_tz);
		sntp_setservername( 0, config()->ntp_servers[0] );
		sntp_setservername( 1, config()->ntp_servers[1] );
		sntp_setservername( 2, config()->ntp_servers[2] );
		sntp_init();
	}

	CFG_Save();
	httpdRedirect(cd, "/");
	return HTTPD_CGI_DONE;
}
