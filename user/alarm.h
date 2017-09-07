#ifndef ALARM_H
#define ALARM_H
#include "c_types.h"

typedef struct alarm {
	uint8_t hour;
	uint8_t min;
	uint8_t on_days;
} alarm;

typedef struct iot_alarm {
	alarm *alarm;
	uint8_t day;
} iot_alarm;

void alarm_ini(void);

void alarm_synchro(void);

#endif // ALARM_H