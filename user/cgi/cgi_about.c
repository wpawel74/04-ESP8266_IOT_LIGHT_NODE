/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * Jeroen Domburg <jeroen@spritesmods.com> wrote this file. As long as you retain 
 * this notice you can do whatever you want with this stuff. If we meet some day, 
 * and you think this stuff is worth it, you can buy me a beer in return. 
 * ----------------------------------------------------------------------------
 */
#include <esp8266.h>
#include "cgi.h"

int ICACHE_FLASH_ATTR tplAbout(HttpdConnData *cd, char *token, void **arg) {
	char buff[128];
	if (token==NULL) return HTTPD_CGI_DONE;

	if (os_strcmp(token, "freeheap")==0) {
		os_sprintf(buff,"Free heap size:%d\n",system_get_free_heap_size());
	}

	tplText( buff, token, "fwver", FWVER);

	httpdSend(cd, buff, -1);
	return HTTPD_CGI_DONE;
}
