#include <esp8266.h>
#include <user_config.h>
#include "config.h"
#include "io.h"
#include "ws2812_i2s.h"
#include "fx.h"

static bool G_on = false;
static ETSTimer G_light_timer;

typedef enum {
	SIMPLE_OFF = 0,
	SIMPLE_ON  = 1,
	USE_FX = 2
} light_style;

static void ICACHE_FLASH_ATTR light_set_style( light_style style ){
	extern char *G_fx_leds;
	int it = 0;

	switch( style ){
	case SIMPLE_OFF:
		fx_stop();
#ifdef CONFIG_WS2812
		for( it = 0; it < config()->light_chain_size ; it++ ){
			if( G_fx_leds ){
				G_fx_leds[ (it * sizeof(struct RGB)) ] = 0;
				G_fx_leds[ (it * sizeof(struct RGB)) + 1 ] = 0;
				G_fx_leds[ (it * sizeof(struct RGB)) + 2 ] = 0;
			}
			ws2812_i2s_push( G_fx_leds, config()->light_chain_size * sizeof(struct RGB) );
		}
#endif // CONFIG_WS2812

		break;
	case SIMPLE_ON:
#ifdef CONFIG_WS2812
		for( it = 0; it < config()->light_chain_size ; it++ ){
			if( G_fx_leds ){
				G_fx_leds[ (it * sizeof(struct RGB)) ] = 0xff;
				G_fx_leds[ (it * sizeof(struct RGB)) + 1 ] = 0xff;
				G_fx_leds[ (it * sizeof(struct RGB)) + 2 ] = 0xff;
			}
			ws2812_i2s_push( G_fx_leds, config()->light_chain_size * sizeof(struct RGB) );
		}
#endif // CONFIG_WS2812

		break;
	case USE_FX:
		for( it = 0; it < config()->light_chain_size ; it++ ){
			if( G_fx_leds ){
				G_fx_leds[ (it * sizeof(struct RGB)) ] = 0;
				G_fx_leds[ (it * sizeof(struct RGB)) + 1 ] = 0;
				G_fx_leds[ (it * sizeof(struct RGB)) + 2 ] = 0;
			}
			ws2812_i2s_push( G_fx_leds, config()->light_chain_size * sizeof(struct RGB) );
		}
		fx_start();
		break;
	default:
		break;
	}
}

void ICACHE_FLASH_ATTR light_enable_power_supply( bool on ) {
	switch( config()->light_power_supply ){
	case RELAY_1:
		sysCfg.relay_1_state = on;
		io_GPIOSet((int)sysCfg.relay_1_state, GPIO_RELAY1);
		break;
	case RELAY_2:
		sysCfg.relay_2_state = on;
		io_GPIOSet((int)sysCfg.relay_2_state, GPIO_RELAY2);
		break;
	case DONT_CARE:
	default:
		break;
	}
}

static bool ICACHE_FLASH_ATTR light_is_fx_active(void){
	if( config()->fx_simple_1_enable ||
	    config()->fx_simple_2_enable ||
	    config()->fx_flames_1_enable ||
	    config()->fx_pulsar_1_enable ) return true;
	return false;
}

static void ICACHE_FLASH_ATTR light_delay_timer_callback(void *arg) {
	bool	*on = (bool *)arg;
	if( !(*on) ) light_enable_power_supply( *on );
	else light_set_style( light_is_fx_active() ? USE_FX: SIMPLE_ON );
}

void ICACHE_FLASH_ATTR light_enable( bool on ) {

	if( on ) light_enable_power_supply( on );
	else light_set_style( SIMPLE_OFF );

	G_on = on;
	os_timer_disarm(&G_light_timer);
	os_timer_setfn(&G_light_timer, light_delay_timer_callback, &G_on);
	os_timer_arm(&G_light_timer, on ? config()->light_delay_power_on: config()->light_delay_power_off, 0);
	os_printf( "light event received: %s\n", on ? "enable" : "disable" );
}

bool ICACHE_FLASH_ATTR light_is_enabled(void){
	return G_on;
}

void ICACHE_FLASH_ATTR light_fx_reload(void){
	extern void ICACHE_FLASH_ATTR simple_set_RGB( void *prv, const struct RGB *rgb );
	extern struct fx G_fx_simple_1;
	G_fx_simple_1.fx.begin = config()->fx_simple_1_start_no;
	G_fx_simple_1.fx.end = config()->fx_simple_1_stop_no;
	simple_set_RGB( G_fx_simple_1.fx.prv, &config()->fx_simple_1_RGB );
	if( config()->fx_simple_1_enable )
		fx_register( &G_fx_simple_1 );
	else
		fx_deregister( &G_fx_simple_1 );

	extern struct fx G_fx_simple_2;
	G_fx_simple_2.fx.begin = config()->fx_simple_2_start_no;
	G_fx_simple_2.fx.end = config()->fx_simple_2_stop_no;
	simple_set_RGB( G_fx_simple_2.fx.prv, &config()->fx_simple_2_RGB );
	if( config()->fx_simple_2_enable )
		fx_register( &G_fx_simple_2 );
	else
		fx_deregister( &G_fx_simple_2 );

	extern struct fx G_fx_flames_1;
	G_fx_flames_1.fx.begin = config()->fx_flames_1_start_no;
	G_fx_flames_1.fx.end = config()->fx_flames_1_stop_no;
	if( config()->fx_flames_1_enable )
		fx_register( &G_fx_flames_1 );
	else
		fx_deregister( &G_fx_flames_1 );

	extern void ICACHE_FLASH_ATTR pulsar_set_RGB_dizzy( void *prv, const struct RGB *rgb );
	extern void ICACHE_FLASH_ATTR pulsar_set_RGB_fuzzy( void *prv, const struct RGB *rgb );
	extern void ICACHE_FLASH_ATTR pulsar_set_delay( void *prv, uint32_t delay );
	extern struct fx G_fx_pulsar_1;
	G_fx_pulsar_1.fx.begin = config()->fx_pulsar_1_start_no;
	G_fx_pulsar_1.fx.end = config()->fx_pulsar_1_stop_no;
	G_fx_pulsar_1.fx.end = config()->fx_pulsar_1_stop_no;
	pulsar_set_RGB_fuzzy( G_fx_pulsar_1.fx.prv, &config()->fx_pulsar_1_RGB_fuzzy );
	pulsar_set_RGB_dizzy( G_fx_pulsar_1.fx.prv, &config()->fx_pulsar_1_RGB_dizzy );

	if( config()->fx_pulsar_1_enable )
		fx_register( &G_fx_pulsar_1 );
	else
		fx_deregister( &G_fx_pulsar_1 );

}

void ICACHE_FLASH_ATTR light_ini(void) {
	extern void dixy_service_ini( void (*rgb_sync)( const char *b, int size ) );
	void (*rgb_sync)( const char *b, int size ) = NULL;

	os_timer_disarm(&G_light_timer);

#ifdef CONFIG_WS2812
	rgb_sync = ws2812_i2s_push;
	ws2812_i2s_init();
#endif // CONFIG_WS2812

	dixy_service_ini(rgb_sync);
	fx_ini(rgb_sync);
	light_fx_reload();
}
