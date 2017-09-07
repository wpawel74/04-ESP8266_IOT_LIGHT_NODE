/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */

/*
This is example code for the esphttpd library. It's a small-ish demo showing off 
the server, including WiFi connection management capabilities, some IO and
some pictures of cats.
*/

#include <esp8266.h>
#include <i2c_master.h>
#include <uart.h>
#include <sntp.h>

#include "user_config.h"
#include "httpd.h"
#include "io.h"
#include "httpdespfs.h"
#include "cgi.h"
#include "cgiwifi.h"
#include "cgiflash.h"
#include "auth.h"
#include "espfs.h"
#include "captdns.h"
#include "webpages-espfs.h"
#include "cgiwebsocket.h"
#include "config.h"
#include "broadcastd.h"
#include "mqtt.h"
#include "mqtt_cb.h"
#include "dht22.h"
#include "ds18b20.h"
#include "bmp180.h"
#include "light.h"
#include "ds1307.h"
#include "alarm.h"

//The example can print out the heap use every 3 seconds. You can use this to catch memory leaks.
//#define SHOW_HEAP_USE

//Function that tells the authentication system what users/passwords live on the system.
//This is disabled in the default build; if you want to try it, enable the authBasic line in
//the builtInUrls below.
int myPassFn(HttpdConnData *connData, int no, char *user, int userLen, char *pass, int passLen) {
	if (no==0) {
		os_strcpy(user, "admin");
		os_strcpy(pass, "s3cr3t");
		return 1;
//Add more users this way. Check against incrementing no for each user added.
//	} else if (no==1) {
//		os_strcpy(user, "user1");
//		os_strcpy(pass, "something");
//		return 1;
	}
	return 0;
}

static ETSTimer websockTimer;

//Broadcast the uptime in seconds every second over connected websockets
static void ICACHE_FLASH_ATTR websockTimerCb(void *arg) {
	static int ctr=0;
	char buff[128];
	ctr++;
	os_sprintf(buff, "Up for %d minutes %d seconds!\n", ctr/60, ctr%60);
	cgiWebsockBroadcast("/websocket/ws.cgi", buff, os_strlen(buff), WEBSOCK_FLAG_NONE);
}

#ifdef ESPFS_POS
CgiUploadFlashDef uploadParams={
	.type=CGIFLASH_TYPE_ESPFS,
	.fw1Pos=ESPFS_POS,
	.fw2Pos=0,
	.fwSize=ESPFS_SIZE,
};
#define INCLUDE_FLASH_FNS
#endif
#ifdef OTA_FLASH_SIZE_K
CgiUploadFlashDef uploadParams={
	.type=CGIFLASH_TYPE_FW,
	.fw1Pos=0x1000,
	.fw2Pos=((OTA_FLASH_SIZE_K*1024)/2)+0x1000,
	.fwSize=((OTA_FLASH_SIZE_K*1024)/2)-0x1000,
	.tagName=OTA_TAGNAME
};
#define INCLUDE_FLASH_FNS
#endif

