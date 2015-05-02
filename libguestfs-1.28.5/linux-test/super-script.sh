#!/bin/bash

cd ..

make -f MakefileLinux -C osdep clean
make -f MakefileLinux -C daemon clean
make -f MakefileLinux -C src clean
make -f MakefileLinux -C appliance clean

make -f MakefileLinux -C osdep
cp osdep/libosdep.a daemon/
cp osdep/libosdep.a src/

make -f MakefileLinux -C src
make -f MakefileLinux -C daemon

make -f MakefileLinux -C appliance
cd appliance
./sync-appliance.sh
#sudo rm -r /usr/local/lib/guestfs/*
sudo cp -r supermin.d /usr/local/lib/guestfs
rm -r /var/tmp/.guestfs-1000/appliance.d/
mv appliance.d /var/tmp/.guestfs-1000/

cd ..
cd linux-test
cp ../src/libguestfs.so .
make clean && make
export LD_LIBRARY_PATH=. && ./test.exe
