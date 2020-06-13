#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/sysmacros.h>

#define LED_MAJOR_NUMBER	501
#define LED_MINOR_NUMBER	100
#define LED_DEV_PATH_NAME		"/dev/led"

int main(int argc, char **argv)
{
	dev_t led_dev;
	int fd;
	int led_state = 0;
	
	led_dev = makedev(LED_MAJOR_NUMBER, LED_MINOR_NUMBER);
	mknod(LED_DEV_PATH_NAME, S_IFCHR|0666, led_dev);
	
	fd = open(LED_DEV_PATH_NAME, O_RDWR);
	
	if(fd < 0){
		printf("fail to open led\n");
		return -1;
	}
	
	while(1){
		led_state = 1;
		write(fd, &led_state, 4);
		sleep(1);
		led_state = 0;
		write(fd, &led_state, 4);
		sleep(1);
	}
	
	close(fd);
	
	return 0;
}
