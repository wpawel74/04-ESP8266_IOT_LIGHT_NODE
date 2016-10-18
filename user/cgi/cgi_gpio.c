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
int ICACHE_FLASH_ATTR cgiGPIO(HttpdConnData *connData) {
	int len;
	char buff[128];
	int gotcmd=0;

	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	len=httpdFindArg(connData->getArgs, "relay1", buff, sizeof(buff));
	if (len>0) {
		currREL1State=atoi(buff);
		ioGPIO(currREL1State, RELAY1_GPIO);
		gotcmd=1;
		//Manually switching relays means switching the thermostat off
		if(sysCfg.thermostat1state!=0) {
			sysCfg.thermostat1state=0;
		}
	}

	len=httpdFindArg(connData->getArgs, "relay2", buff, sizeof(buff));
	if (len>0) {
		currREL2State=atoi(buff);
		ioGPIO(currREL2State, RELAY2_GPIO);
		gotcmd=1;
		//Manually switching relays means switching the thermostat off
		if(sysCfg.thermostat2state!=0) {
			sysCfg.thermostat2state=0;
		}
	}

	if(gotcmd==1) {
		if( sysCfg.relay_latching_enable) {
			sysCfg.relay_1_state=currREL1State;
			sysCfg.relay_2_state=currREL2State;
			CFG_Save();
		}

		httpdRedirect(connData, "relay.tpl");
		return HTTPD_CGI_DONE;
	} else { //with no parameters returns JSON with relay state

		httpdStartResponse(connData, 200);
		httpdHeader(connData, "Content-Type", "text/json");
		httpdHeader(connData, "Access-Control-Allow-Origin", "*");
		httpdEndHeaders(connData);

		len = os_sprintf(buff, "{\"relay1\": %d\n,\"relay1name\":\"%s\",\n\"relay2\": %d\n,\"relay2name\":\"%s\" }\n", 
					currREL1State,(char *)sysCfg.relay1name,
					currREL2State,(char *)sysCfg.relay2name );
		httpdSend(connData, buff, -1);
		return HTTPD_CGI_DONE;
	}
}

//Template code for the led page.
void ICACHE_FLASH_ATTR tplGPIO(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	if (token==NULL) return;

	os_strcpy(buff, "Unknown");

	if (os_strcmp(token, "relay1name")==0) {
		os_strcpy(buff, (char *)sysCfg.relay1name);
	}
	
	if (os_strcmp(token, "relay2name")==0) {
		os_strcpy(buff, (char *)sysCfg.relay2name);
	}

	if (os_strcmp(token, "relay1")==0) {
		if (currREL1State) {
			os_strcpy(buff, "on");
		} else {
			os_strcpy(buff, "off");
		}
		os_printf("Relay 1 is now ");
		os_printf(buff);
		os_printf("\n ");
	}

	if (os_strcmp(token, "relay2")==0) {
		if (currREL2State) {
			os_strcpy(buff, "on");
		} else {
			os_strcpy(buff, "off");
		}
		os_printf("Relay 2 is now ");
		os_printf(buff);
		os_printf("\n ");
	}

	httpdSend(connData, buff, -1);
}
