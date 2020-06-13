sudo rmmod ultrasonic
make clean
make
sudo insmod ultrasonic.ko
rm app
gcc -o app app.c
sudo ./app
