### Copy from http://ilinuxkernel.com/?p=1248

make

insmod dram.ko

ls /proc/devices | grep dram

mknod /dev/dram c 85 0

g++ fileview.cpp -o fileview

./fileview /dev/dram

