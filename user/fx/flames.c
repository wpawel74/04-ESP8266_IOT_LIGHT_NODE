#include <esp8266.h>
#include "random8.h"
#include "math8.h"
#include "scale8.h"
#include "fx.h"

struct fx_flames {
	uint32_t	cooling;		// 55
	uint32_t	sparking;		// 120
	uint8_t		dir;
	uint8_t		leds_chain;
	uint8_t		*heat;
};

static struct RGB ICACHE_FLASH_ATTR flames_get_heat_color( uint8_t temp ){
	struct RGB rgb;
	// Scale 'heat' down from 0-255 to 0-191,
	// which can then be easily divided into three
	// equal 'thirds' of 64 units each.
	uint8_t t192 = scale8_video( temp, 192);

	// calculate a value that ramps up from
	// zero to 255 in each 'third' of the scale.
	uint8_t heatramp = t192 & 0x3F; // 0..63
	heatramp <<= 2; // scale up to 0..252

	// now figure out which third of the spectrum we're in:
	if( t192 & 0x80) {
		// we're in the hottest third
		rgb.r = 255; // full red
		rgb.g = 255; // full green
		rgb.b = heatramp; // ramp up blue
	} else if( t192 & 0x40 ) {
		// we're in the middle third
		rgb.r = 255; // full red
		rgb.g = heatramp; // ramp up green
		rgb.b = 0; // no blue
	} else {
		// we're in the coolest third
		rgb.r = heatramp; // ramp up red
		rgb.g = 0; // no green
		rgb.b = 0; // no blue
	}
	return rgb;
}

static void ICACHE_FLASH_ATTR flames_algo( void *prv, char *dt, int size ) {
	struct fx_flames *flames = (struct fx_flames *)prv;
	int it = 0;
	// Step 1.  Cool down every cell a little
	for( it = 0; it < flames->leds_chain; it++ ){
		flames->heat[ it ] = qsub8( flames->heat[ it ],  random8_2p(0, ((flames->cooling * 10) / flames->leds_chain) + 2));
	}
	// Step 2.  Heat from each cell drifts 'up' and diffuses a little
	for( it = flames->leds_chain - 1; it >= 2; it--) {
		flames->heat[it] = (flames->heat[it - 1] + flames->heat[it - 2] + flames->heat[it - 2] ) / 3;
	}
	// Step 3.  Randomly ignite new 'sparks' of heat near the bottom
	if( random8() < flames->sparking ) {
		it = random8_1p(7);
		flames->heat[it] = qadd8( flames->heat[it], random8_2p(160,255) );
	}
	// Step 4.  Map from heat cells to LED colors
	for( it = 0; it < flames->leds_chain; it++) {
		struct RGB color = flames_get_heat_color( flames->heat[it] );
		int pixelnumber;
		if( flames->dir ) {
			pixelnumber = (flames->leds_chain - 1) - it;
		} else {
			pixelnumber = it;
		}
		if( pixelnumber < size )
			((struct RGB *)dt)[ pixelnumber ] = color;
	}
}

static void ICACHE_FLASH_ATTR flames_start( void *prv ){
	struct fx_flames *flames = (struct fx_flames *)prv;
	if( !flames->heat ) {
		flames->heat = os_malloc( flames->leds_chain );
		memset( flames->heat, 0, flames->leds_chain );
	}
}

static void ICACHE_FLASH_ATTR flames_stop( void *prv ){
	struct fx_flames *flames = (struct fx_flames *)prv;
	if( flames->heat )
		os_free( flames->heat );
	flames->heat = NULL;
}

struct fx_flames G_flames_1 = {
	.cooling = 55,
	.sparking = 120,
	.dir = 0,
	.leds_chain = 60,
	.heat = NULL
};

struct fx G_fx_flames_1 = {
	.prio = 0,
	.pattern = 0x01,
	.name = "flames",
	.fx = {
		.begin = 0,
		.end = 50,
		.start = flames_start,
		.stop = flames_stop,
		.algo = flames_algo,
		.prv = &G_flames_1
	},
	.next = NULL
};
