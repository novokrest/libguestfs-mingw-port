#!/bin/bash

make -C ../src -f MakefileLinux clean && make -C ../src -f MakefileMinGW
cp ../src/g .
rm -f guestfs.h && cp ../src/guestfs.h .
make clean
make
