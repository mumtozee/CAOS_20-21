#!/bin/bash
arm-linux-gnueabi-gcc -marm $1.c $1.S -o $1.exe
sudo qemu-arm -L /home/mumtozee/arm/gcc_linaro/arm-linux-gnueabi/libc ./$1.exe