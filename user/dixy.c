#include <esp8266.h>
#include <espconn.h>
#include "user_config.h"
#include "config.h"

static struct espconn *pUdpServer = NULL;
static void (*rgb_sync)( const char *b, int size ) = NULL;

//Called when new packet comes in.
static void ICACHE_FLASH_ATTR dixy_udpserver_recv(void *arg, char *data, unsigned short len){
	//struct espconn *pespconn = (struct espconn *)arg;

	os_printf("X");

	if( rgb_sync )
		rgb_sync( data + 3, len - 3 );
}

void ICACHE_FLASH_ATTR dixy_service_ini( void (*rgb_sync)( const char *b, int size ) ){
	pUdpServer = (struct espconn *)os_zalloc(sizeof(struct espconn));
	ets_memset( pUdpServer, 0, sizeof( struct espconn ) );
	espconn_create( pUdpServer );
	pUdpServer->type = ESPCONN_UDP;
	pUdpServer->proto.udp = (esp_udp *)os_zalloc(sizeof(esp_udp));
	pUdpServer->proto.udp->local_port = config()->light_udp_port;
	espconn_regist_recvcb(pUdpServer, dixy_udpserver_recv);

	if( espconn_create( pUdpServer ) ) {
		os_printf( "WS2812:UDP(%ld) server was not started!\n", config()->light_udp_port );
	}
}
