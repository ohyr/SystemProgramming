#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/sysmacros.h>

#define BUTTON_MAJOR_NUMBER	503
#define BUTTON_MINOR_NUMBER	101
#define BUTTON_DEV_PATH_NAME		"/dev/button_dev"

#define LED_MAJOR_NUMBER	502
#define LED_MINOR_NUMBER	100
#define LED_DEV_PATH_NAME		"/dev/led_dev"

#define IOCTL_MAGIC_NUMBER		'j'
#define IOCTL_CMD_SET_DIRECTION		_IOWR(IOCTL_MAGIC_NUMBER, 0, int)
#define IOCTL_CMD_SET_LED_ON		_IOWR(IOCTL_MAGIC_NUMBER, 1, int)
#define IOCTL_CMD_SET_LED_OFF		_IOWR(IOCTL_MAGIC_NUMBER, 2, int)

#define LED_STATUS_OFF	0
#define LED_STATUS_ON	1

//#define INTERVAL	100000

int main(void)
{
	dev_t led_dev;
	dev_t button_dev;
	int fd;
	int fd2;
	int pin_direction = -1;
	
	led_dev = makedev(LED_MAJOR_NUMBER, LED_MINOR_NUMBER);
	mknod(LED_DEV_PATH_NAME, S_IFCHR|0666, led_dev);
	fd = open(LED_DEV_PATH_NAME, O_RDWR);
	if(fd < 0){
		printf("fail to open led\n");
		return -1;
	}
	
	button_dev = makedev(BUTTON_MAJOR_NUMBER, BUTTON_MINOR_NUMBER);
	mknod(BUTTON_DEV_PATH_NAME, S_IFCHR|0666, button_dev);
	fd2 = open(BUTTON_DEV_PATH_NAME, O_RDWR);
	if(fd2 < 0){
		printf("fail to open button\n");
		return -1;
	}

	pin_direction = 1;
	ioctl(fd, IOCTL_CMD_SET_DIRECTION, &pin_direction); //set pin out
	
	int led_status = LED_STATUS_ON; //INIT LED OFF
	int current_button_value = 0, prev_button_value=0;
	
	while(1){
		//usleep(INTERVAL);
		prev_button_value = current_button_value;
		
		read(fd2, &current_button_value, sizeof(int));
		
		// rasing edge detection
		if(prev_button_value == 0 && current_button_value != 0){
			if(led_status == LED_STATUS_ON){
				ioctl(fd, IOCTL_CMD_SET_LED_OFF);
				led_status = LED_STATUS_OFF;
				printf("off\n");
			}else{
				ioctl(fd, IOCTL_CMD_SET_LED_ON);
				led_status = LED_STATUS_ON;
				printf("on\n");
			}
		}
		
	}
	
	close(fd);
	close(fd2);
	
	return 0;
}
