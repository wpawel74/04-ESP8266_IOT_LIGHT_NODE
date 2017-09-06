#include <esp8266.h>
#include "fx.h"

struct fx_pulsar {
	struct RGB	rgb;
	struct RGB	rgb_b;
	struct RGB	rgb_fuzzy, rgb_dizzy;
	uint32_t	delay;
	ETSTimer	timer;
	bool		active;
};

static void ICACHE_FLASH_ATTR pulsar_stop( void *prv ){
	struct fx_pulsar *st = (struct fx_pulsar *)prv;
	os_timer_disarm(&st->timer);
}

void ICACHE_FLASH_ATTR pulsar_set_RGB( void *prv, const struct RGB *rgb ){
	struct fx_pulsar *st = (struct fx_pulsar *)prv;
	st->rgb = *rgb;
}

static void ICACHE_FLASH_ATTR pulsar_timer_callback(void *arg){
	struct fx_pulsar *st = (struct fx_pulsar *)arg;
	if( st->rgb.r != st->rgb_b.r )
		st->rgb.r < st->rgb_b.r ? st->rgb.r++: st->rgb.r--;
	if( st->rgb.g != st->rgb_b.g )
		st->rgb.g < st->rgb_b.g ? st->rgb.g++: st->rgb.g--;
	if( st->rgb.b != st->rgb_b.b )
		st->rgb.b < st->rgb_b.b ? st->rgb.b++: st->rgb.b--;

	if(st->rgb_b.r == st->rgb_fuzzy.r && st->rgb_b.g == st->rgb_fuzzy.g && st->rgb_b.b == st->rgb_fuzzy.b)
		pulsar_set_RGB( st, &st->rgb_dizzy );
	if(st->rgb_b.r == st->rgb_dizzy.r && st->rgb_b.g == st->rgb_dizzy.g && st->rgb_b.b == st->rgb_dizzy.b)
		pulsar_set_RGB( st, &st->rgb_fuzzy );
}

void ICACHE_FLASH_ATTR pulsar_set_RGB_dizzy( void *prv, const struct RGB *rgb ){
	struct fx_pulsar *st = (struct fx_pulsar *)prv;
	st->rgb_dizzy = *rgb;
}

void ICACHE_FLASH_ATTR pulsar_set_RGB_fuzzy( void *prv, const struct RGB *rgb ){
	struct fx_pulsar *st = (struct fx_pulsar *)prv;
	st->rgb_fuzzy = *rgb;
}

void ICACHE_FLASH_ATTR pulsar_set_delay( void *prv, uint32_t delay ){
	struct fx_pulsar *st = (struct fx_pulsar *)prv;
	st->delay = delay;
	if( st->active ){
		os_timer_disarm(&st->timer);
		os_timer_arm(&st->timer, st->delay, 1 );
	}
}

static void ICACHE_FLASH_ATTR pulsar_algo( void *prv, char *dt, int size ) {
	struct fx_pulsar *st = (struct fx_pulsar *)prv;
	int it;

	// Step 1.  Map cells to LED colors
	for( it = 0; it < size; it++) {
		((struct RGB *)dt)[ it ] = st->rgb_b;
	}
}

static void ICACHE_FLASH_ATTR pulsar_start( void *prv ){
	struct fx_pulsar *st = (struct fx_pulsar *)prv;
	pulsar_set_RGB( st,  &st->rgb_dizzy );
	os_timer_disarm(&st->timer);
	os_timer_setfn(&st->timer, pulsar_timer_callback, st);
	os_timer_arm(&st->timer, st->delay, 1 );
}

struct fx_pulsar G_pulsar = {
	.rgb = { 0, 0, 0 },
	.rgb_fuzzy = { 255, 255, 255 },
	.rgb_dizzy = { 0, 0, 0 },
	.rgb_b = { 0, 0, 0 },
	.delay = 200,
	.active = false,
};

struct fx G_fx_pulsar_1 = {
	.prio = 0,
	.pattern = 0x02,
	.name = "pulsar",
	.fx = {
		.begin = 0,
		.end = 50,
		.start = pulsar_start,
		.stop = pulsar_stop,
		.algo = pulsar_algo,
		.prv = &G_pulsar

	},
	.next = NULL
};
