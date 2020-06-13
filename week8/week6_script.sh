sudo rmmod led_dev
make clean
make
sudo insmod led_dev.ko
rm led
gcc -o led led.c
