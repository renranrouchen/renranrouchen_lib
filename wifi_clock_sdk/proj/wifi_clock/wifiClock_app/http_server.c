/*-------------------------------------------------------------------------
*@Author		Zwxf
*@Email			Zwxf.cw@gmail.com
*@Brief			
*@FileName		http_server.c
*@Data			2017/8/22 9:44:48
-------------------------------------------------------------------------*/
#include <common.h>

#if defined(CONFIG_FREERTOS)
#include <FreeRTOS.h>
#include <task.h>
#endif
#include <httpd.h>
#include <wifi_clock_cfg.h>
#include <cfg_api.h>

extern sdk_param sdkParam;
extern sdk_param g_defparam;
#define IS_EQUAL(str, buff) (strcmp(str, buff) == 0)
#if WEB_CONFIG
#if (VERSION_MUSHAN || VERSION_DEMO || CUSTOMER_VERSION)
#define CITY "city"
#define SSID "SSID"
#define PSWD "PSWD"
#define DOWNTOWN "downtown"
#define BRIGHTNESS "brightness"
#define TIMEFMT "timeFmt"
#define NOWTIME "nowtime"
#define TEMPFMT "tempFmt"
#define ALARM1 "alarm1"
#define ALARMTIME1 "alarmTime1"
#define SLEEP1 "sleep1"
#define ALARM2 "alarm2"
#define ALARMTIME2 "alarmTime2"
#define SLEEP2 "sleep2"
#define ALARM3 "alarm3"
#define ALARMTIME3 "alarmTime3"
#define SLEEP3 "sleep3"
#define REPEAT1 "r1"
#define REPEAT2 "r2"
#define REPEAT3 "r3"
#define ACID "acid"
#define DEVICEID "deviceid"

typedef struct
{
    char apName[100];
    char password[100];
    char cityName[50];
    char downtown[50];
    unsigned int cityId;
    char nowtime;
    char timeFmt;
    char tempFmt;
    char brightness;
#if !VERSION_MUSHAN
    alarm_param alarm[21];
#endif
#if VERSION_MUSHAN
    t_v2_alarm_param v2_alarm_param;
    char acid[30];
    char deviceId[30];
#endif
} tParam;

char *
parse_data(const char *post, const char *str)
{
    char *addr = NULL, *data = NULL;
    static char postData[100] = {
        0,
    };
    data = postData;
    addr = (char *)strstr(str, post);
    if (addr == NULL)
    {
        return NULL;
    }
    memset(postData, 0, sizeof(postData));
    addr += (strlen(post) + 1);
    while (*addr != '&' & *addr != '\0')
    {
        memcpy(data++, addr++, 1);
    }
    return postData;
}
#if !VERSION_MUSHAN
void alarm_param_process(const char *alarm, const char *alarmTime,
                         const char *sleep, const char *repeat, alarm_param *alarmIndex, const char *str)
{
    const char *getSleep = NULL;
    const char *getRepeat = NULL;
    const char *getAlarm = NULL;
    unsigned char loop = 0;
    unsigned char dayIndex = 0;
    unsigned int getTime = 0;
    getAlarm = parse_data(alarm, str);
    getRepeat = parse_data(repeat, str);
    if (getAlarm != NULL && getRepeat != NULL)
    {
        dayIndex = atoi(getRepeat);
        getTime = atoi(parse_data(alarmTime, str));
        getSleep = parse_data(sleep, str);
        for (loop = 0; loop < 7; loop++)
        {
            alarmIndex[loop * 3].isOpen = false;
            alarmIndex[loop * 3].isOpenSnooze = false;
            if (getSleep != NULL)
            {
                alarmIndex[loop * 3].isOpenSnooze = true;
            }
            alarmIndex[loop * 3].alarmTime = (getTime);
            alarmIndex[loop * 3].defaultAlarmTime = (getTime);
            if (dayIndex & (1 << loop))
            {
                alarmIndex[loop * 3].isOpen = true;
            }
        }
    }
    else
    {
        for (loop = 0; loop < 7; loop++)
        {
            alarmIndex[loop * 3].isOpen = false;
        }
    }
}
#endif
/* Function: urlDecode */
char *urlDecode(char *str)
{
    int d = 0; /* whether or not the string is decoded */

    static char dStr[100];
    memset(dStr, 0, 100);
    char eStr[] = "00"; /* for a hex code */

    strcpy(dStr, str);

    while (!d)
    {
        d = 1;
        int i; /* the counter for the string */

        for (i = 0; i < strlen(dStr); ++i)
        {
            if (dStr[i] == '%')
            {
                if (dStr[i + 1] == 0)
                    return dStr;

                if (isxdigit(dStr[i + 1]) && isxdigit(dStr[i + 2]))
                {

                    d = 0;

                    /* combine the next to numbers into one */
                    eStr[0] = dStr[i + 1];
                    eStr[1] = dStr[i + 2];

                    /* convert it to decimal */
                    long int x = strtol(eStr, NULL, 16);

                    /* remove the hex */
                    memmove(&dStr[i + 1], &dStr[i + 3], strlen(&dStr[i + 3]) + 1);

                    dStr[i] = x;
                }
            } 
        }
    }

    return dStr;
}

