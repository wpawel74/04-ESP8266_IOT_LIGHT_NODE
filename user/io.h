#ifndef IO_H
#define IO_H

#define RELAY1_GPIO			4
#define RELAY2_GPIO			5

void ICACHE_FLASH_ATTR ioGPIO(int ena,int gpio);
void ICACHE_FLASH_ATTR ioLed(int ena);
void ioInit(void);

extern char currREL1State;
extern char currREL2State;

#endif // IO_H