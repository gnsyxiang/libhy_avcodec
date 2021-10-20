/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    main.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    20/10 2021 08:32
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        20/10 2021      create the file
 * 
 *     last modified: 20/10 2021 08:32
 */
#include <stdio.h>

#include "module_a.h"

int main(int argc, char const* argv[])
{
    printf("hello world \n");

    HyModuleAInit();

    return 0;
}
