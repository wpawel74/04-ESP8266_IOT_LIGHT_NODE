#include "esp8266.h"
#include "sys/time.h"
#include "time.h"


struct timeb {
	long	time;
	unsigned short millitm;
	short	timezone;
	short	dstflag;
};

int ICACHE_FLASH_ATTR getdayofweek(int year, int month, int day){
	int week;

	if (month > 2)
		month -= 2;
	else {
		month += 10;
		year--;
	}
	week = year/100;
	year %= 100;
	return (day + (13 * month - 1)/5 + year + year/4 + week/4 + 5 * week) % 7;
}

int ICACHE_FLASH_ATTR gettimeofday( struct timeval *tv, struct timezone *tz){
	tv->tv_sec = time( NULL );
	tv->tv_usec = (system_get_time() % 1000000);

//	OS_getTimeOfDay( tv );
	tz->tz_dsttime = 0;
	tz->tz_minuteswest = 0;
	return 0;
}

void ICACHE_FLASH_ATTR _ftime(struct timeb *bp){
	bp->time = time( NULL );
	bp->millitm = (system_get_time() % 1000000);
	bp->timezone = 0;
	bp->dstflag = 0;
}

