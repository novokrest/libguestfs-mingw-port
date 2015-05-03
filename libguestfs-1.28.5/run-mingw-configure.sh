#!/bin/bash

QEMU="C:/MinGW/msys/1.0/home/novokrestWin/qemu-windows-ivshmem/build/qemu-system-x86_64.exe"

#automake -a
#autoreconf --install --force
./configure -C \
    --enable-mingwport \
    --enable-shared-memory \
    --disable-daemon \
    --disable-appliance \
    --with-qemu=no
