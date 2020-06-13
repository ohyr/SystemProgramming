#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	int dev;
	int str = 0;
	//char buffer[1024];
	
	dev = open("/dev/syspd", O_RDWR);
	
	if(dev == -1){
		perror("failed to open; because ");
		return 1;
	}
	//printf("dev = %d\n", dev);
	
	printf("Input the number\n");
	scanf("%d", &str);
	write(dev, &str, 4);
	read(dev, &str, 4);
	printf("The accumulated number in kernel space is %d\n", str);
	
	
	close(dev);
	
	exit(EXIT_SUCCESS);
	
}
