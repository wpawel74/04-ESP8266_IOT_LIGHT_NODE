#ifndef LIGHT_H
#define LIGHT_H

void ICACHE_FLASH_ATTR light_ini(void);
bool ICACHE_FLASH_ATTR light_is_enabled(void);
void ICACHE_FLASH_ATTR light_enable( bool on );
void ICACHE_FLASH_ATTR light_fx_reload(void);

#endif // LIGHT_H