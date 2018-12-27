/**
 * 
 * 
 * 
 * 
 * 
 * 
 */
#include "wifi_clock_cfg.h"
#include "string.h"

#define xMINUTE   (60             ) //1分的秒数
#define xHOUR      (60*xMINUTE) //1小时的秒数
#define xDAY        (24*xHOUR   ) //1天的秒数
#define xYEAR       (365*xDAY   ) //1年的秒数
typedef struct {
  int year; int month;  int day;
  int hour; int minute;  int second;
} ntp_time_t ;

ntp_time_t UTC_sting_to_time(const char* Tm)
{
    char utcTime[30] = {0,};
    ntp_time_t time;
    memset(&time,0,sizeof(ntp_time_t));
    char* position = NULL;
    position = strstr(Tm,"-");
    if (position != NULL) {
        strcpy(utcTime,Tm);
        time.year = atoi(utcTime);
        strcpy(utcTime,(char* )(position + 1));
        time.month = atoi(utcTime);
    }
    position = strstr(Tm + 5,"-");
    if (position != NULL) {
        strcpy(utcTime,(char* )(position + 1));
        time.day = atoi(utcTime);
    }
    position = strstr(Tm + 10,"T");
    if (position != NULL) {
        strcpy(utcTime,(char* )(position + 1));
        time.hour = atoi(utcTime);
    }
    position = strstr(Tm + 10,":");
    if (position != NULL) {
        strcpy(utcTime,(char* )(position + 1));
        time.minute = atoi(utcTime);
    }
    position = strstr(Tm + 16,":");
    if (position != NULL) {
        strcpy(utcTime,(char* )(position + 1));
        time.second = atoi(utcTime);
    }
    printf("%d %d %d %d %d %d",
               time.year,
               time.month,
               time.day,
               time.hour,
               time.minute,
               time.second
           );
    return time;
}

unsigned int UTC_to_seconds(ntp_time_t* time)
{
  static unsigned int  month[12]={
    /*01月*/xDAY*(0),
    /*02月*/xDAY*(31),
    /*03月*/xDAY*(31+28),
    /*04月*/xDAY*(31+28+31),
    /*05月*/xDAY*(31+28+31+30),
    /*06月*/xDAY*(31+28+31+30+31),
    /*07月*/xDAY*(31+28+31+30+31+30),
    /*08月*/xDAY*(31+28+31+30+31+30+31),
    /*09月*/xDAY*(31+28+31+30+31+30+31+31),
    /*10月*/xDAY*(31+28+31+30+31+30+31+31+30),
    /*11月*/xDAY*(31+28+31+30+31+30+31+31+30+31),
    /*12月*/xDAY*(31+28+31+30+31+30+31+31+30+31+30)
  };
  unsigned int  seconds = 0;
  unsigned int  year = 0;
  year = time->year-1970;       //不考虑2100年千年虫问题
  seconds = xYEAR*year + xDAY*((year+1)/4);  //前几年过去的秒数
  seconds += month[time->month-1];      //加上今年本月过去的秒数
  if( (time->month > 2) && (((year+2)%4)==0) )//2008年为闰年
    seconds += xDAY;            //闰年加1天秒数
  seconds += xDAY*(time->day-1);         //加上本天过去的秒数
  seconds += xHOUR*time->hour;           //加上本小时过去的秒数
  seconds += xMINUTE*time->minute;       //加上本分钟过去的秒数
  seconds += time->second;               //加上当前秒数<br>　seconds -= 8 * xHOUR;
  return seconds;
}

unsigned int get_uinx_timestamp(const char* time)
{
    ntp_time_t tm = UTC_sting_to_time(time);
    if (tm.year != 0) {
        return UTC_to_seconds(&tm) + 8 * 60 * 60;
    }
    return 0;
}


