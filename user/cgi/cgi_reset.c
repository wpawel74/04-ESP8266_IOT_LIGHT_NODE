/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */
#include <esp8266.h>
#include <stdlib.h>
#include <string.h>
#include <osapi.h>
#include "cgi.h"

void ICACHE_FLASH_ATTR restartTimerCb(void *arg) {
	os_printf("Restarting..\n");
	system_restart();
}

int ICACHE_FLASH_ATTR cgiReset(HttpdConnData *connData){
	static ETSTimer restartTimer;
	//Schedule restart
	os_timer_disarm(&restartTimer);
	os_timer_setfn(&restartTimer, restartTimerCb, NULL);
	os_timer_arm(&restartTimer, 2000, 0);
	httpdRedirect(connData, "restarting.html");
	return HTTPD_CGI_DONE;
}
