#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define GPIO_BASE_ADDR 0x3F200000
#define GPFSEL1 0x04
#define GPFSEL2 0x08
#define GPSET0 0x1C
#define GPCLR0 0x28
#define GPLEV0 0x34

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
	gpio[GPCLR0/4] |= (1<<18); // GPIO18 -> 0
	gpio[GPFSEL2/4] |= (1<<3); // GPIO21 -> OUT	
	gpio[GPSET0/4] |= (1<<21); // GPIO21 -> 1
	int button;
	int button2;
	int onoff = 0;
	while(1){
		gpio[GPSET0/4] |= (1<<21); // GPIO21 -> 1
		button = gpio[GPLEV0/4] & (1<<20); // GPIO20 -> button	
		button2 = gpio[GPLEV0/4] & (1<<21); // GPIO21 -> button2	
		if( onoff > 0 && button2 == 0){ //if button on state
			printf("on\n");
			while(1){
				printf("on down\n");
				button2 = gpio[GPLEV0/4] & (1<<20); // GPIO20 -> button
				if(button2 > 0){
					gpio[GPCLR0/4] |= (1<<18);
					printf("on up\n");
					onoff = 0;
					break;
				}			
			}
		}
		if(onoff == 0 && button2 == 0){
			printf("off\n");
			while(1){
				printf("off down\n");
				gpio[GPSET0/4] |= (1<<21); // GPIO21 -> 1
				button2 = gpio[GPLEV0/4] & (1<<20); // GPIO20 -> button
				if(button2 > 0){
					gpio[GPSET0/4] |= (1<<18);
					printf("off up\n");
					onoff = 1;
					break;
				}				
			}
		}
		
	}
	//
	
	munmap(gpio_memory_map, 4096);
	
	return 0;
}

