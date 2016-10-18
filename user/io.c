
/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */


#include <esp8266.h>
#include "gpio.h"
#include "config.h"
#include "io.h"

#define LEDGPIO 2
#define BTNGPIO 0

static ETSTimer resetBtntimer;
char currREL1State = 0;
char currREL2State = 0;

void ICACHE_FLASH_ATTR ioGPIO(int ena, int gpio) {
	//gpio_output_set is overkill. ToDo: use better macros
	if (ena) {
		gpio_output_set((1<<gpio), 0, (1<<gpio), 0);
	} else {
		gpio_output_set(0, (1<<gpio), (1<<gpio), 0);
	}
}

void ICACHE_FLASH_ATTR ioLed(int ena) {
	//gpio_output_set is overkill. ToDo: use better mactos
	if (ena) {
		gpio_output_set((1<<LEDGPIO), 0, (1<<LEDGPIO), 0);
	} else {
		gpio_output_set(0, (1<<LEDGPIO), (1<<LEDGPIO), 0);
	}
}

static void ICACHE_FLASH_ATTR resetBtnTimerCb(void *arg) {
	static int resetCnt=0;
	if (!GPIO_INPUT_GET(BTNGPIO)) {
		resetCnt++;
	} else {
		if (resetCnt>=6) { //3 sec pressed
			wifi_station_disconnect();
			wifi_set_opmode(0x3); //reset to AP+STA mode
			os_printf("Reset to AP mode. Restarting system...\n");
			system_restart();
		}
		resetCnt=0;
	}
}

void ioInit() {
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO5_U, FUNC_GPIO5);
//	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDO_U, FUNC_GPIO15);

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);
	gpio_output_set(0, 0, (1<<LEDGPIO), (1<<BTNGPIO));

	if( sysCfg.relay_latching_enable) {
		os_printf("Relay latching is %d, Relay1=%d,Relay2=%d\n\r",
				(int)sysCfg.relay_latching_enable,
				(int)sysCfg.relay_1_state,
				(int)sysCfg.relay_2_state);
		currREL1State=(int)sysCfg.relay_1_state;
		currREL2State=(int)sysCfg.relay_2_state;

		ioGPIO((int)sysCfg.relay_1_state,RELAY1_GPIO);
		ioGPIO((int)sysCfg.relay_2_state,RELAY2_GPIO);
	} else {
		ioGPIO(0,RELAY1_GPIO);
		ioGPIO(0,RELAY2_GPIO);
	}

	os_timer_disarm(&resetBtntimer);
	os_timer_setfn(&resetBtntimer, resetBtnTimerCb, NULL);
	os_timer_arm(&resetBtntimer, 500, 1);
}

