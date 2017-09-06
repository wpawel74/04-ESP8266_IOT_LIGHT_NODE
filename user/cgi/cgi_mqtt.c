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

void ICACHE_FLASH_ATTR tplMQTT(HttpdConnData *cd, char *token, void **arg) {
	char buff[128];
	if ( token == NULL ) return;

	os_strcpy(buff, "Unknown");

	tplInt( buff, token, "mqtt-enable", (int)config()->mqtt_enable );
	tplInt( buff, token, "mqtt-use-ssl", (int)config()->mqtt_use_ssl );
	tplText( buff, token, "mqtt-host", (char *)config()->mqtt_host );
	tplInt( buff, token, "mqtt-port", (int)config()->mqtt_port);
	tplInt( buff, token, "mqtt-keepalive", config()->mqtt_keepalive);
	tplText( buff, token, "mqtt-devid", (char *)config()->mqtt_devid );
	tplText( buff, token, "mqtt-user", (char *)config()->mqtt_user);
	tplText( buff, token, "mqtt-pass", (char *)config()->mqtt_pass);
	tplText( buff, token, "mqtt-relay-subs-topic", (char *)config()->mqtt_relay_subs_topic);
	tplText( buff, token, "mqtt-dht22-temp-pub-topic", (char *)config()->mqtt_dht22_temp_pub_topic);
	tplText( buff, token, "mqtt-dht22-humi-pub-topic", (char *)config()->mqtt_dht22_humi_pub_topic);
	tplText( buff, token, "mqtt-ds18b20-temp-pub-topic", (char *)config()->mqtt_ds18b20_temp_pub_topic);

	httpdSend(cd, buff, -1);
}

int ICACHE_FLASH_ATTR cgiMQTT(HttpdConnData *cd) {
	if ( cd->conn == NULL ) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	cgiCheckBox( cd, "mqtt-enable", &config()->mqtt_enable );
	cgiCheckBox( cd, "mqtt-use-ssl", &config()->mqtt_use_ssl );
	cgiText( cd, "mqtt-host", (char *)config()->mqtt_host, sizeof(config()->mqtt_host) );
	cgiInt( cd, "mqtt-port", &config()->mqtt_port );
	cgiInt( cd, "mqtt-keepalive", &config()->mqtt_keepalive );
	cgiText( cd, "mqtt-devid", (char *)config()->mqtt_devid, sizeof(config()->mqtt_devid) );
	cgiText( cd, "mqtt-user", (char *)config()->mqtt_user, sizeof(config()->mqtt_user) );
	cgiText( cd, "mqtt-pass", (char *)config()->mqtt_pass, sizeof(config()->mqtt_pass) );
	cgiText( cd, "mqtt-relay-subs-topic", (char *)config()->mqtt_relay_subs_topic, sizeof(config()->mqtt_relay_subs_topic) );
	cgiText( cd, "mqtt-dht22-temp-pub-topic", (char *)config()->mqtt_dht22_temp_pub_topic, sizeof(config()->mqtt_dht22_temp_pub_topic) );
	cgiText( cd, "mqtt-dht22-humi-pub-topic", (char *)config()->mqtt_dht22_humi_pub_topic, sizeof(config()->mqtt_dht22_humi_pub_topic) );
	cgiText( cd, "mqtt-ds18b20-temp-pub-topic", (char *)config()->mqtt_ds18b20_temp_pub_topic, sizeof(config()->mqtt_ds18b20_temp_pub_topic) );

	CFG_Save();
	httpdRedirect(cd, "/");
	return HTTPD_CGI_DONE;
}
