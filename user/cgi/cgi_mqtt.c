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

void ICACHE_FLASH_ATTR tplMQTT(HttpdConnData *connData, char *token, void **arg) {
	char buff[128];
	if (token==NULL) return;

	os_strcpy(buff, "Unknown");

	if (os_strcmp(token, "mqtt-enable")==0) {
		os_sprintf(buff, "%d", (int)sysCfg.mqtt_enable);
	}

	if (os_strcmp(token, "mqtt-use-ssl")==0) {
		os_sprintf(buff,"%d", (int)sysCfg.mqtt_use_ssl);
	}

	if (os_strcmp(token, "mqtt-host")==0) {
		os_strcpy(buff, (char *)sysCfg.mqtt_host);
	}

	if (os_strcmp(token, "mqtt-port")==0) {
		os_sprintf(buff, "%d", (int)sysCfg.mqtt_port);
	}

	if (os_strcmp(token, "mqtt-keepalive")==0) {
		os_sprintf(buff, "%d", (int)sysCfg.mqtt_keepalive);
	}

	if (os_strcmp(token, "mqtt-devid")==0) {
		os_strcpy(buff, (char *)sysCfg.mqtt_devid);
	}

	if (os_strcmp(token, "mqtt-user")==0) {
		os_strcpy(buff, (char *)sysCfg.mqtt_user);
	}

	if (os_strcmp(token, "mqtt-pass")==0) {
		os_strcpy(buff, (char *)sysCfg.mqtt_pass);
	}

	if (os_strcmp(token, "mqtt-relay-subs-topic")==0) {
		os_strcpy(buff, (char *)sysCfg.mqtt_relay_subs_topic);
	}

	if (os_strcmp(token, "mqtt-dht22-temp-pub-topic")==0) {
		os_strcpy(buff, (char *)sysCfg.mqtt_dht22_temp_pub_topic);
	}

	if (os_strcmp(token, "mqtt-dht22-humi-pub-topic")==0) {
		os_strcpy(buff, (char *)sysCfg.mqtt_dht22_humi_pub_topic);
	}

	if (os_strcmp(token, "mqtt-ds18b20-temp-pub-topic")==0) {
		os_strcpy(buff, (char *)sysCfg.mqtt_ds18b20_temp_pub_topic);
	}

	httpdSend(connData, buff, -1);
}

int ICACHE_FLASH_ATTR cgiMQTT(HttpdConnData *connData) {
	char buff[128];
	int len;

	if (connData->conn==NULL) {
		//Connection aborted. Clean up.
		return HTTPD_CGI_DONE;
	}

	len=httpdFindArg(connData->post->buff, "mqtt-enable", buff, sizeof(buff));
	sysCfg.mqtt_enable = (len > 0) ? 1:0;

	len=httpdFindArg(connData->post->buff, "mqtt-use-ssl", buff, sizeof(buff));
	sysCfg.mqtt_use_ssl = (len > 0) ? 1:0;

	len=httpdFindArg(connData->post->buff, "mqtt-host", buff, sizeof(buff));
	if (len>0) {
		os_sprintf((char *)sysCfg.mqtt_host,buff);
	}

	len=httpdFindArg(connData->post->buff, "mqtt-port", buff, sizeof(buff));
	if (len>0) {
		sysCfg.mqtt_port=atoi(buff);
	}

	len=httpdFindArg(connData->post->buff, "mqtt-keepalive", buff, sizeof(buff));
	if (len>0) {
		sysCfg.mqtt_keepalive=atoi(buff);
	}

	len=httpdFindArg(connData->post->buff, "mqtt-devid", buff, sizeof(buff));
	if (len>0) {
		os_sprintf((char *)sysCfg.mqtt_devid,buff);
	}

	len=httpdFindArg(connData->post->buff, "mqtt-user", buff, sizeof(buff));
	if (len>0) {
		os_sprintf((char *)sysCfg.mqtt_user,buff);
	}

	len=httpdFindArg(connData->post->buff, "mqtt-pass", buff, sizeof(buff));
	if (len>0) {
		os_sprintf((char *)sysCfg.mqtt_pass,buff);
	}

	len=httpdFindArg(connData->post->buff, "mqtt-relay-subs-topic", buff, sizeof(buff));
	if (len>0) {
		os_sprintf((char *)sysCfg.mqtt_relay_subs_topic,buff);
	}

	len=httpdFindArg(connData->post->buff, "mqtt-dht22-temp-pub-topic", buff, sizeof(buff));
	if (len>0) {
		os_sprintf((char *)sysCfg.mqtt_dht22_temp_pub_topic,buff);
	}

	len=httpdFindArg(connData->post->buff, "mqtt-dht22-humi-pub-topic", buff, sizeof(buff));
	if (len>0) {
		os_sprintf((char *)sysCfg.mqtt_dht22_humi_pub_topic,buff);
	}

	len=httpdFindArg(connData->post->buff, "mqtt-ds18b20-temp-pub-topic", buff, sizeof(buff));
	if (len>0) {
		os_sprintf((char *)sysCfg.mqtt_ds18b20_temp_pub_topic,buff);
	}

	CFG_Save();

	httpdRedirect(connData, "/");
	return HTTPD_CGI_DONE;
}
