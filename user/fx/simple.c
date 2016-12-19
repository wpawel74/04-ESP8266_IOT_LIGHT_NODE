#include <esp8266.h>
#include "fx.h"

struct fx_simple {
	struct RGB	rgb;
};

void ICACHE_FLASH_ATTR simple_set_RGB( void *prv, const struct RGB *rgb ){
	struct fx_simple *simple = (struct fx_simple *)prv;
	simple->rgb = *rgb;
}

static void ICACHE_FLASH_ATTR simple_algo( void *prv, char *dt, int size ) {
	struct fx_simple *simple = (struct fx_simple *)prv;
	int it;

	// Step 1.  Map cells to LED colors
	for( it = 0; it < size; it++) {
		((struct RGB *)dt)[ it ] = simple->rgb;
	}
}

static void ICACHE_FLASH_ATTR simple_start( void *prv ){
}

static void ICACHE_FLASH_ATTR simple_stop( void *prv ){
}

struct fx_simple G_simple_1 = {
	.rgb = { 0, 0, 0 }
};

struct fx G_fx_simple_1 = {
	.prio = 0,
	.pattern = 0x02,
	.name = "simple",
	.fx = {
		.begin = 0,
		.end = 50,
		.start = simple_start,
		.stop = simple_stop,
		.algo = simple_algo,
		.prv = &G_simple_1
	},
	.next = NULL
};

struct fx_simple G_simple_2 = {
	.rgb = { 0, 0, 0 }
};

struct fx G_fx_simple_2 = {
	.prio = 0,
	.pattern = 0x02,
	.name = "simple",
	.fx = {
		.begin = 0,
		.end = 50,
		.start = simple_start,
		.stop = simple_stop,
		.algo = simple_algo,
		.prv = &G_simple_2
	},
	.next = NULL
};
