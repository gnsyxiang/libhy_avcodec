/**
 *
 * Release under GPLv-3.0.
 * 
 * @file    hy_g711.h
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/12 2021 18:30
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/12 2021      create the file
 * 
 *     last modified: 25/12 2021 18:30
 */
#ifndef __LIBHY_AVCODEC_INCLUDE_HY_G711_H_
#define __LIBHY_AVCODEC_INCLUDE_HY_G711_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hy_hal/hy_type.h"

typedef enum {
    HY_G711_A_LAW,
    HY_G711_MU_LAW,

    HY_G711_MAX,
} HyG711Type_t;

/**
 * @brief pcm编码成g711
 *
 * @param type g711类型，详见HyG711Type_t
 * @param pcm pcm数据
 * @param pcm_len pcm数据长度
 * @param g711 g711数据
 *
 * @return g711数据长度
 */
hy_u32_t HyG711Encode(HyG711Type_t type,
        const char *pcm, hy_u32_t pcm_len, char *g711);

/**
 * @brief g711解码成pcm
 *
 * @param type g711类型，详见HyG711Type_t
 * @param g711 g711数据
 * @param g711_len g711数据长度
 * @param pcm pcm数据
 *
 * @return pcm数据长度
 */
hy_u32_t HyG711Decode(HyG711Type_t type,
        const char *g711, hy_u32_t g711_len, char *pcm);

#ifdef __cplusplus
}
#endif

#endif
