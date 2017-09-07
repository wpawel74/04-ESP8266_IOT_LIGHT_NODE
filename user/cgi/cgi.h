#ifndef CGI_H
#define CGI_H

#include "httpd.h"
#include "fx.h"

void tplInt( char *buff, const char *token, const char *name, int value );
void tplText( char *buff, const char *token, const char *name, const char *text );
void tplCheckBox( char *buff, const char *token, const char *name, bool checked );
void tplRGB( char *buff, const char *token, const RGB *rgb, const char *name );

int cgiCheckBox(HttpdConnData *cd, const char *name, bool *dst);
int cgiInt(HttpdConnData *cd, const char *name, int32_t *dst);
int cgiRGB(HttpdConnData *cd, const char *name, RGB *rgb);
int cgiText(HttpdConnData *cd, const char *name, char *dst, int size);

int cgiLed(HttpdConnData *cd);
int tplLed(HttpdConnData *cd, char *token, void **arg);

int tplCounter(HttpdConnData *cd, char *token, void **arg);
int tplAbout(HttpdConnData *cd, char *token, void **arg);

int cgiGPIO(HttpdConnData *cd);
void tplGPIO(HttpdConnData *cd, char *token, void **arg);

void tplDHT(HttpdConnData *cd, char *token, void **arg);
int cgiDHT22(HttpdConnData *cd);

void tplDS18b20(HttpdConnData *cd, char *token, void **arg);
int cgiDS18b20(HttpdConnData *cd);

int cgiState(HttpdConnData *cd);

int cgiUI(HttpdConnData *cd);
void tplUI(HttpdConnData *cd, char *token, void **arg);

void tplMQTT(HttpdConnData *cd, char *token, void **arg);
int cgiMQTT(HttpdConnData *cd);

void tplHTTPD(HttpdConnData *cd, char *token, void **arg);
int cgiHTTPD(HttpdConnData *cd);

void tplBroadcastD(HttpdConnData *cd, char *token, void **arg);
int cgiBroadcastD(HttpdConnData *cd);

void tplNTP(HttpdConnData *cd, char *token, void **arg);
int cgiNTP(HttpdConnData *cd);

void tplRLYSettings(HttpdConnData *cd, char *token, void **arg);
int cgiRLYSettings(HttpdConnData *cd);

int cgiReset(HttpdConnData *cd);

void tplSensorSettings(HttpdConnData *cd, char *token, void **arg);
int cgiSensorSettings(HttpdConnData *cd);

void tplBMP180(HttpdConnData *cd, char *token, void **arg);
int cgiBMP180(HttpdConnData *cd);

void tplRTC(HttpdConnData *cd, char *token, void **arg);
int cgiRTC(HttpdConnData *cd);

int cgiLightSettings(HttpdConnData *cd);
void tplLightSettings(HttpdConnData *cd, char *token, void **arg);

int cgiLight(HttpdConnData *cd);
void tplLight(HttpdConnData *cd, char *token, void **arg);

int cgiAlarmSettings(HttpdConnData *cd);
void tplAlarmSettings(HttpdConnData *cd, char *token, void **arg);

#endif // CGI_H
