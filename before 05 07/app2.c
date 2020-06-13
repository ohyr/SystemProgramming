#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
	int dev;
	char* str = "Hello Device Driver!\n";
	char buffer[1024];
	
	printf("Device Driver Test\n");
	
	dev = open("/dev/syspd", O_RDWR);
	
	if(dev == -1){
		perror("failed to open; because ");
		return 1;
	}
	
	printf("dev = %d\n", dev);
	
	write(dev, str, strlen(str));
	read(dev, buffer, strlen(str));
	
	printf("Read from Device : %s\n", buffer);
	
	close(dev);
	
	exit(EXIT_SUCCESS);
	
}