/*
This is the main url->function dispatching data struct.
In short, it's a struct with various URLs plus their handlers. The handlers can
be 'standard' CGI functions you wrote, or 'special' CGIs requiring an argument.
They can also be auth-functions. An asterisk will match any url starting with
everything before the asterisks; "*" matches everything. The list will be
handled top-down, so make sure to put more specific rules above the more
general ones. Authorization things (like authBasic) act as a 'barrier' and
should be placed above the URLs they protect.
*/
HttpdBuiltInUrl builtInUrls[]={
	{"*", cgiRedirectApClientToHostname, "esp8266.nonet"},
	{"/", cgiRedirect, "/index.tpl"},
	{"/led.tpl", cgiEspFsTemplate, tplLed},
	{"/index.tpl", cgiEspFsTemplate, tplCounter},
	{"/about.tpl", cgiEspFsTemplate, tplAbout},
	{"/led.cgi", cgiLed, NULL},

	{"/flash/download", cgiReadFlash, NULL},
#ifdef INCLUDE_FLASH_FNS
	{"/flash/next", cgiGetFirmwareNext, &uploadParams},
	{"/flash/upload", cgiUploadFirmware, &uploadParams},
#endif
	{"/flash/reboot", cgiRebootFirmware, NULL},

	//Routines to make the /wifi URL and everything beneath it work.

//Enable the line below to protect the WiFi configuration with an username/password combo.
//	{"/wifi/*", authBasic, myPassFn},

	{"/config/wifi", cgiRedirect, "/config/wifi/wifi.tpl"},
	{"/config/wifi/", cgiRedirect, "/config/wifi/wifi.tpl"},
	{"/config/wifi/wifiscan.cgi", cgiWiFiScan, NULL},
	{"/config/wifi/wifi.tpl", cgiEspFsTemplate, tplWlan},
	{"/config/wifi/connect.cgi", cgiWiFiConnect, NULL},
	{"/config/wifi/connstatus.cgi", cgiWiFiConnStatus, NULL},
	{"/config/wifi/setmode.cgi", cgiWiFiSetMode, NULL},
	{"/config/mqtt.tpl", cgiEspFsTemplate, tplMQTT},
	{"/config/mqtt.cgi", cgiMQTT, NULL},
	{"/config/httpd.tpl", cgiEspFsTemplate, tplHTTPD},
	{"/config/httpd.cgi", cgiHTTPD, NULL},
	{"/config/broadcastd.tpl", cgiEspFsTemplate, tplBroadcastD},
	{"/config/broadcastd.cgi", cgiBroadcastD, NULL},
	{"/config/ntp.tpl", cgiEspFsTemplate, tplNTP},
	{"/config/ntp.cgi", cgiNTP, NULL},
	{"/config/relay.tpl", cgiEspFsTemplate, tplRLYSettings},
	{"/config/relay.cgi", cgiRLYSettings, NULL},
	{"/config/sensor.tpl", cgiEspFsTemplate, tplSensorSettings},
	{"/config/sensor.cgi", cgiSensorSettings, NULL},
	{"/config/rtc.tpl", cgiEspFsTemplate, tplRTC},
	{"/config/rtc.cgi", cgiRTC, NULL},
	{"/config/light.tpl", cgiEspFsTemplate, tplLightSettings},
	{"/config/light.cgi", cgiLightSettings, NULL},
	{"/config/alarm.tpl", cgiEspFsTemplate, tplAlarmSettings},
	{"/config/alarm.cgi", cgiAlarmSettings, NULL},

	{"/control/ui.tpl", cgiEspFsTemplate, tplUI},

#ifdef CONFIG_RELAYS
	{"/control/relay.tpl", cgiEspFsTemplate, tplGPIO},
	{"/control/relay.cgi", cgiGPIO, NULL},
#endif // CONFIG_RELAYS
#ifdef CONFIG_DTH22
	{"/control/dht22.tpl", cgiEspFsTemplate, tplDHT},
	{"/control/dht22.cgi", cgiDHT22, NULL}, 
#endif // CONFIG_DTH22
#ifdef CONFIG_DS18B20
	{"/control/ds18b20.tpl", cgiEspFsTemplate, tplDS18b20},
	{"/control/ds18b20.cgi", cgiDS18b20, NULL}, 
#endif // CONFIG_DS18B20
#ifdef CONFIG_BMP180
	{"/control/bmp180.tpl", cgiEspFsTemplate, tplBMP180},
	{"/control/bmp180.cgi", cgiBMP180, NULL}, 
#endif // CONFIG_BMP180

	{"/control/light.tpl", cgiEspFsTemplate, tplLight},
	{"/control/light.cgi", cgiLight, NULL},

	{"/control/state.cgi", cgiState, NULL}, 
	{"/control/reset.cgi", cgiReset, NULL}, 

	{"*", cgiEspFsHook, NULL}, //Catch-all cgi function for the filesystem
	{NULL, NULL, NULL}
};


#ifdef SHOW_HEAP_USE
static ETSTimer prHeapTimer;

static void ICACHE_FLASH_ATTR prHeapTimerCb(void *arg) {
	os_printf("Heap: %ld\n", (unsigned long)system_get_free_heap_size());
}
#endif

