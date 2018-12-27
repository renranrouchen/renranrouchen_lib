/*
 * Copyright(c) 2016-2100.  zwxf.  All rights reserved.
 */
/*
 * FileName:      web_register.c
 * Author:        zwxf
 * Email:         zwxf.cw@gmail.com
 * DateTime:      2017-12-04 13:54:23
 * ModifiedTime:  2017-12-04 13:54:23
 */

#include <common.h>
#if defined(CONFIG_FREERTOS)
#include <FreeRTOS.h>
#include <task.h>
#endif
#include <wifi_clock_cfg.h>
#include <arch/ba22/chip.h>
#include <arch/irq.h>
#include <ui_display.h>
#include <alarm_setting.h>
#include <cfg_api.h>

#if (VERSION_MUSHAN == 1)
#undef REGISTER_URI
#define REGISTER_URI "/msapi/device"
#undef REGISTER_HOST
#define REGISTER_HOST "pgyep.com"
#undef REGISTER_HOST_ADDR
#define REGISTER_HOST_ADDR "120.55.41.172"
#undef REGISTER_PORT
#define REGISTER_PORT 80

#define POST_MESSAGE "{\
  \"id\": %d,\
  \"version\": \"v1.0\",\
  \"address\": \"浙江省 杭州市 江干区 秋涛北路76号\",\
  \"acid\": \"%s\",\
  \"hotel\": \"\",\
  \"room\": \"\"\
}"

struct httpClient *registerClient = NULL;
enum
{
    REGISTER_OK = 1,
    REGISTER_ERRO_ID,
    REGISTER_WRONG_DATA,
    REFISTER_ERRO_REQUEST,
    REGISTER_FAIL,
};
extern sdk_param sdkParam;

static int result_callback(char *data, int lenth)
{
    printf("\n----------------------------->register callbcack %s", data);
    int ret = -1;
    char *tmp = NULL;
    int httpHeadLen = head_check(data);
    if (httpHeadLen < 0)
    {
        registerClient->state = REGISTER_FAIL;
        return -1;
    }
    registerClient->state = REGISTER_OK;
    /* Flash result */
    sdkParam.webRegisterStatue = true;
    config_set(&sdkParam);
    config_submit();
    free(registerClient);
    registerClient = NULL;
    // config_get(&sdkParam);
    // config_dump();
    // reboot(1);
}

static void start_register(struct httpClient *client, int (*data_handle_cb)(char *, int))
{
    if (s_linkup != CONNECT)
    {
        goto erro;
    }
    int ret = -1;
    ret = connectServer(client);
    if (ret < 0)
    {
        // printf("---------------->connect erro \n");
        goto erro;
    }
    /* if connect fail when starting wifi */
    ret = httpPost(client->clientfd, client->host, client->uri, client->postData);
    if (ret < 0)
    {
        // printf("---------------->post erro \n");
        goto erro;
    }
    if (start_read_data(client, data_handle_cb) < 0)
    {
        // printf("================read erro\r\n");
        goto erro;
    }
    // printf("update success %d", weather.update.updateFail);
    close(client->clientfd);
    client->clientfd = -1;
    return;
erro:
    if (client->clientfd != -1)
    {
        close(client->clientfd);
        client->clientfd = -1;
    }
    client->state = REGISTER_FAIL;
}

static void inline init_register_param()
{
    unsigned long deviceId;
    registerClient = (struct httpClient *)malloc(sizeof(struct httpClient));
    memset(registerClient, 0, sizeof(struct httpClient));
    if (registerClient == NULL)
    {
        serial_printf("---------------->FUNC=%s fail", __FUNCTION__);
    }
    else
    {
        registerClient->host = REGISTER_HOST;
        registerClient->uri = REGISTER_URI;
        registerClient->port = 80;
        registerClient->iaddr = REGISTER_HOST_ADDR;
        registerClient->state = REGISTER_FAIL;
        registerClient->postData = malloc(strlen(POST_MESSAGE) + 30);
        deviceId = atoi(sdkParam.deviceId);
        sprintf(registerClient->postData, POST_MESSAGE, deviceId, sdkParam.acid);
    }
}

void register_id_to_web(void *arg)
{
    if (sdkParam.webRegisterStatue == true)
    {
        vTaskDelete(NULL);
    }
    init_register_param();
    while (1)
    {
        switch (registerClient->state)
        {
        case REGISTER_FAIL:
            start_register(registerClient, result_callback);
            break;
        case REGISTER_OK:
            vTaskDelete(NULL);
        default:
            break;
        }
        sys_msleep(10000);
    }
}

#endif
