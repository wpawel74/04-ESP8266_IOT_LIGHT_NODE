#ifndef IO_H
#define IO_H

#define GPIO_RELAY1			4
#define GPIO_RELAY2			5

void ICACHE_FLASH_ATTR io_GPIOSet(int ena,int gpio);
bool ICACHE_FLASH_ATTR io_GPIOGet(int gpio);

void ICACHE_FLASH_ATTR ioLed(int ena);

void io_GPIOIni(void);

#endif // IO_H