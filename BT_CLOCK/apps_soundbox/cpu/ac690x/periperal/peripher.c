/**
 * 
 * 
 * 
 * 
 * 
 * 
 * 
 */
#include "sdk_cfg.h"
#include "common/common.h"
#include "timer_api.h"
#include "timer.h"
#if (ALL_FUNCTION_VERSION == 1)


#define get_vibrate_value()     (JL_PORTC->IN & BIT(1))
static char vibrateFlag = 0;
#define VIBRATE_IO_NUM             1
#define TEMPERATURE_GATHER_IO_NUM  2

#define is_vibrated()   (JL_PORTC->IN & (BIT(VIBRATE_IO_NUM)))

void check_display_status()
{
    if (get_low_power_statue() == true && get_display_off_statue() == true && vibrateFlag == true) {
        leave_display_off_mode();
    } 
}

void handle_vibrable(void* arg)
{
#define VIBTATE_SENSITIVE   3    
    static unsigned vibrateCount = 0;
    if (is_vibrated() == true) {
        vibrateCount++;
    } else {
        vibrateCount = 0;
    }   

    // FIXME: if vibrateCount bigger than VIBTATE_SENSITIVE ,we should handle this situation; 
    if (vibrateCount > VIBTATE_SENSITIVE && vibrateCount < VIBTATE_SENSITIVE + 5) {
        vibrateFlag = true;
        printf("-------------------------------------->vibrate \n");
    }
}

void handle_temperatur_process(void* arg)
{
    printf("------------------------------------>temperature handle \n");
}

void peripher_init()
{
    // vibrate init
    JL_PORTC->DIR |= (BIT(1) | BIT(2));   
}


void regiter_periphral_handle()
{
    s32 ret;
    peripher_init();    
    ret = timer_reg_isr_fun(timer0_hl,500,handle_vibrable,NULL);
    ret = timer_reg_isr_fun(timer0_hl,500,handle_temperatur_process,NULL);
}

#endif
