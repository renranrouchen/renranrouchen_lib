#include <string.h>
#include "production_config_param.h"
#if (ALL_FUNCTION_VERSION == 1)
static config_param_t configParam;
void init_config_param(void)
{
    memset(&configParam,0,sizeof(config_param_t));
}

config_param_t get_config_param(void)
{
    return configParam;
}

config_param_t* get_config_param_point()
{
    return &configParam;
}
/**
 * @brief
 * @note
 * @retval None
 */
void set_alarm_up()
{
    configParam.powerConfig.alarmStatue = true;
}

void set_alarm_down()
{
    configParam.powerConfig.alarmStatue = false;
}

BOOL get_alarm_statue()
{
    return configParam.powerConfig.alarmStatue;
}
/**
 * @brief
 * @note
 * @retval None
 */
void set_display_statue_true()
{
    configParam.powerConfig.displayStatue = true;
}

void set_display_statue_false()
{
    configParam.powerConfig.displayStatue = false;
}

BOOL get_display_statue()
{
    return configParam.powerConfig.displayStatue;
}

/**
 * @brief
 * @note
 * @retval None
 */
void set_external_power_true()
{
    configParam.powerConfig.exteralPowerMode = true;
}

void set_extern_power_false()
{
    configParam.powerConfig.exteralPowerMode = false;
}

BOOL get_externpower_statue()
{
    return configParam.powerConfig.exteralPowerMode;
}

/**
 * @brief
 * @note
 * @retval None
 */
void set_displayup_count_down2zero()
{
    configParam.powerConfig.displayUpCountDown = 0;
}

s8 get_display_countdown_value()
{
    return configParam.powerConfig.displayUpCountDown;
}

/**
 * @brief
 * @note
 * @retval None
 */
void enrty_lower_power_mode()
{
    configParam.powerConfig.lowPowerMode = true;
}

void leave_low_power_mode()
{
    configParam.powerConfig.lowPowerMode = false;
}

BOOL get_low_power_statue()
{
    return configParam.powerConfig.lowPowerMode;
}

/**
 * @brief
 * @note
 * @retval None
 */
void entry_display_off_mode()
{
    configParam.powerConfig.displayOffMode = true;
}

void leave_display_off_mode()
{
    configParam.powerConfig.displayOffMode = false;
}

BOOL get_display_off_statue()
{
    return configParam.powerConfig.displayOffMode;
}

/**
 * @brief
 * @note
 * @retval None
 */
void enrty_time_format_to_24()
{
    configParam.TimeFormat24 = true;
}

void leave_time_format_to_24()
{
    configParam.TimeFormat24 = false;
}

BOOL get_time_format_statue()
{
    return configParam.TimeFormat24;
}
/**
 * @brief
 * @note
 * @retval None
 */
void set_degree()
{
    configParam.degreeFormat = true;
}

void leave_degree()
{
    configParam.degreeFormat = false;
}

BOOL get_degree_statue()
{
    return configParam.degreeFormat;
}

#endif