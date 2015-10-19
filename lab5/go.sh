sudo insmod hello.ko || exit
sudo chmod a+w+r /sys/kernel/hello/delay
sudo echo 5 > /sys/kernel/hello/delay
sudo rmmod hello
