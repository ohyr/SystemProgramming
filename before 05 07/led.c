#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define GPIO_BASE_ADDR 0x3F200000
#define GPFSEL1 0x04
#define GPSET0 0x1C
#define GPCLR0 0x28

int main(int argc, char **argv)
{
	int fd = open("/dev/mem", O_RDWR|O_SYNC);
	if(fd < 0){
		printf("fail to open virtual memory\n");
		return -1;
	}
	
	char *gpio_memory_map = (char *)mmap(0, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, GPIO_BASE_ADDR);
	
	if(gpio_memory_map == MAP_FAILED){
		printf("fail to mmap\n");
		return -1;
	}
	
	volatile unsigned int* gpio = (volatile unsigned int*)gpio_memory_map;
	//
	close(fd);
	gpio[GPFSEL1/4] |= (1<<24);
	int i;
	for(i=0;i<5;i++){
		gpio[GPSET0/4] |= (1<<18);
		sleep(1);
		gpio[GPCLR0/4] |= (1<<18);
		sleep(1);
	}
	
	//
	
	munmap(gpio_memory_map, 4096);
	
	return 0;
}

