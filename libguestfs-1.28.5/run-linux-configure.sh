#!/bin/bash

QEMU="C:/MinGW/msys/1.0/home/novokrestWin/qemu-windows-ivshmem/build/qemu-system-x86_64.exe"

#automake -a
#autoreconf --install --force
./configure \
    --enable-mingwport \
    --enable-shared-memory \
    --enable-daemon \
    --enable-appliance 
#    --without-qemu
#    --with-qemu=$QEMU
