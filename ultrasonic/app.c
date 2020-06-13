#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/sysmacros.h>

#define ULTRASONIC_MAJOR_NUMBER	504
#define ULTRASONIC_MINOR_NUMBER	100
#define ULTRASONIC_DEV_PATH_NAME		"/dev/ultrasonic"

int main(void)
{
	dev_t ultrasonic_dev;
	int fd;
	char buffer[1024];
	int distance = 0;
	
	ultrasonic_dev = makedev(ULTRASONIC_MAJOR_NUMBER, ULTRASONIC_MINOR_NUMBER);
	mknod(ULTRASONIC_DEV_PATH_NAME, S_IFCHR|0666, ultrasonic_dev);
	
	fd = open(ULTRASONIC_DEV_PATH_NAME, O_RDWR);

	if(fd < 0){
		printf("fail to open ultrasonic\n");
		return -1;
	}
	
	while(1){
		read(fd,buffer,1024);
		distance = atoi(buffer);
		printf("%.6f cm\n",(double)distance/1000000); // 10^-6cm -> 1cm
		sleep(1);
	}

	close(fd);
	
	return 0;
}
