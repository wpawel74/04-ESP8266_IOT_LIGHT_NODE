#include "espmissingincludes.h"
#include <time.h>
#include <esp8266.h>
#include <user_config.h>
#include "config.h"
#include "alarm.h"

static ETSTimer G_alarm_timer, G_alarm_duration;

const char *days[] = { "Sun", "Mon", "Tue", "Wen", "Thu", "Fri", "Sat" };

static iot_alarm iot_alarms[6];

extern int ICACHE_FLASH_ATTR getdayofweek(int year, int month, int day);

void ICACHE_FLASH_ATTR alarm_show_date( char *buf, size_t size, const struct tm *tm ){
	//
	// better solution is os_snprintf but is not available in current SDK
	//
	os_sprintf( buf, "%d/%d/%d %s %02d:%02d:%02d", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
		days[ getdayofweek(tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday) ],
		tm->tm_hour, tm->tm_min, tm->tm_sec );
}


extern struct fx G_fx_pulsar_1;

static void ICACHE_FLASH_ATTR alarm_duration_callback(void *arg){
//	iot_alarm *ea = (iot_alarm *)arg;

#ifdef CONFIG_WS2812
	fx_deregister( &G_fx_pulsar_1 );
#endif //CONFIG_WS2812

}

int ICACHE_FLASH_ATTR alarm_clock( iot_alarm *ea ){

#ifdef CONFIG_WS2812
	fx_register( &G_fx_pulsar_1 );
#endif //CONFIG_WS2812

	os_timer_setfn(&G_alarm_duration, alarm_duration_callback, ea );
	os_timer_arm(&G_alarm_duration, config()->alarm_duration, 0);
	return 0;
}

int ICACHE_FLASH_ATTR alarm_check( iot_alarm *ea, const struct tm *tm ){
	if( ea->alarm->hour == tm->tm_hour && ea->alarm->min == tm->tm_min && ea->day != tm->tm_mday ){
		int day_of_week = getdayofweek(tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday);
		if( (ea->alarm->on_days >> day_of_week) & 0x01 ){
			ea->day = tm->tm_mday;
			alarm_clock( ea );
			return true;
		}
	}
	return false;
}

static void ICACHE_FLASH_ATTR bobo(const struct tm *tm, time_t tim){
	char buff[64];
	alarm_show_date( buff, sizeof(buff), tm );
	os_printf("BOBO %ld-> %s \n", tim, buff);
}

static void ICACHE_FLASH_ATTR alarm_timer_callback(void *arg){
	struct tm *tm;
	int it;
	time_t tim;

	tim = time( NULL );
	tm = gmtime( &tim );

	for( it = 0; it < sizeof(iot_alarms)/sizeof(iot_alarm); it++ )
		alarm_check( iot_alarms + it, tm );

	bobo( tm, tim );
}

void ICACHE_FLASH_ATTR alarm_set_pool(uint32_t milis){
	os_timer_disarm(&G_alarm_timer);
	if( milis > 0 ){
		os_timer_setfn(&G_alarm_timer, alarm_timer_callback, NULL );
		os_timer_arm(&G_alarm_timer, milis, 1);
	}
}

void ICACHE_FLASH_ATTR alarm_dismiss(void){
	os_timer_disarm(&G_alarm_duration);
	alarm_set_pool(2000);
}

void ICACHE_FLASH_ATTR alarm_ini(void) {
	int it;

	memset( iot_alarms, 0, sizeof(iot_alarms) );
	for( it = 0; it < sizeof(iot_alarms)/sizeof(iot_alarm) && it < sizeof(config()->alarms)/sizeof(alarm); it++ )
		(iot_alarms + it)->alarm = config()->alarms + it;
	alarm_set_pool(2000);
}

void ICACHE_FLASH_ATTR alarm_synchro(void){
}
