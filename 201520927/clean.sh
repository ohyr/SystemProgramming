cd led_dev
sudo rmmod led_dev
make clean
cd ../button_dev
sudo rmmod button_dev
make clean
cd ../control_app
rm app