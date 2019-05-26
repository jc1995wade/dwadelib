#ifndef __DW_TIME_H__
#define __DW_TIME_H__

typedef struct dw_time {
    int year;
    int mon;
    int day;
    int hour;
    int min;
    int sec;
}Dw_time_type;


int dw_get_local_time(Dw_time_type *time);



#endif
