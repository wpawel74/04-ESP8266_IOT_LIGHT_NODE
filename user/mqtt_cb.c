/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */
#include <esp8266.h>
#include "espmissingincludes.h"
#include "ets_sys.h"
#include "httpclient.h"
#include "mqtt.h"
#include "config.h"
#include "io.h"

MQTT_Client mqttClient;

void ICACHE_FLASH_ATTR http_callback_IP(char * response, int http_status, char * full_response){
	//os_printf("http_status=%d\n", http_status);
	if (http_status != HTTP_STATUS_GENERIC_ERROR) {
		//os_printf("strlen(full_response)=%d\n", strlen(full_response));
		//os_printf("response=%s<EOF>\n", response);
		os_printf("External IP address=%s\n", response);
	}
}

void ICACHE_FLASH_ATTR wifiConnectCb(uint8_t status){
	if(status == STATION_GOT_IP){

		os_printf("Trying to find external IP address\n");
		http_get("http://wtfismyip.com/text", http_callback_IP);

		if(sysCfg.mqtt_enable==1) {
			MQTT_Connect(&mqttClient);
		} else {
			MQTT_Disconnect(&mqttClient);
		}
	}	
}

void ICACHE_FLASH_ATTR mqttConnectedCb(uint32_t *args){
	MQTT_Client* client = (MQTT_Client*)args;
	os_printf("MQTT: Connected\r\n");
	MQTT_Subscribe(client, (char *)sysCfg.mqtt_relay_subs_topic,0);
}

void ICACHE_FLASH_ATTR mqttDisconnectedCb(uint32_t *args)
{
//	MQTT_Client* client = (MQTT_Client*)args;
	os_printf("MQTT: Disconnected\r\n");
}

void ICACHE_FLASH_ATTR mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t lengh){
	char strTopic[topic_len + 1];
	os_memcpy(strTopic, topic, topic_len);
	strTopic[topic_len] = '\0';

	char strData[lengh + 1];
	os_memcpy(strData, data, lengh);
	strData[lengh] = '\0';

	char relayNum=strTopic[topic_len-1];
	char strSubsTopic[strlen((char *)sysCfg.mqtt_relay_subs_topic)];
	os_strcpy(strSubsTopic,(char *)sysCfg.mqtt_relay_subs_topic);
	strSubsTopic[(strlen((char *)sysCfg.mqtt_relay_subs_topic)-1)]=relayNum;

	os_printf("MQTT strSubsTopic: %s, strTopic: %s \r\n", strSubsTopic, strTopic);

	if (os_strcmp(strSubsTopic,strTopic) == 0 ) {
		os_printf("Relay %d is now: %s \r\n", relayNum-'0', strData);

		if(relayNum=='1')
			io_GPIOSet(atoi(strData), GPIO_RELAY1);

		if(relayNum=='2')
			io_GPIOSet(atoi(strData), GPIO_RELAY2);

		if( sysCfg.relay_latching_enable) {
			sysCfg.relay_1_state = io_GPIOGet(GPIO_RELAY1);
			sysCfg.relay_2_state = io_GPIOGet(GPIO_RELAY2);
			CFG_Save();
		}
	}
	os_printf("MQTT topic: %s, data: %s \r\n", strTopic, strData);
}

void ICACHE_FLASH_ATTR mqttPublishedCb(uint32_t *args)
{
//    MQTT_Client* client = (MQTT_Client*)args;
	os_printf("MQTT: Published\r\n");
}
