#include <stdio.h>
#include <string.h>
#include <time.h>
#include "dw_time.h"

#define _DEBUG_ 0

#if _DEBUG_
#define DWBUG(...) printf(__VA_ARGS__)
#else
#define DWBUG(...)
#endif

int dw_get_local_time(Dw_time_type *dwtime)
{
    time_t timep;   
    struct tm *p; 
   
    time(&timep);   
    p = localtime(&timep);
   
    DWBUG("%d-%d-%d %d:%d:%d\n", (1900 + p->tm_year), ( 1 + p->tm_mon), p->tm_mday,
                                (p->tm_hour + 12), p->tm_min, p->tm_sec); 

    if (dwtime != NULL) {
        dwtime->year = 1900 + p->tm_year;
	    dwtime->mon  = 1 + p->tm_mon;
        dwtime->day  = p->tm_mday;
	    dwtime->hour = p->tm_hour + 12;
	    dwtime->min  = p->tm_min;
	    dwtime->sec  = p->tm_sec;
    }
 
    return 0;
}