static char* process_complicate_word(char* getData)
{
    // char* decodeData = NULL;
    unsigned char loop = 0;
    while (loop < strlen(getData))
    {
        // FIXED ' ' reverse to '+'
        if (getData[loop] == '+') 
        {
            getData[loop] = ' ';
        } 
        ++loop;
    }
    getData = urlDecode(getData);
    // printf("-------> decode %s",getData);

    return getData;
}

void fillin_param(const char *str, tParam *param)
{
    unsigned int loop = 0;
    char *getData = NULL;
    memset(param, 0, sizeof(tParam));
    getData = parse_data(SSID, str);
    getData = process_complicate_word(getData);
    strcpy(param->apName, getData);
    
    getData = parse_data(PSWD, str);
    getData = process_complicate_word(getData);
    strcpy(param->password, getData);

    getData = parse_data(CITY, str);
    strcpy(param->cityName, urlDecode(getData));
    extern const char *cityTable[2475];
    extern const unsigned int cityIdTable[2475];

    getData = parse_data(DOWNTOWN, str);
    strcpy(param->downtown, urlDecode(getData));

    param->cityId = cityIdTable[0];
    for (loop = 0; loop < 2475; loop++)
    {
        if (strncmp(param->downtown, cityTable[loop], strlen(cityTable[loop])) == 0)
        {
            param->cityId = cityIdTable[loop];
        }
    }

    getData = parse_data(NOWTIME, str);
    if (getData != NULL)
    {
        param->nowtime = 1;
    }
    else
    {
        param->nowtime = 0;
    }

    getData = parse_data(BRIGHTNESS, str);
    if (getData != NULL)
    {
        param->brightness = *getData - '0';
    }
    else
    {
        param->brightness = 0;
    }

    getData = parse_data(TEMPFMT, str);
    if (getData != NULL)
    {
        param->tempFmt = 1;
    }
    else
    {
        param->tempFmt = 0;
    }

    getData = parse_data(TIMEFMT, str);
    if (getData != NULL)
    {
        param->timeFmt = 1;
    }
    else
    {
        param->timeFmt = 0;
    }

    memset(param->acid, 0, 30);
    getData = parse_data(ACID, str);
    if (getData != NULL)
    {
        strcpy(param->acid, getData);
    }
    getData = parse_data(DEVICEID, str);
    if (getData != NULL)
    {
        strcpy(param->deviceId, getData);
        printf("device if %s \n", param->deviceId);
    }
#if !VERSION_MUSHAN
    alarm_param_process(ALARM1, ALARMTIME1, SLEEP1, REPEAT1, &param->alarm[0], str);
    alarm_param_process(ALARM2, ALARMTIME2, SLEEP2, REPEAT2, &param->alarm[1], str);
    alarm_param_process(ALARM3, ALARMTIME3, SLEEP3, REPEAT3, &param->alarm[2], str);
#endif
}
void dump_param(tParam *param)
{
    unsigned char loop = 0;
    printf("\r\n apName %s,passwd %s,cityName %s,cityId %d,downtown %s,nowtime %d,timeFmt %d,temp %d,brightness %d \r\n",
           param->apName,
           param->password,
           param->cityName,
           param->cityId,
           param->downtown,
           param->nowtime,
           param->timeFmt,
           param->tempFmt,
           param->brightness,
           param->acid);
#if !VERSION_MUSHAN
    for (; loop < 21; loop++)
    {
        printf("alram default time: day:%d,default:%d,alarm:%d,snooze:%d,snooze open %d,isopen:%d\n",
               param->alarm[loop].alarmDay,
               param->alarm[loop].defaultAlarmTime,
               param->alarm[loop].alarmTime,
               param->alarm[loop].snoozeDelay,
               param->alarm[loop].isOpenSnooze,
               param->alarm[loop].isOpen);
    }
#endif
}
void flash_config(tParam *configParam)
{
    struct sdk *tmpParam = (sdk_param *)malloc(sizeof(sdk_param));
    memcpy(&sdkParam, &g_defparam, sizeof(sdk_param));
    config_get(tmpParam);
    memcpy(sdkParam.sta_param[2].ssid, tmpParam->sta_param[1].ssid,32);
    memcpy(sdkParam.sta_param[2].key, tmpParam->sta_param[1].key,64);

    memcpy(sdkParam.sta_param[1].ssid, tmpParam->sta_param[0].ssid,32);
    memcpy(sdkParam.sta_param[1].key, tmpParam->sta_param[0].key,64);

    free(tmpParam);

    memcpy(sdkParam.sta_param[0].ssid, configParam->apName,32);
    memcpy(sdkParam.sta_param[0].key, configParam->password,64);
    memcpy(sdkParam.acid, configParam->acid,30);
    memcpy(sdkParam.deviceId, configParam->deviceId,30);

    sdkParam.nt_param.isOpen = configParam->nowtime;
    //sdkParam.timeFmt_param.fmt =
    sdkParam.wth_parma.cityId = configParam->cityId;
    sdkParam.temperature = configParam->tempFmt;
    sdkParam.brightness = configParam->brightness;
#if !VERSION_MUSHAN
    memcpy(sdkParam.alm_param, configParam->alarm, ALARM_NUM * sizeof(alarm_param));
#endif
    config_set(&sdkParam);
    config_submit();
    config_dump();
    reboot(1);
}

