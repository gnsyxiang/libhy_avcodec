#!/usr/bin/env bash

# set -x

help_info()
{
    echo "eg: ./build.sh pc/arm [_build]"
    exit
}

if [ $# -gt 2 -o $# -lt 1 ]; then
    help_info
fi

data_disk_path=/opt/data

_cppflags_com=""
_cflags_com=""
_cxxflags_com=""
_ldflag_com=""
_param_com=""

_cppflags_com="${_cppflags_com} -W -Wall -Werror"
_cppflags_com="${_cppflags_com} -Wno-unused-parameter"
_cppflags_com="${_cppflags_com} -Wno-error=unused-but-set-variable"
_cppflags_com="${_cppflags_com} -Wno-error=unused-variable"
_cppflags_com="${_cppflags_com} -Wno-error=unused-function"
_cppflags_com="${_cppflags_com} -pipe"
_cppflags_com="${_cppflags_com} -ffunction-sections"
_cppflags_com="${_cppflags_com} -fdata-sections"
_cppflags_com="${_cppflags_com} -fstack-protector-all"

_ldflag_com="${_ldflag_com} -rdynamic"
_ldflag_com="${_ldflag_com} -Wl,--gc-sections"
_ldflag_com="${_ldflag_com} -Wl,--as-needed"
_ldflag_com="${_ldflag_com} -Wl,-rpath=../lib"

if [ x$1 = x"pc" ]; then
    vender=pc
    gcc_version=x86_64-linux-gnu
elif [ x$1 = x"arm" ]; then
    vender=hisi
    host=arm-himix200-linux
    gcc_version=arm-himix200-linux
    gcc_prefix=arm-himix200-linux
    cross_gcc_path=${data_disk_path}/opt/toolchains/${vender}/${gcc_version}/bin/${gcc_prefix}-
else
    help_info
fi

# 3rd_lib path
prefix_path=${data_disk_path}/install/${vender}/${gcc_version}
_cppflags_com="${_cppflags_com} -I${prefix_path}/include"
_ldflag_com="${_ldflag_com} -L${prefix_path}/lib"

make distclean

cd `pwd` && ./autogen.sh && cd -

if [ $# = 2 ]; then
    mkdir -p $2/${vender}
    cd $2/${vender}
fi

`pwd`/configure                                             \
    CC=${cross_gcc_path}gcc                                 \
    CXX=${cross_gcc_path}g++                                \
    CPPFLAGS="${_cppflags_com}"                             \
    CFLAGS="${_cflags_com}"                                 \
    CXXFLAGS="${_cxxflags_com}"                             \
    LDFLAGS="${_ldflag_com}"                                \
    LIBS=""                                                 \
    PKG_CONFIG_PATH="${prefix_path}/lib/pkgconfig"          \
    --prefix=${prefix_path}                                 \
    --build=                                                \
    --host=${host}                                          \
    --target=${host}                                        \
    \
    ${_param_com}


thread_jobs=`getconf _NPROCESSORS_ONLN 2>/dev/null || echo 1`

make -j${thread_jobs} && make install
