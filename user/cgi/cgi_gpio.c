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
#include "io.h"
#include "config.h"

//Cgi that turns the Relays on or off according to the 'relayX' param in the GET data
int ICACHE_FLASH_ATTR cgiGPIO(HttpdConnData *cd) {
	char buff[128];
	int32_t gotcmd_r1 = -1;
	int32_t gotcmd_r2 = -1;

	if ( cd->conn == NULL ) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	cgiInt( cd, "relay1", &gotcmd_r1 );
	if ( gotcmd_r1 != -1 )
		io_GPIOSet( gotcmd_r1, GPIO_RELAY1);

	cgiInt( cd, "relay2", &gotcmd_r2);
	if ( gotcmd_r2 != -1 )
		io_GPIOSet( gotcmd_r2, GPIO_RELAY2);

	if( gotcmd_r1 != -1 || gotcmd_r2 != -1 ) {
		if( config()->relay_latching_enable ) {
			sysCfg.relay_1_state = io_GPIOGet(GPIO_RELAY1);
			sysCfg.relay_2_state = io_GPIOGet(GPIO_RELAY2);
			CFG_Save();
		}

		httpdRedirect(cd, "relay.tpl");
		return HTTPD_CGI_DONE;
	} else { //with no parameters returns JSON with relay state

		httpdStartResponse(cd, 200);
		httpdHeader(cd, "Content-Type", "text/json");
		httpdHeader(cd, "Access-Control-Allow-Origin", "*");
		httpdEndHeaders(cd);

		os_sprintf(buff, "{\"relay1\": %d\n,\"relay1name\":\"%s\",\n\"relay2\": %d\n,\"relay2name\":\"%s\" }\n", 
					io_GPIOGet(GPIO_RELAY1), config()->relay1name,
					io_GPIOGet(GPIO_RELAY2), config()->relay2name );
		httpdSend(cd, buff, -1);
		return HTTPD_CGI_DONE;
	}
}

//Template code for the led page.
void ICACHE_FLASH_ATTR tplGPIO(HttpdConnData *cd, char *token, void **arg) {
	char buff[128];
	if ( token == NULL ) return;

	os_strcpy(buff, "Unknown");

	tplText( buff, token, "relay1name", config()->relay1name);
	tplText( buff, token, "relay2name", config()->relay2name);

	if( os_strcmp(token, "relay1") == 0 )
		os_strcpy(buff, io_GPIOGet(GPIO_RELAY1) ? "on": "off");

	if( os_strcmp(token, "relay2") == 0 )
		os_strcpy(buff, io_GPIOGet(GPIO_RELAY2) ? "on": "off");

	httpdSend(cd, buff, -1);
}
