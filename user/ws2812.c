#include <c_types.h>
#include <esp8266.h>
#include <espconn.h>
#include "ws2812_i2s.h"
#include "user_config.h"
#include "config.h"

static struct espconn *pUdpServer;
uint8_t last_leds[512*3];	// ??? what for ??
int last_led_count = 0;

//Called when new packet comes in.
static void ICACHE_FLASH_ATTR
udpserver_recv(void *arg, char *pusrdata, unsigned short len){
	//struct espconn *pespconn = (struct espconn *)arg;

	os_printf("X");

	ws2812_i2s_push( (uint8_t*)pusrdata + 3, len - 3 );

	len -= 3;
	if( len > sizeof(last_leds) + 3 ){
		len = sizeof(last_leds) + 3;
	}
	ets_memcpy( last_leds, pusrdata+3, len );
	last_led_count = len / 3;
}

void ICACHE_FLASH_ATTR ws2812_init(void){
	pUdpServer = (struct espconn *)os_zalloc(sizeof(struct espconn));
	ets_memset( pUdpServer, 0, sizeof( struct espconn ) );
	espconn_create( pUdpServer );
	pUdpServer->type = ESPCONN_UDP;
	pUdpServer->proto.udp = (esp_udp *)os_zalloc(sizeof(esp_udp));
	pUdpServer->proto.udp->local_port = config()->light_udp_port;
	espconn_regist_recvcb(pUdpServer, udpserver_recv);

	if( espconn_create( pUdpServer ) ) {
		os_printf( "WS2812:UDP(%ld) server was not started!\n", config()->light_udp_port );
	} else 
		ws2812_i2s_init();
}
