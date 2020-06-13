cd led_dev
sudo rmmod led_dev
make clean
make
sudo insmod led_dev.ko
cd ../button_dev
sudo rmmod button_dev
make clean
make
sudo insmod button_dev.ko
cd ../control_app
rm app
gcc -o app app.c
sudo ./app