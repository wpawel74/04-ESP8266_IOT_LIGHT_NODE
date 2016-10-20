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
#include "config.h"

int ICACHE_FLASH_ATTR cgiLightSettings(HttpdConnData *connData) {
	char buff[128];

	if(connData->conn == NULL)
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;

	sysCfg.light_driver_enable = (httpdFindArg(connData->post->buff, "light_driver_enable", buff, sizeof(buff)) > 0) ? 1:0;

	if( httpdFindArg(connData->post->buff, "delay_power_on", buff, sizeof(buff)) > 0 )
		sysCfg.light_delay_power_on = atoi(buff);

	if( httpdFindArg(connData->post->buff, "delay_power_off", buff, sizeof(buff)) > 0 )
		sysCfg.light_delay_power_off = atoi(buff);

	if( httpdFindArg(connData->post->buff, "power_supply_configuration", buff, sizeof(buff)) > 0 )
		sysCfg.light_power_supply = atoi(buff);

	if( httpdFindArg(connData->post->buff, "chain_size", buff, sizeof(buff)) > 0 )
		sysCfg.light_chain_size = atoi(buff);

	CFG_Save();
	httpdRedirect(connData, "/");
	return HTTPD_CGI_DONE;
}

void ICACHE_FLASH_ATTR tplLightSettings(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];

	if( token == NULL ) return;

	os_strcpy(buff, "Unknown");

	if( os_strcmp(token, "light_driver_enable") == 0 )
		os_strcpy(buff, sysCfg.light_driver_enable == 1 ? "checked" : "" );

	if( os_strcmp(token, "name_relay1") == 0 )
		os_strcpy(buff, (char *)sysCfg.relay1name);

	if( os_strcmp(token, "name_relay2") == 0 )
		os_strcpy(buff, (char *)sysCfg.relay2name);

	if( os_strcmp(token, "selected_relay1") == 0 )
		os_strcpy(buff, sysCfg.light_power_supply == 1 ? "selected" : "" );

	if( os_strcmp(token, "selected_relay2") == 0 )
		os_strcpy(buff, sysCfg.light_power_supply == 2 ? "selected" : "" );

	if( os_strcmp(token, "selected_dontcare") == 0 )
		os_strcpy(buff, sysCfg.light_power_supply == 0 ? "selected" : "" );

	if( os_strcmp(token, "delay_power_on") == 0 )
		os_sprintf(buff,"%d", (int)sysCfg.light_delay_power_on);

	if( os_strcmp(token, "delay_power_off") == 0 )
		os_sprintf(buff,"%d", (int)sysCfg.light_delay_power_off);

	if( os_strcmp(token, "chain_size") == 0 )
		os_sprintf(buff,"%d", (int)sysCfg.light_chain_size);

	httpdSend(connData, buff, -1);
}

