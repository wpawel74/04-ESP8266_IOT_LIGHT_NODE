#include <esp8266.h>
#include <user_config.h>
#include "config.h"
#include "io.h"
#include "ws2812.h"
#include "ws2812_i2s.h"
#include "fx.h"

static bool G_on = false;
static ETSTimer G_light_timer;

typedef enum {
	SIMPLE_OFF = 0,
	SIMPLE_ON  = 1,
} light_style;

static void ICACHE_FLASH_ATTR light_set_style( light_style style ){
	extern uint8_t *G_fx_leds;
	int it = 0;

	switch( style ){
	case SIMPLE_OFF:
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
	if( !(*on) ) light_enable_power_supply( *on );
	else light_set_style( SIMPLE_ON );
}

void ICACHE_FLASH_ATTR light_enable( bool on ) {

	if( on ) light_enable_power_supply( on );
	else light_set_style( SIMPLE_OFF );

	G_on = on;
	os_timer_disarm(&G_light_timer);
	os_timer_setfn(&G_light_timer, light_delay_timer_callback, &G_on);
	os_timer_arm(&G_light_timer, on ? config()->light_delay_power_on: config()->light_delay_power_off, 0);
}

bool light_is_enabled(void){
	return G_on;
}

void light_ini(void) {
	extern struct fx G_fx_flames_1;
	extern struct fx G_fx_simple_1;
	extern struct fx G_fx_simple_2;
	extern void ICACHE_FLASH_ATTR simple_set_RGB( void *prv, const struct RGB *rgb );

	os_timer_disarm(&G_light_timer);

#ifdef CONFIG_WS2812
	ws2812_init();
#endif // CONFIG_WS2812

	fx_ini();

#if 1
	G_fx_simple_1.fx.begin = sysCfg.fx_simple_1_start_no;
	G_fx_simple_1.fx.end = sysCfg.fx_simple_1_stop_no;
	simple_set_RGB( G_fx_simple_1.fx.prv, &config()->fx_simple_1_RGB );

	G_fx_simple_2.fx.begin = sysCfg.fx_simple_2_start_no;
	G_fx_simple_2.fx.end = sysCfg.fx_simple_2_stop_no;
	simple_set_RGB( G_fx_simple_2.fx.prv, &config()->fx_simple_2_RGB );

	G_fx_flames_1.fx.begin = sysCfg.fx_flames_1_start_no;
	G_fx_flames_1.fx.end = sysCfg.fx_flames_1_stop_no;

	if( sysCfg.fx_simple_1_enable )
		fx_register( &G_fx_simple_1 );
//	if( sysCfg.fx_simple_2_enable )
//		fx_register( &G_fx_simple_2 );
//	if( sysCfg.fx_flames_1_enable )
//		fx_register( &G_fx_flames_1 );
#endif

}
