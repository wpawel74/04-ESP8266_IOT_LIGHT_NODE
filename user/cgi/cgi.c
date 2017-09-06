#include <esp8266.h>
#include <stdlib.h>
#include <string.h>
#include <osapi.h>
#include "cgi.h"
#include "fx.h"
#include "config.h"

void ICACHE_FLASH_ATTR tplInt( char *buff, const char *token, const char *name, int value ){
	if( os_strcmp(token, name) == 0 )
		os_sprintf(buff,"%d", value);
}

void ICACHE_FLASH_ATTR tplText( char *buff, const char *token, const char *name, const char *text ){
	if( os_strcmp(token, name) == 0 )
		os_strcpy(buff, (char *)text);
}

void ICACHE_FLASH_ATTR tplCheckBox( char *buff, const char *token, const char *name, bool checked ){
	if( os_strcmp(token, name) == 0 )
		os_strcpy(buff, checked ? "checked" : "" );
}

void ICACHE_FLASH_ATTR tplRGB( char *buff, const char *token, const RGB *rgb, const char *name ){
	char qtok[ 64 ];

	os_sprintf( qtok, "%s_rgb_r", name );
	if( os_strcmp(token, qtok) == 0 )
		os_sprintf(buff,"%d", (int)rgb->r);

	os_sprintf( qtok, "%s_rgb_g", name );
	if( os_strcmp(token, qtok) == 0 )
		os_sprintf(buff,"%d", (int)rgb->g);

	os_sprintf( qtok, "%s_rgb_b", name );
	if( os_strcmp(token, qtok) == 0 )
		os_sprintf(buff,"%d", (int)rgb->b);
}

int ICACHE_FLASH_ATTR cgiCheckBox(HttpdConnData *cd, const char *name, bool *dst) {
	char buff[64];
	*dst = (httpdFindArg(cd->post->buff, (char *)name, buff, sizeof(buff)) > 0) ? 1: 0;
	return 0;
}

int ICACHE_FLASH_ATTR cgiInt(HttpdConnData *cd, const char *name, int32_t *dst) {
	char buff[64];
	if( httpdFindArg(cd->post->buff, (char *)name, buff, sizeof(buff)) > 0 )
		*dst = atoi(buff);
	return 0;
}

int ICACHE_FLASH_ATTR cgiRGB(HttpdConnData *cd, const char *name, RGB *rgb) {
	char buff[64];
	char token[64];

	os_sprintf( token, "%s_rgb_r", name );
	if( httpdFindArg(cd->post->buff, token, buff, sizeof(buff)) > 0 )
		rgb->r = atoi(buff);

	os_sprintf( token, "%s_rgb_g", name );
	if( httpdFindArg(cd->post->buff, token, buff, sizeof(buff)) > 0 )
		rgb->g = atoi(buff);

	os_sprintf( token, "%s_rgb_b", name );
	if( httpdFindArg(cd->post->buff, token, buff, sizeof(buff)) > 0 )
		rgb->b = atoi(buff);
	return 0;
}

int ICACHE_FLASH_ATTR cgiText(HttpdConnData *cd, const char *name, char *dst, int size){
	char buff[64];
	if( httpdFindArg(cd->post->buff, (char *)name, buff, sizeof(buff)) > 0 ){
		if( strlen(buff) < size )
			os_strcpy(dst, buff);
		else
			return -1;
	}
	return 0;
}