/*
void
post_data_handle(char* data,unsigned int lenth)
{
	unsigned int len = 0;
	static tParam param;
	memset(&param,0,sizeof(tParam));
	while(lenth--){
		printf("%c",data[len++]);
	}
	
	fillin_param(data,&param);
	dump_param(&param);	
	flash_config(&param);
}
*/
void get_data_handle(char *data, unsigned int lenth)
{
    unsigned int len = 0;
    static unsigned char getData[512] = {
        0,
    };
    static tParam param;
    // printf("------------------>data %s \n",data);
    if (data != NULL && data[13] == '?')
    {
        memcpy(getData, data + 14, lenth - 14);
        while (lenth--)
        {
            printf("%c", getData[len++]);
        }

        fillin_param(data, &param);
        dump_param(&param);
        flash_config(&param);
    }
}
#endif

#if 0
void
get_data_handle(char* data,unsigned int lenth)
{
	unsigned int len = 0;
	static unsigned char getData[300] = {0,};
	if (data != NULL && data[1] == '?' ) {
		memcpy(getData,data + 2,lenth - 1);
		while(lenth--){
			printf("%c",getData[len++]);
		}
	}
}
#endif

const char *
index_handle(int iIndex, int iNumParams, char *pcParam[], char *pcValue[])
{
    printf(" \r\n index handle \r\n");
    return "/index.html";
}

void http_server_start(void *param)
{
    static tCGI pCGIs[] = {
        {"/index.html", (tCGIHandler)index_handle},
    };
    http_set_cgi_handlers(pCGIs, sizeof(pCGIs) / sizeof(pCGIs[0]));
    //	postdata_register_callback(post_data_handle);
    getdata_register_back(get_data_handle);
    httpd_init();
    vTaskDelete(NULL);
}
#endif
