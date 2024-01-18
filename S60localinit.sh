#!bin/sh

sleep 10

#install drivers
insmod /etc/led.ko

#mount message queue
mkdir /dev/mqueue
mount -t mqueue none/dev/mqueue

#start daemon
./etc/daemon

#start program
./etc/main
