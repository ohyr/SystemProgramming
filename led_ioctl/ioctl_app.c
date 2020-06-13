#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/sysmacros.h>

#define LED_MAJOR_NUMBER	502
#define LED_MINOR_NUMBER	100
#define LED_DEV_PATH_NAME		"/dev/led_ioctl"

#define IOCTL_MAGIC_NUMBER		'j'
#define IOCTL_CMD_SET_DIRECTION		_IOWR(IOCTL_MAGIC_NUMBER, 0, int)
#define IOCTL_CMD_BLINK				_IOWR(IOCTL_MAGIC_NUMBER, 1, int)

int main(void)
{
	dev_t led_dev;
	int fd;
	int pin_direction = -1, numLoop = 0;
	
	led_dev = makedev(LED_MAJOR_NUMBER, LED_MINOR_NUMBER);
	mknod(LED_DEV_PATH_NAME, S_IFCHR|0666, led_dev);
	
	fd = open(LED_DEV_PATH_NAME, O_RDWR);
	
	if(fd < 0){
		printf("fail to open led\n");
		return -1;
	}
	
	pin_direction = 1;
	ioctl(fd, IOCTL_CMD_SET_DIRECTION, &pin_direction); //set pin out
	
	numLoop = 5;
	ioctl(fd, IOCTL_CMD_BLINK, &numLoop);
	
	close(fd);
	
	return 0;
}
