#include "esp8266.h"
#include "time.h"
#include "user_interface.h"
#include "sntp.h"

static time_t offset = 0;		// in seconds from epoch

time_t ICACHE_FLASH_ATTR time( time_t *t ){
	time_t res = (system_get_time()/1000000) + offset;
	return res;
}

int ICACHE_FLASH_ATTR stime(time_t *t){
	offset = (*t) - ((time_t)(system_get_time()/1000000));
	return 0;
}

/**
  * tricky: function to convert time structure to tm is already written in lwip stack. We need here only method declaration.
  */
//struct tm * ICACHE_FLASH_ATTR sntp_mktm_r(const time_t * tim_p ,struct tm *res ,int is_gmtime);

//struct tm * gmtime( const time_t *t ){
//	static struct tm tm;
//	return sntp_mktm_r( t, &tm, 0 );
//}