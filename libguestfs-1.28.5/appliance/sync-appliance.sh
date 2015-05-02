#!/bin/bash

sudo mount -o loop appliance.d/root loop-dir
sudo cp -r /usr/share/augeas loop-dir/usr/share
sudo umount loop-dir
