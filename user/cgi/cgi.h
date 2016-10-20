#ifndef CGI_H
#define CGI_H

#include "httpd.h"

int cgiLed(HttpdConnData *connData);
int tplLed(HttpdConnData *connData, char *token, void **arg);

int tplCounter(HttpdConnData *connData, char *token, void **arg);
int tplAbout(HttpdConnData *connData, char *token, void **arg);

int cgiGPIO(HttpdConnData *connData);
void tplGPIO(HttpdConnData *connData, char *token, void **arg);

void tplDHT(HttpdConnData *connData, char *token, void **arg);
int cgiDHT22(HttpdConnData *connData);

void tplDS18b20(HttpdConnData *connData, char *token, void **arg);
int cgiDS18b20(HttpdConnData *connData);

int cgiState(HttpdConnData *connData);

int cgiUI(HttpdConnData *connData);
void tplUI(HttpdConnData *connData, char *token, void **arg);

void tplMQTT(HttpdConnData *connData, char *token, void **arg);
int cgiMQTT(HttpdConnData *connData);

void tplHTTPD(HttpdConnData *connData, char *token, void **arg);
int cgiHTTPD(HttpdConnData *connData);

void tplBroadcastD(HttpdConnData *connData, char *token, void **arg);
int cgiBroadcastD(HttpdConnData *connData);

void tplNTP(HttpdConnData *connData, char *token, void **arg);
int cgiNTP(HttpdConnData *connData);

void tplRLYSettings(HttpdConnData *connData, char *token, void **arg);
int cgiRLYSettings(HttpdConnData *connData);

int cgiReset(HttpdConnData *connData);

void tplSensorSettings(HttpdConnData *connData, char *token, void **arg);
int cgiSensorSettings(HttpdConnData *connData);

void tplBMP180(HttpdConnData *connData, char *token, void **arg);
int cgiBMP180(HttpdConnData *connData);

void tplRTC(HttpdConnData *connData, char *token, void **arg);
int cgiRTC(HttpdConnData *connData);

int cgiLightSettings(HttpdConnData *connData);
void tplLightSettings(HttpdConnData *connData, char *token, void **arg);

int cgiLight(HttpdConnData *connData);
void tplLight(HttpdConnData *connData, char *token, void **arg);

#endif // CGI_H