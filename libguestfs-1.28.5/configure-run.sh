#!/bin/bash

QEMU="C:/MinGW/msys/1.0/home/novokrestWin/qemu-windows-ivshmem/build/qemu-system-x86_64.exe"

autoconf
./configure \
    --enable-mingwport \
    --disable-daemon \
    --disable-appliance \
    --without-qemu
#    --with-qemu=$QEMU