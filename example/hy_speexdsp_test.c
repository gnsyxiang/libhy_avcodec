/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_speexdsp_test.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/12 2021 20:16
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/12 2021      create the file
 * 
 *     last modified: 25/12 2021 20:16
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hy_hal/hy_module.h"
#include "hy_hal/hy_hal_utils.h"
#include "hy_hal/hy_mem.h"
#include "hy_hal/hy_string.h"
#include "hy_hal/hy_signal.h"
#include "hy_hal/hy_type.h"

#include "hy_utils/hy_log.h"

#include "speex/speex_resampler.h"

typedef struct {
    void        *log_handle;
    void        *signal_handle;

    hy_s32_t    exit_flag;
} _main_context_t;

static void _signal_error_cb(void *args)
{
    LOGE("------error cb\n");

    _main_context_t *context = args;
    context->exit_flag = 1;
}

static void _signal_user_cb(void *args)
{
    LOGE("------user cb\n");

    _main_context_t *context = args;
    context->exit_flag = 1;
}

static void _module_destroy(_main_context_t **context_pp)
{
    _main_context_t *context = *context_pp;

    // note: 增加或删除要同步到module_create_t中
    module_destroy_t module[] = {
        {"signal",  &context->signal_handle,    HySignalDestroy},
        {"log",     &context->log_handle,       HyLogDestroy},
    };

    RUN_DESTROY(module);

    HY_MEM_FREE_PP(context_pp);
}

static _main_context_t *_module_create(void)
{
    _main_context_t *context = HY_MEM_MALLOC_RET_VAL(_main_context_t *,
            sizeof(*context), NULL);

    HyLogConfig_t log_config;
    log_config.save_config.buf_len      = 1024 * 10;
    log_config.save_config.level        = HY_LOG_LEVEL_INFO;
    log_config.save_config.color_enable = HY_TYPE_FLAG_ENABLE;

    int8_t signal_error[HY_SIGNAL_NUM_MAX_32] = {
        SIGQUIT, SIGILL, SIGTRAP, SIGABRT, SIGFPE,
        SIGSEGV, SIGBUS, SIGSYS, SIGXCPU, SIGXFSZ,
    };

    int8_t signal_user[HY_SIGNAL_NUM_MAX_32] = {
        SIGINT, SIGTERM, SIGUSR1, SIGUSR2,
    };

    HySignalConfig_t signal_config;
    memset(&signal_config, 0, sizeof(signal_config));
    HY_MEMCPY(signal_config.error_num, signal_error, sizeof(signal_error));
    HY_MEMCPY(signal_config.user_num, signal_user, sizeof(signal_user));
    signal_config.save_config.app_name          = "template";
    signal_config.save_config.coredump_path     = "./";
    signal_config.save_config.error_cb          = _signal_error_cb;
    signal_config.save_config.user_cb           = _signal_user_cb;
    signal_config.save_config.args              = context;

    // note: 增加或删除要同步到module_destroy_t中
    module_create_t module[] = {
        {"log",     &context->log_handle,       &log_config,        (create_t)HyLogCreate,      HyLogDestroy},
        {"signal",  &context->signal_handle,    &signal_config,     (create_t)HySignalCreate,   HySignalDestroy},
    };

    RUN_CREATE(module);

    return context;
}

static void _speexdsp_test(void)
{
    #define _SAMPLE_POINT (160)
    int err = 0;
    SpeexResamplerState *resampler = NULL;
    char pcm_in[_SAMPLE_POINT * sizeof(short)] = {0};
    char pcm_out[_SAMPLE_POINT * 2] = {0};
    FILE *fp_pcm_16k;
    FILE *fp_pcm_8k;
    hy_u32_t ret;
    hy_u32_t out_len = 0;

    fp_pcm_16k = fopen("../res/test.pcm", "r");
    if (!fp_pcm_16k) {
        LOGES("failed \n");
        return ;
    }

    fp_pcm_8k = fopen("../res/test_8k.pcm", "w");
    if (!fp_pcm_8k) {
        LOGES("failed \n");
        return ;
    }

    resampler = speex_resampler_init(1, 16000, 8000, 10, &err);
    if (!resampler) {
        LOGE("failed \n");
    }

    while (1) {
        HY_MEMSET(pcm_in, sizeof(pcm_in));
        ret = fread(pcm_in, sizeof(short), _SAMPLE_POINT, fp_pcm_16k);
        if (ret == 0) {
            LOGES("failed \n");
            break;
        }

        ret = _SAMPLE_POINT;
        out_len = _SAMPLE_POINT;
        HY_MEMSET(pcm_out, sizeof(pcm_out));
        ret = speex_resampler_process_int(resampler, 0, (short*)pcm_in, &ret, (short*)pcm_out, &out_len);
        if(ret != RESAMPLER_ERR_SUCCESS) {
            LOGE("failed, ret: %d \n", ret);
            break;
        }

        fwrite(pcm_out, sizeof(short), out_len, fp_pcm_8k);
    }

    if (fp_pcm_16k) {
        fclose(fp_pcm_16k);
    }

    if (fp_pcm_8k) {
        fclose(fp_pcm_8k);
    }
}

int main(int argc, char *argv[])
{
    _main_context_t *context = _module_create();
    if (!context) {
        LOGE("_module_create faild \n");
        return -1;
    }

    LOGE("version: %s, data: %s, time: %s \n", "0.1.0", __DATE__, __TIME__);

    _speexdsp_test();

    while (!context->exit_flag) {
        sleep(1);
    }

    _module_destroy(&context);

    return 0;
}
