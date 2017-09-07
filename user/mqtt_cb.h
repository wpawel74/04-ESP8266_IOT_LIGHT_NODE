#ifndef MQTT_CB_H
#define MQTT_CB_H
#include "mqtt.h"

extern MQTT_Client mqttClient;

void mqttConnectedCb(uint32_t *args);
void mqttDisconnectedCb(uint32_t *args);

void mqttDataCb(uint32_t *args, const char* topic, uint32_t topic_len, const char *data, uint32_t lengh);
void mqttPublishedCb(uint32_t *args);

#endif // MQTT_CB