#ifndef PRODUCT_CONFIG_PARAM
#define PRODUCT_CONFIG_PARAM
#include "common/common.h"
#include "sdk_cfg.h"

#if (ALL_FUNCTION_VERSION == 1)
typedef enum {
    POWER_NORMAL_MODE = 0,
    POWER_LOW_MODE,
    POWER_NODISPLAY_MODE,
}POWER_MODE;


typedef struct {

    BOOL exteralPowerMode;
    BOOL displayStatue;
    s8 displayUpCountDown;
    BOOL alarmStatue;
    // Normal mode ,low power mode ,do not display
    s8 displayMode;
    // Use for vibrate function
    BOOL lowPowerMode;
    BOOL displayOffMode;
}power_config_t;

typedef struct config_param {
    // if true is 24 time format
    BOOL TimeFormat24;
    BOOL degreeFormat;

    power_config_t powerConfig;
}config_param_t;

#define STATUE_CHECK(name,checkFlag)    do {    \
    if (#name() == checkFlag) {                 \
        true;                                   \
    } else {                                    \
        false;                                  \    
    }                                           \
}while(0);

void init_config_param(void);
config_param_t get_config_param(void);
config_param_t* get_config_param_point();
/**
 * @brief
 * @note
 * @retval None
 */
void set_alarm_up();
void set_alarm_down();
BOOL get_alarm_statue();
/**
 * @brief
 * @note
 * @retval None
 */
void set_display_statue_true();
void set_display_statue_false();
BOOL get_display_statue();
/**
 * @brief
 * @note
 * @retval None
 */
void set_external_power_true();
void set_extern_power_false();
BOOL get_externpower_statue();
/**
 * @brief
 * @note
 * @retval None
 */
void set_displayup_count_down2zero();
s8 get_display_countdown_value();
/**
 * @brief
 * @note
 * @retval None
 */
void enrty_lower_power_mode();
void leave_low_power_mode();
BOOL get_low_power_statue();
/**
 * @brief
 * @note
 * @retval None
 */
void entry_display_off_mode();
void leave_display_off_mode();
BOOL get_display_off_statue();
/**
 * @brief
 * @note
 * @retval None
 */
void enrty_time_format_to_24();
void leave_time_format_to_24();
BOOL get_time_format_statue();
/**
 * @brief
 * @note
 * @retval None
 */
void set_degree();
void leave_degree();
BOOL get_degree_statue();
#endif





#endif
