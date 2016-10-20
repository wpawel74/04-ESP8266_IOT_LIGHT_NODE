#include <esp8266.h>
#include <user_config.h>
#include "config.h"
#include "io.h"
#include "ws2812.h"
#include "ws2812_i2s.h"

static bool G_on = false;
static ETSTimer G_light_timer;

typedef enum {
	SIMPLE_OFF = 0,
	SIMPLE_ON  = 1,
} light_style;

static void ICACHE_FLASH_ATTR light_set_style( light_style style ){
	int it = 0;

	switch( style ){
	case SIMPLE_OFF:
#ifdef CONFIG_WS2812
		for( it = 0; it < config()->light_chain_size ; it++ ){
			uint8_t ws2812_sled[ 3 ] = { 0x00, 0x00, 0x00 };
			ws2812_i2s_push( ws2812_sled, sizeof(ws2812_sled) );
		}
#endif // CONFIG_WS2812

		break;
	case SIMPLE_ON:
#ifdef CONFIG_WS2812
		for( it = 0; it < config()->light_chain_size ; it++ ){
			uint8_t ws2812_sled[ 3 ] = { 0xff, 0xff, 0xff };
			ws2812_i2s_push( ws2812_sled, sizeof(ws2812_sled) );
		}
#endif // CONFIG_WS2812

		break;
	default:
		break;
	}
}

void ICACHE_FLASH_ATTR light_enable_power_supply( bool on ) {
	switch( config()->light_power_supply ){
	case RELAY_1:
		sysCfg.relay_1_state = on;
		ioGPIO((int)sysCfg.relay_1_state, RELAY1_GPIO);
		break;
	case RELAY_2:
		sysCfg.relay_2_state = on;
		ioGPIO((int)sysCfg.relay_2_state, RELAY2_GPIO);
		break;
	case DONT_CARE:
	default:
		break;
	}
}

static void ICACHE_FLASH_ATTR light_delay_timer_callback(void *arg) {
	bool	*on = (bool *)arg;
	if( !on )
		light_enable_power_supply( on );
	else 
		light_set_style( *on ? SIMPLE_ON: SIMPLE_OFF );
}

void ICACHE_FLASH_ATTR light_enable( bool on ) {

	if( on )
		light_enable_power_supply( on );
	else
		light_set_style( on ? SIMPLE_ON: SIMPLE_OFF );

	G_on = on;
	os_timer_disarm(&G_light_timer);
	os_timer_setfn(&G_light_timer, light_delay_timer_callback, &G_on);
	os_timer_arm(&G_light_timer, on ? config()->light_delay_power_on: config()->light_delay_power_off, 0);
}

bool light_is_enabled(void){
	return G_on;
}

void light_ini(void) {
	os_timer_disarm(&G_light_timer);

#ifdef CONFIG_WS2812
	ws2812_init();
#endif // CONFIG_WS2812

}
