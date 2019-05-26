#include <stdio.h>
#include "dw_time.h"

int main(void)
{
    Dw_time_type tm;
    
    dw_get_local_time(&tm);
   
    printf("dw time:%d-%d-%d %d:%d:%d\n",tm.year, tm.mon, tm.day, tm.hour, tm.min, tm.sec);
    return 0;
}
