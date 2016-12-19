#include <esp8266.h>
#include "fx.h"

struct fx_smooth {
	struct RGB	rgb;
	struct RGB	rgb_b;
	uint32_t	delay;
	ETSTimer	timer;
	bool		active;
};

static void ICACHE_FLASH_ATTR smooth_timer_callback(void *arg){
	struct fx_smooth *smooth = (struct fx_smooth *)arg;
	if( smooth->rgb.r != smooth->rgb_b.r )
		smooth->rgb.r < smooth->rgb_b.r ? smooth->rgb.r++: smooth->rgb.r--;
	if( smooth->rgb.g != smooth->rgb_b.g )
		smooth->rgb.g < smooth->rgb_b.g ? smooth->rgb.g++: smooth->rgb.g--;
	if( smooth->rgb.b != smooth->rgb_b.b )
		smooth->rgb.b < smooth->rgb_b.b ? smooth->rgb.b++: smooth->rgb.b--;
}

void ICACHE_FLASH_ATTR smooth_set_RGB( void *prv, const struct RGB *rgb ){
	struct fx_smooth *smooth = (struct fx_smooth *)prv;
	smooth->rgb = *rgb;
}

void ICACHE_FLASH_ATTR smooth_set_delay( void *prv, uint32_t delay ){
	struct fx_smooth *smooth = (struct fx_smooth *)prv;
	smooth->delay = delay;
	if( smooth->active ){
		os_timer_disarm(&smooth->timer);
		os_timer_arm(&smooth->timer, smooth->delay, 1 );
	}
}

static void ICACHE_FLASH_ATTR smooth_algo( void *prv, char *dt, int size ) {
	struct fx_smooth *smooth = (struct fx_smooth *)prv;
	int it;

	// Step 1.  Map cells to LED colors
	for( it = 0; it < size; it++) {
		((struct RGB *)dt)[ it ] = smooth->rgb_b;
	}
}

static void ICACHE_FLASH_ATTR smooth_start( void *prv ){
	struct fx_smooth *smooth = (struct fx_smooth *)prv;
	os_timer_disarm(&smooth->timer);
	os_timer_setfn(&smooth->timer, smooth_timer_callback, smooth);
	os_timer_arm(&smooth->timer, smooth->delay, 1 );
}

static void ICACHE_FLASH_ATTR smooth_stop( void *prv ){
	struct fx_smooth *smooth = (struct fx_smooth *)prv;
	os_timer_disarm(&smooth->timer);
}

struct fx_smooth G_smooth = {
	.rgb = { 0, 0, 0 },
	.rgb_b = { 0, 0, 0 },
	.delay = 200,
	.active = false
};

struct fx G_fx_smooth_1 = {
	.prio = 0,
	.pattern = 0x02,
	.name = "smooth",
	.fx = {
		.begin = 0,
		.end = 50,
		.start = smooth_start,
		.stop = smooth_stop,
		.algo = smooth_algo,
		.prv = &G_smooth
	},
	.next = NULL
};