//Main routine. Initialize stdout, the I/O, filesystem and the webserver and we're done.
void user_init(void) {

	//uart_init(BIT_RATE_115200,BIT_RATE_115200);
	extern void stdoutInit(void);
	stdoutInit();
	os_delay_us(100000);
	CFG_Load();
	io_GPIOIni();
	i2c_master_gpio_init();

	// 0x40200000 is the base address for spi flash memory mapping, ESPFS_POS is the position
	// where image is written in flash that is defined in Makefile.
#ifdef ESPFS_POS
	espFsInit((void*)(0x40200000 + ESPFS_POS));
#else
	espFsInit((void*)(webpages_espfs_start));
#endif // ESPFS_POS

	httpdInit(builtInUrls, 80); //sysCfg.httpd_port);

	if(sysCfg.ntp_enable) {
		sntp_set_timezone(config()->ntp_tz);	//timezone
		sntp_setservername( 0, config()->ntp_servers[0] );
		sntp_setservername( 1, config()->ntp_servers[1] );
		sntp_setservername( 2, config()->ntp_servers[2] );
		sntp_init();
	}

#ifdef CONFIG_DTH22
	if(sysCfg.sensor_dht22_enable) 
		DHTInit(SENSOR_DHT22, 30000);
#endif // CONFIG_DTH22
#ifdef CONFIG_DS18B20
	if(sysCfg.sensor_ds18b20_enable) 
		ds_init(30000);
#endif // CONFIG_DS18B20
#ifdef CONFIG_BMP180
	if(sysCfg.sensor_bmp180_enable){
		if( BMP180_Init() ){
			int32_t temp, pressure;
			if( BMP180_GetTemperature( &temp ) == 0 && BMP180_GetPressure(OSS_0, &pressure ) == 0 )
				os_printf( "BMP180: temperature %ld.%ld C, air pressure %ld Pa\n", temp/10, temp - ((temp/10)*10), pressure );
		}
	}
#endif // CONFIG_BMP180
#ifdef CONFIG_DS1307
	{
	struct tm tm;
	if( ds1307_getTime(&tm) ){
		time_t time;
		extern time_t ICACHE_FLASH_ATTR mktime(struct tm *timep);
		extern int ICACHE_FLASH_ATTR stime(time_t *t);

		time = mktime( &tm );
		//
		// set system time based on date and time from RTC
		//
		stime( &time );
		os_printf( "RTC: date %d/%02d/%02d %02d:%02d:%02d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec );
	}
	}
#endif // CONFIG_DS1307

#ifdef CONFIG_WS2812
	light_ini();
#endif // CONFIG_WS2812

#ifdef CONFIG_MQTT
	if(sysCfg.mqtt_enable) {
		MQTT_InitConnection(&mqttClient, (uint8_t *)sysCfg.mqtt_host, sysCfg.mqtt_port, sysCfg.mqtt_use_ssl );
		MQTT_InitClient(&mqttClient, (uint8_t *)sysCfg.mqtt_devid, (uint8_t *)sysCfg.mqtt_user, (uint8_t *)sysCfg.mqtt_pass, sysCfg.mqtt_keepalive,1);
		MQTT_OnConnected(&mqttClient, mqttConnectedCb);
		MQTT_OnDisconnected(&mqttClient, mqttDisconnectedCb);
		MQTT_OnPublished(&mqttClient, mqttPublishedCb);
		MQTT_OnData(&mqttClient, mqttDataCb);
	}
	broadcastd_init();
#endif // CONFIG_MQTT

	alarm_ini();

#ifdef SHOW_HEAP_USE
	os_timer_disarm(&prHeapTimer);
	os_timer_setfn(&prHeapTimer, prHeapTimerCb, NULL);
	os_timer_arm(&prHeapTimer, 3000, 1);
#endif
	os_timer_disarm(&websockTimer);
	os_timer_setfn(&websockTimer, websockTimerCb, NULL);
	os_timer_arm(&websockTimer, 1000, 1);
	os_printf("\nReady\n");
}

/******************************************************************************
 * FunctionName : user_rf_cal_sector_set
 * Description  : SDK just reversed 4 sectors, used for rf init data and paramters.
 *                We add this function to force users to set rf cal sector, since
 *                we don't know which sector is free in user's application.
 *                sector map for last several sectors : ABCCC
 *                A : rf cal
 *                B : rf init data
 *                C : sdk parameters
 * Parameters   : none
 * Returns      : rf cal sector
*******************************************************************************/
uint32 ICACHE_FLASH_ATTR
user_rf_cal_sector_set(void)
{
	enum flash_size_map size_map = system_get_flash_size_map();
	uint32 rf_cal_sec = 0;

	switch (size_map) {
	case FLASH_SIZE_4M_MAP_256_256:
		rf_cal_sec = 128 - 5;
		break;

	case FLASH_SIZE_8M_MAP_512_512:
		rf_cal_sec = 256 - 5;
		break;

	case FLASH_SIZE_16M_MAP_512_512:
	case FLASH_SIZE_16M_MAP_1024_1024:
		rf_cal_sec = 512 - 5;
		break;

	case FLASH_SIZE_32M_MAP_512_512:
	case FLASH_SIZE_32M_MAP_1024_1024:
		rf_cal_sec = 1024 - 5;
		break;

	default:
		rf_cal_sec = 0;
		break;
	}

	return rf_cal_sec;
}

void user_rf_pre_init() {
	//Not needed, but some SDK versions want this defined.
}
