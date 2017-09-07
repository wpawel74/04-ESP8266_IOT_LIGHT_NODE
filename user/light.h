#ifndef LIGHT_H
#define LIGHT_H

void light_ini(void);
bool light_is_enabled(void);
void light_enable( bool on );
void light_fx_reload(void);

typedef enum {
	SIMPLE_OFF = 0,
	SIMPLE_ON  = 1,
	USE_FX = 2
} light_style;

void light_set_style( light_style style );

#endif // LIGHT_H