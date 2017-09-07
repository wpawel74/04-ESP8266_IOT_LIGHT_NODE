#include <esp8266.h>
#include "config.h"
#include "ws2812_i2s.h"
#include "fx.h"

struct fx      *G_fx_head = NULL;
char *G_fx_leds = NULL;
static void (*G_fx_rgb_sync)( const char *buff, int size ) = NULL;
static ETSTimer G_fx_timer;

static bool ICACHE_FLASH_ATTR fx_is_registered( const struct fx *fx ){
	struct fx *node = G_fx_head;

	while( node ){
		if( node == fx )
			return true;
		node = node->next;
	}
	return false;
}

static void ICACHE_FLASH_ATTR fx_add( struct fx *new ){
	struct fx *node = G_fx_head,
			  *prev = NULL;

	while( node ){
		if( node->prio > new->prio ){
			if( prev )
				prev->next = new;
			else
				G_fx_head = new;
			new->next = node;
			return;
		}
		prev = node;
		node = node->next;
	}
	if( prev )
		prev->next = new;
	else
		G_fx_head = new;
}

static void ICACHE_FLASH_ATTR fx_del( struct fx *fx ){
	struct fx *node = G_fx_head,
			  *prev = NULL;

	while( node ){
		if( node == fx ){
			if( prev )
				prev->next = fx->next;
			else
				G_fx_head = fx->next;
			return;
		}
		prev = node;
		node = node->next;
	}
}

static void ICACHE_FLASH_ATTR fx_poll(void){
	struct fx *node = G_fx_head;
	u32		pattern = 0;

	while( node ){
		if( ((pattern ^ node->pattern) & node->pattern) == node->pattern ){
			if( (node->fx.begin + (node->fx.end - node->fx.begin)) < config()->light_chain_size )
				node->fx.algo( node->fx.prv, (char *)G_fx_leds + (node->fx.begin * sizeof(struct RGB)), node->fx.end - node->fx.begin );
			pattern |= node->pattern;
		}
		node = node->next;
	}
}

static void ICACHE_FLASH_ATTR fx_timer_callback(void *arg){
	int it;
	if( G_fx_leds == NULL )
		return;

	fx_poll();

	for( it = 0; it < config()->light_chain_size; it++ )
		os_printf( "%x.%x.%x ", G_fx_leds[ it * sizeof(struct RGB) ], G_fx_leds[ it * sizeof(struct RGB) + 1 ] , G_fx_leds[ it * sizeof(struct RGB) + 2 ] );
	os_printf("\n");

	if( G_fx_rgb_sync )
		G_fx_rgb_sync( G_fx_leds, config()->light_chain_size * sizeof(struct RGB) );
}

void ICACHE_FLASH_ATTR fx_ini(void (*rgb_sync)( const char *b, int size ) ){
	if( G_fx_leds == NULL )
		G_fx_leds = os_malloc( config()->light_chain_size * sizeof(struct RGB) );
	memset( G_fx_leds, 0x00, config()->light_chain_size * sizeof(struct RGB) );
	G_fx_rgb_sync = rgb_sync;
	os_timer_disarm(&G_fx_timer);
	os_timer_setfn(&G_fx_timer, fx_timer_callback, NULL);
}

void ICACHE_FLASH_ATTR fx_dei(void){
	os_timer_disarm(&G_fx_timer);
	if( G_fx_leds )
		os_free( G_fx_leds );
	G_fx_leds = NULL;
}

void ICACHE_FLASH_ATTR fx_stop(void){
	os_timer_disarm(&G_fx_timer);
}

void ICACHE_FLASH_ATTR fx_start(void){
	os_timer_arm(&G_fx_timer, config()->fx_poll_time, 1);
}

void ICACHE_FLASH_ATTR fx_register( struct fx *fx ){
	if( fx_is_registered( fx ) == TRUE ){
		return;
	}
	fx->fx.start( fx->fx.prv );
	fx_add( fx );
	os_printf( "D: F/X: %s effect added\n", fx->name ? fx->name: "(null)" );
}

void ICACHE_FLASH_ATTR fx_deregister( struct fx *fx ){
	if( fx_is_registered( fx ) == TRUE ){
		fx_del( fx );
		fx->next = NULL;
		fx->fx.stop( fx->fx.prv );
		os_printf( "D: F/X: %s effect removed\n", fx->name ? fx->name: "(null)" );
	}
}
