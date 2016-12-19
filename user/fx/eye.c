#include <esp8266.h>
#include "fx.h"

struct fx_eye {
	struct RGB	rgb;
	uint32_t	head;
	uint32_t	tail;
	int32_t		pos;
	bool		dir;
	uint32_t	delay;
	ETSTimer	timer;
	bool		active;
};

static void ICACHE_FLASH_ATTR eye_timer_callback(void *arg){
	struct fx_eye *eye = (struct fx_eye *)arg;
	if( eye->dir )
		eye->pos++;
	else
		eye->pos--;
}

void ICACHE_FLASH_ATTR eye_set_RGB( void *prv, const struct RGB *rgb ){
	struct fx_eye *eye = (struct fx_eye *)prv;
	eye->rgb = *rgb;
}

void ICACHE_FLASH_ATTR eye_set_delay( void *prv, uint32_t delay ){
	struct fx_eye *eye = (struct fx_eye *)prv;
	eye->delay = delay;
	if( eye->active ){
		os_timer_disarm(&eye->timer);
		os_timer_arm(&eye->timer, eye->delay, 1 );
	}
}

static void ICACHE_FLASH_ATTR eye_left( void *prv, char *dt, int size ) {
	struct fx_eye *eye = (struct fx_eye *)prv;
	int it;
	for( it = 0; it < size; it++) {
		struct RGB rgb = { 0, 0, 0 };
		rgb.r = (eye->rgb.r / ( size < eye->rgb.r ? size: eye->rgb.r )) * it;
		rgb.g = (eye->rgb.g / ( size < eye->rgb.g ? size: eye->rgb.g )) * it;
		rgb.b = (eye->rgb.b / ( size < eye->rgb.b ? size: eye->rgb.b )) * it;
		((struct RGB *)dt)[ it ] = rgb;
	}
}

static void ICACHE_FLASH_ATTR eye_right( void *prv, char *dt, int size ) {
	struct fx_eye *eye = (struct fx_eye *)prv;
	int it;
	for( it = 0; it < size; it++) {
		struct RGB rgb = { 0, 0, 0 };
		rgb.r = eye->rgb.r - (eye->rgb.r / ( size < eye->rgb.r ? size: eye->rgb.r )) * it;
		rgb.g = eye->rgb.g - (eye->rgb.g / ( size < eye->rgb.g ? size: eye->rgb.g )) * it;
		rgb.b = eye->rgb.b - (eye->rgb.b / ( size < eye->rgb.b ? size: eye->rgb.b )) * it;
		((struct RGB *)dt)[ it ] = rgb;
	}
}

static void ICACHE_FLASH_ATTR eye_algo( void *prv, char *dt, int size ) {
	struct fx_eye *eye = (struct fx_eye *)prv;
	int it;

	// Step 1.  Map cells to LED colors
	for( it = 0; it < size; it++) {
		if( it < (eye->pos - (eye->dir ? eye->tail: eye->head)) && it > (eye->pos + (eye->dir ? eye->tail: eye->head)) ){
			struct RGB none = { .r = 0, .b = 0, .g = 0 };
			((struct RGB *)dt)[ it ] = none;
		} else {
//			if( eye->dir )
//				eye_left( prv, dt, it + size );
		}
	}
}

static void ICACHE_FLASH_ATTR eye_start( void *prv ){
	struct fx_eye *eye = (struct fx_eye *)prv;
	os_timer_disarm(&eye->timer);
	os_timer_setfn(&eye->timer, eye_timer_callback, eye);
	os_timer_arm(&eye->timer, eye->delay, 1 );
}

static void ICACHE_FLASH_ATTR eye_stop( void *prv ){
	struct fx_eye *eye = (struct fx_eye *)prv;
	os_timer_disarm(&eye->timer);
}

struct fx_eye G_eye = {
	.rgb = { 0, 0, 0 },
	.delay = 200,
	.active = false
};

struct fx G_fx_eye_1 = {
	.prio = 0,
	.pattern = 0x02,
	.name = "eye",
	.fx = {
		.begin = 0,
		.end = 50,
		.start = eye_start,
		.stop = eye_stop,
		.algo = eye_algo,
		.prv = &G_eye
	},
	.next = NULL
};
