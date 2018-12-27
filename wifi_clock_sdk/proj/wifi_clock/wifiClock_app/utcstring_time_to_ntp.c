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

#define xMINUTE   (60             ) //1�ֵ�����
#define xHOUR      (60*xMINUTE) //1Сʱ������
#define xDAY        (24*xHOUR   ) //1�������
#define xYEAR       (365*xDAY   ) //1�������
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
    /*01��*/xDAY*(0),
    /*02��*/xDAY*(31),
    /*03��*/xDAY*(31+28),
    /*04��*/xDAY*(31+28+31),
    /*05��*/xDAY*(31+28+31+30),
    /*06��*/xDAY*(31+28+31+30+31),
    /*07��*/xDAY*(31+28+31+30+31+30),
    /*08��*/xDAY*(31+28+31+30+31+30+31),
    /*09��*/xDAY*(31+28+31+30+31+30+31+31),
    /*10��*/xDAY*(31+28+31+30+31+30+31+31+30),
    /*11��*/xDAY*(31+28+31+30+31+30+31+31+30+31),
    /*12��*/xDAY*(31+28+31+30+31+30+31+31+30+31+30)
  };
  unsigned int  seconds = 0;
  unsigned int  year = 0;
  year = time->year-1970;       //������2100��ǧ�������
  seconds = xYEAR*year + xDAY*((year+1)/4);  //ǰ�����ȥ������
  seconds += month[time->month-1];      //���Ͻ��걾�¹�ȥ������
  if( (time->month > 2) && (((year+2)%4)==0) )//2008��Ϊ����
    seconds += xDAY;            //�����1������
  seconds += xDAY*(time->day-1);         //���ϱ����ȥ������
  seconds += xHOUR*time->hour;           //���ϱ�Сʱ��ȥ������
  seconds += xMINUTE*time->minute;       //���ϱ����ӹ�ȥ������
  seconds += time->second;               //���ϵ�ǰ����<br>��seconds -= 8 * xHOUR;
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


