#include <esp8266.h>
#include <stdlib.h>
#include <string.h>
#include <osapi.h>
#include "cgi.h"
#include "fx.h"
#include "light.h"
#include "config.h"

int ICACHE_FLASH_ATTR cgiLightSettings(HttpdConnData *cd) {

	if(cd->conn == NULL)
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;

	cgiCheckBox( cd, "light_driver_enable", &config()->light_driver_enable );
	cgiInt( cd, "delay_power_on", &config()->light_delay_power_on );
	cgiInt( cd, "delay_power_off", &config()->light_delay_power_off );
	// NOTE: check cast enum to int32_t
	cgiInt( cd, "power_supply_configuration", (int32_t *)&config()->light_power_supply );
	cgiInt( cd, "chain_size", &config()->light_chain_size );
	cgiInt( cd, "light_udp_port", &config()->light_udp_port );

	cgiCheckBox( cd, "light_simple_1_enable", &config()->fx_simple_1_enable );
	cgiInt( cd, "simple_1_start_no", &config()->fx_simple_1_start_no );
	cgiInt( cd, "simple_1_stop_no", &config()->fx_simple_1_stop_no );
	cgiRGB( cd, "simple_1", &config()->fx_simple_1_RGB );

	cgiCheckBox( cd, "light_simple_2_enable", &config()->fx_simple_2_enable );
	cgiInt( cd, "simple_2_start_no", &config()->fx_simple_2_start_no );
	cgiInt( cd, "simple_2_stop_no", &config()->fx_simple_2_stop_no );
	cgiRGB( cd, "simple_2", &config()->fx_simple_2_RGB );

	cgiCheckBox( cd, "light_flame_1_enable", &config()->fx_flames_1_enable );
	cgiInt( cd, "flame_1_start_no", &config()->fx_flames_1_start_no );
	cgiInt( cd, "flame_1_stop_no", &config()->fx_flames_1_stop_no );

	cgiCheckBox( cd, "light_pulsar_1_enable", &config()->fx_pulsar_1_enable );
	cgiInt( cd, "pulsar_1_start_no", &config()->fx_pulsar_1_start_no );
	cgiInt( cd, "pulsar_1_stop_no", &config()->fx_pulsar_1_stop_no );
	cgiInt( cd, "pulsar_1_delay", &config()->fx_pulsar_1_delay );
	cgiRGB( cd, "pulsar_1_dizzy", &config()->fx_pulsar_1_RGB_dizzy );
	cgiRGB( cd, "pulsar_1_fuzzy", &config()->fx_pulsar_1_RGB_fuzzy );

	light_fx_reload();

	CFG_Save();
	httpdRedirect(cd, "/");
	return HTTPD_CGI_DONE;
}

void ICACHE_FLASH_ATTR tplLightSettings(HttpdConnData *cd, char *token, void **arg) {
	char buff[64];

	if( token == NULL ) return;

	os_strcpy(buff, "Unknown");

	tplCheckBox( buff, token, "light_driver_enable", config()->light_driver_enable );

	tplText( buff, token, "name_relay1", config()->relay1name);
	tplText( buff, token, "name_relay2", config()->relay2name);

	if( os_strcmp(token, "selected_relay1") == 0 )
		os_strcpy(buff, config()->light_power_supply == 1 ? "selected" : "" );

	if( os_strcmp(token, "selected_relay2") == 0 )
		os_strcpy(buff, config()->light_power_supply == 2 ? "selected" : "" );

	if( os_strcmp(token, "selected_dontcare") == 0 )
		os_strcpy(buff, config()->light_power_supply == 0 ? "selected" : "" );

	tplInt( buff, token, "delay_power_on", (int)config()->light_delay_power_on );
	tplInt( buff, token, "delay_power_off", (int)config()->light_delay_power_off );
	tplInt( buff, token, "chain_size", (int)config()->light_chain_size );
	tplInt( buff, token, "light_udp_port", (int)config()->light_udp_port );

	tplCheckBox( buff, token, "light_simple_1_enable", config()->fx_simple_1_enable );

	tplInt( buff, token, "simple_1_start_no", (int)config()->fx_simple_1_start_no );
	tplInt( buff, token, "simple_1_stop_no", (int)config()->fx_simple_1_stop_no );
	tplRGB( buff, token, &config()->fx_simple_1_RGB, "simple_1" );

	tplCheckBox( buff, token, "light_simple_2_enable", config()->fx_simple_2_enable );

	tplInt( buff, token, "simple_2_start_no", (int)config()->fx_simple_2_start_no);
	tplInt( buff, token, "simple_2_stop_no", (int)config()->fx_simple_2_stop_no);
	tplRGB( buff, token, &config()->fx_simple_2_RGB, "simple_2" );

	tplCheckBox( buff, token, "light_flame_1_enable", config()->fx_flames_1_enable );
	tplInt( buff, token, "flame_1_start_no", (int)config()->fx_flames_1_start_no);
	tplInt( buff, token, "flame_1_stop_no", (int)config()->fx_flames_1_stop_no);

	tplCheckBox( buff, token, "light_pulsar_1_enable", config()->fx_pulsar_1_enable );
	tplInt( buff, token, "pulsar_1_start_no", (int)config()->fx_pulsar_1_start_no);
	tplInt( buff, token, "pulsar_1_stop_no", (int)config()->fx_pulsar_1_stop_no);
	tplInt( buff, token, "pulsar_1_delay", (int)config()->fx_pulsar_1_delay);
	tplRGB( buff, token, &config()->fx_pulsar_1_RGB_dizzy, "pulsar_1_dizzy" );
	tplRGB( buff, token, &config()->fx_pulsar_1_RGB_fuzzy, "pulsar_1_fuzzy" );

	httpdSend(cd, buff, -1);
}
