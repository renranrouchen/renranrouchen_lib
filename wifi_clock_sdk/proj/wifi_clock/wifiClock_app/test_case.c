
#include <stdio.h>
#define PC      1
#if PC
#include "common.h"
#include "wifi_clock_cfg.h"
#include "ui_display.h"

#if (WEATHER_CHART_TEST == 1)

void test_func(void* arg)
{
    int loop = 0,index = 0;
    ui_display_init(); 
    char* carLimitTable[] = {
        "13579",
        "02468",
        "28",
        "19",
        "46",
        "37"
    };       
    while(1) {
        ui_setup(weatherList[loop],WEATHER_CHAR_DATA);
        ui_setup(carLimitTable[index++],CAR_LIMIT_DATA);
        if (index >= sizeof(carLimitTable) / sizeof(char*)) 
        {
            index = 0;
        }
        ui_display();
        if (++loop > 17) {
            loop = 0;
        }
        sys_msleep(1000 * 5);
    }
}


void start_weather_chart_test()
{
    xTaskCreate(test_func,"testcase",1024 * 6,NULL,2,NULL);
}

#endif

#if (LOCAL_TEMPERATURE_DISPLAY == 1)

void test_func(void* arg)
{
    int loop = 0;
    ui_display_init(); 
    char* testNumber[] = {
        "11",
        "-20",
        "-5",
        "200",
        "2"
    };       
    while(1) {
        ui_setup("CCC",LOCAL_TEMPERATURE_DATA);
        ui_setup(testNumber[loop],LOCAL_TEMPERATURE_DATA);
        // ui_setup("19",CAR_LIMIT_DATA);
        ui_display();
        if (++loop > sizeof(testNumber) / sizeof(char*) - 1) 
        {
            loop = 0;
        }
        sys_msleep(1000 * 5);
    }
}


void start_local_temperature_test()
{
    xTaskCreate(test_func,"testcase",1024 * 6,NULL,2,NULL);
}


#endif

#if (TEST_CASE == 1)
void start_test_case()
{
#if (WEATHER_CHART_TEST == 1)    
    start_weather_chart_test();
#endif
#if (LOCAL_TEMPERATURE_DISPLAY == 1)
    start_local_temperature_test();
#endif

}
#endif



#else
void main()
{
    int = -3;
    printf("-----------> %d %d %d ",-3 / 100, -3 / 20, -3 / 1);
}


#endif



