#!/bin/bash

mkdir appliance.d

/usr/local/bin/supermin \
  --build \
  --verbose \
  --lock /var/tmp/.guestfs-1000/lock \
  --copy-kernel \
  --host-cpu x86_64 \
  -f ext2 \
  supermin.d \
  -o appliance.d

